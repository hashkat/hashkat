#!/usr/bin/env python
 
#################################################################
# Generates INFILE parameters that are tricky to alter by hand
# By creating value tables with a Python script, we are able to
# simplify the logic in the C++, not worrying about the various
# functions a user might want to supply.
#################################################################

def expression_expand(expression):
    # Place convenience constants for INFILE.yaml here
    # Convenience rates, in minutes
    minute = 1
    hour = minute * 60
    day = 24 * hour
    year = 365 * day
    # Very large number representable comfortably as signed 64bit integer:
    unlimited = 2L**53 

    # Evaluate the expression with the above constants
    return eval(expression, {"__builtins": None}, {
        "minute": minute, "hour": hour, "day": day, "year": year, 
        "unlimited": unlimited
    })

#################################################################
# CEASE PLACING CONVENIENCE CONSTANTS.
#################################################################

import yaml
import sys
import os 

from pprint import pprint
from math import *

def get_var_arg(test, default_val):
    for i in range(len(sys.argv) - 1): 
        if sys.argv[i] == test:
            return sys.argv[i+1]
    return default_val

# This environment variable needs can set by the user ahead of time,
# but is set if not present to the current directory by run.sh.
# Explicitly don't resolve os.environ['HASHKAT'] if --input and --base-input arguments are present.
INPUT_FILE_NAME = get_var_arg("--input", None)
if not INPUT_FILE_NAME:
    INPUT_FILE_NAME = "./INFILE.yaml"
DEFAULT_FILE_NAME = get_var_arg("--base-input", None)
if not DEFAULT_FILE_NAME:
    DEFAULT_FILE_NAME = os.environ['HASHKAT'] + "/DEFAULT.yaml"

print("hashkat_pre.py -- Loading defaults from " + DEFAULT_FILE_NAME)
print("hashkat_pre.py -- Generating rates for " + INPUT_FILE_NAME)

#################################################################
# Load the relevant pieces of the config.
# We will add a 'generated' node to this, and emit it as INFILE.yaml-generated
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

    default_check(src, dst, "preference_classes")
    default_check(src, dst, "agents")
    
merge_config(DEFAULT_CONFIG, CONFIG)

agents = CONFIG["agents"]

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
    tweet_obs_time_span = expression_expand(tweet_obs_time_span)

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
    "French+English" : 1,
    "French" : 2,
    "Spanish" : 3   
}

lang_n = 4
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

def compute_simpson(f, a, b):
    """Approximates the definite integral of f from a to b by the
    composite Simpson's rule, using n subintervals (with n even)"""
 
    n = 1000 # This won't run much, just overkill it
 
    h = (b - a) / float(n)
    s = f(a) + f(b)
 
    for i in range(1, n, 2):
        s += 4 * f(a + i * h)
    for i in range(2, n-1, 2):
        s += 2 * f(a + i * h)
 
    return s * h / 3

def tweet_observation_integral(x1, x2):
    val = compute_simpson(tweet_obs_density_function, x1, x2)
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
    full_int = tweet_observation_integral(tweet_obs_x_start, tweet_obs_x_end)

    while bound < tweet_obs_x_end:
        bound += res
        bound = min(bound, tweet_obs_x_end)
        obs = tweet_observation_integral(prev_bound, bound)
        #obs /= full_int
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
#   Agent preference class
#   X Tweet type (for ideological tweets, whether ideologies match)
#   X Original tweeter agent type
#################################################################

def load_relevance_function(content):
    exec('def __TEMP(): return ' + str(content))
    return __TEMP # A hack

def load_relevance_weights():
    weights = []
    for pref in pref_classes:
        pref_weights = []
        for tweet_type in ["plain", "same_ideology", "plain", "humorous", "different_ideology"]:
        
            weight_set = []
            #print(tweet_type, "=>", weight_set)
            retweet_rel = pref["tweet_transmission"][tweet_type]
            # Load all the functions based on the different agent types
            # Defaults to the 'else' node.
            for e in agents:
                name = e["name"]
                if name in retweet_rel:
                    func_str = retweet_rel[name]
                elif "all" in retweet_rel:
                    func_str = retweet_rel["all"]
                else:
                    func_str = retweet_rel["else"]
                func = load_relevance_function(func_str)
                weight_set.append(func())
            #print(tweet_type, "=>", weight_set)
            pref_weights.append(weight_set)
    
        #print("pref_weights", pref_weights)
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

#f = open('dens_func.dat', 'w')
counter = 0
suma = 0
n_retweets = 0
for elem in obs_function:
    #f.write("%f\t%f\n" % (obs_bin_bounds[counter], elem))
    if counter == 0:
        n_retweets += obs_bin_bounds[counter] * elem
    else:
        n_retweets += (obs_bin_bounds[counter] - obs_bin_bounds[counter - 1]) * elem
    counter += 1
    suma += elem
#print "SUM =", suma
#print "N_RETWEETS =", n_retweets

for val in "max_analysis_steps", "max_time", "max_real_time", "max_agents", "initial_agents":
    if isinstance(CONFIG["analysis"][val], str):
        CONFIG["analysis"][val] = expression_expand(CONFIG["analysis"][val])

with open(INPUT_FILE_NAME + "-generated", "w") as f:
    yaml.dump(CONFIG, f)

try:
    os.mkdir('output') 
    print("hashkat_pre.py -- Created an output directory")
    os.system('cp ./INFILE.yaml ./output/INFILE.yaml')
    print("hashkat_pre.py -- Copied INFILE.yaml to output directory")
except OSError:
    print "hashkat_pre.py -- An output directory already exists, leaving it intact"

print("hashkat_pre.py -- Done generating rates")
