#!/usr/bin/python

import yaml

from pprint import pprint
from math import *

#################################################################
# Generates INFILE parameters that are tricky to alter by hand
# By creating value tables with a Python script, we are able to
# simplify the logic in the C++, not worrying about the various
# functions a user might want to supply.
#################################################################

######################
# Load the relevant pieces of the config.
# We will add a 'generated' node to this, and emit it as INFILE-generated.yaml

CONFIG = yaml.load(open("INFILE.yaml", "r"))

entities = CONFIG["entities"]

# Both functions are computed from a lookup table generated below.
# Note that the relevance factor is a many-dimensional function,
# whilst tweet_obs takes only time.

obs_pdf = CONFIG["tweet_observation"]

tweet_obs_half_life = obs_pdf["half_life"]
tweet_obs_initial_resolution = obs_pdf["initial_resolution"]
tweet_obs_final_rate = obs_pdf["final_rate"]

tweet_rel = CONFIG["tweet_relevance"]
distance_bins = tweet_rel["distance_bins"]
humour_bins = tweet_rel["humour_bins"]
location_bins = tweet_rel["location_bins"]

pref_classes = tweet_rel["preference_classes"]

def load_relevance_function(content):
    exec('def __TEMP(distance, humour, entity_type): return ' + str(content))
    return __TEMP # A hack

def load_relevance_functions():
    funcs = []
    for p in pref_classes:
        func_set = {}
        # Load all the functions based on the different entity types
        # Defaults to the 'else' node.
        for e in entities:
            name = e["name"]
            retweet_rel = p["retweet_relevance"]
            if name in retweet_rel:
                func_str = retweet_rel[name]
            else:
                func_str = retweet_rel["else"]
            func = load_relevance_function(func_str)
            func_set[name] = func
        funcs.append(func_set)
    return funcs

profile_funcs = load_relevance_functions()

#######################
# Rate derivation

# If this is changed, the code must be updated accordingly:

# We define 'tweet_observation_pdf' to be a probability definition function that
# controls the rate at which a tweet is observed, as a function of time.
# It is scaled by the relevance factor.
def tweet_observation_pdf(t):
    hl = tweet_obs_half_life
    val = exp(-t / hl * log(2))
    #print(str(t) + ' ' + str(val)) #Uncomment for simple, plottable data
    return val

# We compute all bins over a..b at (a+b)/2, ie the midpoint rule
def compute_tweet_obs():
    rates = []

    prev_bound = 0
    bound = 0
    res = tweet_obs_initial_resolution

    while True:
        bound += res
        # Compute the value of the function, using the function's midpoint
        obs = tweet_observation_pdf((bound + prev_bound) / 2)
        prev_bound = bound
        res *= 2.0 # Increase the resolution by double
        rates.append(obs)
        if obs <= tweet_obs_final_rate:
            break

    return rates

#######################
# Relevance lookup table generation

def make_object(dict):
    class Struct: # Helper
        def __init__(self, **entries):
            self.__dict__.update(entries)
    obj = Struct(**dict)
    return obj

def relevance_rate_vector(entity_type, humour):
    results = []
    for func_set in profile_funcs:
        f = func_set[entity_type.name]
        res = f(0, humour, entity_type) # TODO 0 -> distance
        results.append(res)
    return results

def relevance_humour_component(entity_type):
    results = []
    for i in range(humour_bins):
        res = relevance_rate_vector(entity_type, i / float(humour_bins))
        results.append(res)
    return results

def relevance_entity_type_component():
    results = []
    n_types = len(entities)

    for i in range(n_types):
        entity_type = make_object(entities[i])
        res = relevance_humour_component(entity_type)
        results.append(res)
    return results

def compute_relevance_table(): # N-dimensional array
    return relevance_entity_type_component()

#######################
# YAML emission

generated = {
    "obs_function" : compute_tweet_obs(),
    "rel_function" : compute_relevance_table()
}

CONFIG["GENERATED"] = generated

yaml.dump(CONFIG, open("INFILE-generated.yaml", "w"))
