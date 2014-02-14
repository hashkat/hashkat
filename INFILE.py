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
# Step 1) Parse the valid YAML input file that exists at INFILE.yaml
# We will add a 'generated' node to this, and emit it as INFILE-generated.yaml

CONFIG = yaml.load(open("INFILE.yaml", "r"))

entity_type_name_list = []
for entity in CONFIG["entities"]:
    entity_type_name_list.append(entity["name"])

def unpack_entity_type(raw_entity_type):
    return entity_type_name_list[raw_entity_type] # Converted to a name

print("Entity types are:", entity_type_name_list)

def define_function(name, content):
    exec('def ' + name + '(distance, humour, entity_type): return ' + str(content))

# Both functions are computed from a lookup table generated below.
# Note that the relevance factor is a many-dimensional function,
# whilst tweet_omega takes only time.

obs_pdf = CONFIG["tweet_observation"]

tweet_omega_half_life = obs_pdf["half_life"]
tweet_omega_initial_resolution = obs_pdf["initial_resolution"]
tweet_omega_final_rate = obs_pdf["final_rate"]

distance_bins = CONFIG["tweet_relevance"]["distance_bins"]

#######################
# Rate derivation

# If this is changed, the code must be updated accordingly:

# We define 'tweet_observation_pdf' to be a probability definition function that
# controls the rate at which a tweet is observed, as a function of time.
# It is scaled by the relevance factor.
def tweet_observation_pdf(t):
    hl = tweet_omega_half_life
    return exp(-t / hl * log(2))

# We compute all bins over a..b at (a+b)/2, ie the midpoint rule
def compute_tweet_omega():
    rates = []

    prev_bound = 0
    bound = 0
    res = tweet_omega_initial_resolution

    while True:
        bound += res
        # Compute the value of the function, using the function's midpoint
        omega = tweet_observation_pdf((bound + prev_bound) / 2)
        prev_bound = bound
        res *= 2.0 # Increase the resolution by double
        rates.append(omega)
        if omega <= tweet_omega_final_rate:
            break

    return rates

print compute_tweet_omega()

def compute_relevance_matrix():
    pass

#######################
# YAML emission

generated = {
    "omega_function" : compute_tweet_omega()
}

CONFIG["GENERATED"] = generated

repr = yaml.dump(CONFIG)
print(repr)
