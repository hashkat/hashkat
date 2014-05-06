#!/usr/bin/env python
 
#################################################################
# Generates INFILE parameters that are tricky to alter by hand
# By creating value tables with a Python script, we are able to
# simplify the logic in the C++, not worrying about the various
# functions a user might want to supply.
#
# Dependencies:
#   PyYAML
#   SciPY
#################################################################

#################################################################
# PLACE CONVENIENCE FUNTIONS FOR INFILE.yaml HERE
#################################################################

# Convenience rates, in minutes
minute = 60
hour = minute * 60
day = 24 * hour
year = 365 * day

#################################################################
# CEASE PLACING CONVENIENCE FUNCTIONS.
#################################################################

import yaml
import sys

from pprint import pprint
from math import *
from scipy.integrate import quad # For observation PDF integration

def get_var_arg(test, default_val):
    for i in range(len(sys.argv) - 1): 
        if sys.argv[i] == test:
            return sys.argv[i+1]
    return default_val

INPUT_FILE_NAME = get_var_arg("--input", "INFILE.yaml")
DEFAULT_FILE_NAME = "DEFAULT.yaml"

print("INFILE.py -- Loading defaults from " + DEFAULT_FILE_NAME)
print("INFILE.py -- Generating rates for " + INPUT_FILE_NAME)

#################################################################
# Load the relevant pieces of the config.
# We will add a 'generated' node to this, and emit it as INFILE-generated.yaml
#################################################################

DEFAULT_CONFIG = yaml.load(open(DEFAULT_FILE_NAME, "r"))
CONFIG = yaml.load(open(INPUT_FILE_NAME, "r"))

# Merges all not found in 'dst'
def merge_part(src, dst, label):
    if label not in dst:
        dst[label] = src[label]
        return
    else:
        dst, src = dst[label],src[label]
    for k in src: 
        if k not in dst:
            dst[k] = src[k]

def default_check(src, dst, label):
    if label not in dst:
        dst[label] = src[label]

# Merges all not found in 'dst'
def merge_config(src, dst):
    # Mimics structure of INFILE.yaml
    merge_part(src, dst, "analysis")
    merge_part(src, dst, "rates")
    merge_part(src, dst, "output")
    merge_part(src, dst, "tweet_ranks")
    merge_part(src, dst, "retweet_ranks")
    merge_part(src, dst, "follow_ranks")
    merge_part(src, dst, "tweet_observation")

    default_check(src, dst, "ideologies")
    default_check(src, dst, "regions")

    merge_part(src, dst, "config_static")

    default_check(src, dst, "preference_classes")
    default_check(src, dst, "entities")
    
merge_config(DEFAULT_CONFIG, CONFIG)

for k in DEFAULT_CONFIG:
	print k

entities = CONFIG["entities"]

#################################################################
# Both functions are computed from a lookup table generated below.
# Note that the relevance factor is a many-dimensional function,
# whilst tweet_obs takes only time.
#################################################################

obs_pdf = CONFIG["tweet_observation"]
regions = CONFIG["regions"]

def load_observation_pdf(content):
    exec('def __TEMP(x): return ' + str(content))
    return __TEMP # A hack

tweet_obs_density_function = load_observation_pdf(obs_pdf["density_function"])
tweet_obs_x_start = obs_pdf["x_start"]
tweet_obs_x_end = obs_pdf["x_end"]
tweet_obs_initial_resolution = obs_pdf["initial_resolution"]
tweet_obs_resolution_growth_factor = obs_pdf["resolution_growth_factor"]
tweet_obs_time_span = obs_pdf["time_span"]

if isinstance(tweet_obs_time_span, str): # Allow for time constants
    tweet_obs_time_span = eval(tweet_obs_time_span)

config_static = CONFIG["config_static"]

humour_bins = config_static["humour_bins"]

pref_classes = CONFIG["preference_classes"]

#################################################################
# Make the region data easier to parse for the C++:
#################################################################

def weights_to_probs(weights, map, n):
    ret = []
    for i in range(n): ret.append(0)
    total_sum = 0
    for k in weights:
        total_sum += weights[k]
    for k in weights:
        ret[map[k]] = weights[k] / float(total_sum)
    return ret

lang_order = {
    "English" : 0,
    "French" : 1,
    "French+English" : 2
}

lang_n = 3
ideo_order,pref_order = {},{}
ideo_n, pref_n  = 0, 0
for p in CONFIG["ideologies"]:
    ideo_order[p["name"]] = ideo_n
    ideo_n += 1
for p in CONFIG["preference_classes"]:
    pref_order[p["name"]] = pref_n
    pref_n += 1

def preprocess_weights(ret,orig):
    ret["ideology_probs"] = weights_to_probs(orig["ideology_weights"], ideo_order, ideo_n)
    ret["language_probs"] = weights_to_probs(orig["language_weights"], lang_order, lang_n)
    ret["preference_class_probs"] = weights_to_probs(orig["preference_class_weights"], pref_order, pref_n)
    return ret

def preprocess_region(region, add_weight_total):
    ret = {}
    preprocess_weights(ret, region)
    ret["name"] = region["name"]
    ret["add_prob"] = region["add_weight"] / add_weight_total
    return ret

def preprocess_regions():
    ret = []
    total_weight = 0.0
    for region in regions: total_weight += region["add_weight"]
    for region in regions:
        ret.append(preprocess_region(region, total_weight))
    return ret

#################################################################
# Tweet observation probability function integration and binning
# Using 'compute_tweet_obs', we compute the rate bins that correspond 
# to the time that a tweet has been active. These bins control how the
# relevance function below drops off over time. 
#
# If the relevance function is 1 for a person viewing a tweet, in theory 
# that person will always retweet it, given enough time. 
# Note, however, that due to the random-select nature of KMC this cannot be guaranteed.
#################################################################

def tweet_observation_integral(x1, x2):
    val,err = quad(tweet_obs_density_function, x1, x2)
    return val

# Since we bin logarithmatically, we must do a weighted normalization considering
# the span of the observation bin.
def normalize_tweet_obs(rates, spans):
    rate_sum = 0
    # Computed a weighted sum according to the span of the bin:
    for i in range(len(rates)):
        rate_sum += rates[i] * spans[i]

    # Normalize the rates to form a PDF:
    for i in range(len(rates)):
        rates[i] /= rate_sum
        #print(str(spans[i]) + ' ' + str(rates[i])) #Uncomment for simple, plottable data

def x_bound_to_time_bound(x_bound):
    span = (tweet_obs_x_end - tweet_obs_x_start)
    mult = tweet_obs_time_span / float(span)
    return (x_bound - tweet_obs_x_start) * mult

def compute_tweet_obs():
    rates = []
    spans = []
    bounds = []

    prev_bound = tweet_obs_x_start 
    bound = prev_bound
    res = tweet_obs_initial_resolution

    while bound < tweet_obs_x_end:
        bound += res
        bound = min(bound, tweet_obs_x_end)
        obs = tweet_observation_integral(prev_bound, bound)
        rates.append(obs)
        spans.append(res)
        bounds.append(x_bound_to_time_bound(bound))

        prev_bound = bound # Set current bound to new previous
        res *= tweet_obs_resolution_growth_factor # Increase the resolution by the growth factor

    normalize_tweet_obs(rates, spans)
    return rates, bounds

#################################################################
# Relevance lookup table generation
# Factors:
#   Entity preference class
#   X Tweet type (for ideological tweets, whether ideologies match)
#   X Original tweeter entity type
#################################################################

def load_relevance_function(content):
    exec('def __TEMP(): return ' + str(content))
    return __TEMP # A hack

def load_relevance_weights():
    weights = []
    for pref in pref_classes:
        # NOTE: *MUST* be in same order as TweetType in config_static.h!

        pref_weights = []
        for tweet_type in ["plain", "same_ideology", "plain", "humourous", "different_ideology"]:
        
            weight_set = []
            print(tweet_type, "=>", weight_set)
            retweet_rel = pref["tweet_transmission"][tweet_type]
            # Load all the functions based on the different entity types
            # Defaults to the 'else' node.
            for e in entities:
                name = e["name"]
                if name in retweet_rel:
                    func_str = retweet_rel[name]
                elif "all" in retweet_rel:
                    func_str = retweet_rel["all"]
                else:
                    func_str = retweet_rel["else"]
                func = load_relevance_function(func_str)
                weight_set.append(func())
            print(tweet_type, "=>", weight_set)
            pref_weights.append(weight_set)
    
        print("pref_weights", pref_weights)
        weights.append(pref_weights)
    return weights

#################################################################
# YAML emission
#################################################################

obs_function, obs_bin_bounds = compute_tweet_obs()

generated = {
    "obs_function" : obs_function,
    "obs_bin_bounds" : obs_bin_bounds,
    "tweet_react_table" : load_relevance_weights(),
    "regions" : preprocess_regions()
}

CONFIG["GENERATED"] = generated

for val in "max_time", "max_real_time", "max_entities", "initial_entities":
    if isinstance(CONFIG["analysis"][val], str):
        CONFIG["analysis"][val] = eval(CONFIG["analysis"][val])

yaml.dump(CONFIG, open(INPUT_FILE_NAME + "-generated", "w"))

print("INFILE.py -- Done generating rates")
