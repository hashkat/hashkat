#!/usr/bin/env python

#################################################################
# Create configurations programmatically.
# This allows us to test features in combination, 
# as well as in isolation.
#################################################################

import yaml
import sys

from subprocess import call

#################################################################
# 'Constants' shared by all test runs
#################################################################

MAX_AGENTS = 10000
MAX_ANALYSIS_STEPS = 10000# Increase for longer test simulation duration
ANALYSIS_STEP_SAVE_POINT = MAX_ANALYSIS_STEPS * 3/4 # For saving tests, when should we save to disk?
MAX_REAL_TIME = "hour" # Increase for longer test time allowance
RAND_TIME_INCR = False
RUN_SH_FLAGS = "--debug-std --stacktrace"

#################################################################
# Create configurations programmatically.
#
# This allows us to test features in combination, 
# as well as in isolation.
#################################################################

class Configuration:

    def __init__(self, 
                follow_model, 
                use_tweeting, 
                use_retweeting, 
                use_follow, 
                use_tweet_follow, 
                use_unfollow, 
                use_followback,
                use_hashtags,
                use_add_rate):

            self.follow_model = follow_model
            self.use_tweeting = use_tweeting 
            self.use_retweeting = use_retweeting 
            self.use_follow = use_follow
            self.use_tweet_follow = use_tweet_follow 
            self.use_unfollow = use_unfollow 
            self.use_followback = use_followback
            self.use_hashtags = use_hashtags
            self.use_add_rate = use_add_rate
            self.initial_agents = MAX_AGENTS if not use_add_rate else 0

    def description(self):
        tweet = "+Tweets: on" if self.use_tweeting else "-Tweets: OFF"
        retweet = "+Retweets: on" if self.use_retweeting else "-Retweets: OFF"
        unfollow = "+Unfollows: on" if self.use_unfollow else "-Unfollows: OFF"
        followback = "+Followback: on" if self.use_followback else "-Retweets: OFF"
        hashtags = "+Hashtags: on" if self.use_hashtags else "-Hashtags: OFF"
        add_rate = "+Add Rate: on" if self.use_add_rate else "-Add Rate: OFF"

        return "[Follow-model=%s, %s %s %s %s, Initial=%d, Max=%d]" % (
            self.follow_model, 
            tweet, retweet, followback, add_rate,
            self.initial_agents, MAX_AGENTS
        )

FOLLOW_MODELS = ['barabasi', 'random', 'preferential', 'agent', 'preferential-agent', 'hashtag', 'twitter', 'none']

configurations = []

# Saving is implemented by saving numbers into 'observables'
# and comparing the corresponding numbers when implementing saving
for follow_model in FOLLOW_MODELS:
    for use_tweeting in [True, False]: 
        for use_retweeting in [False,True]: 
            for use_tweet_follow in [False,True]: 
                for use_follow in [False,True]: 
                    for use_unfollow in [False,True]:
                        for use_followback in [False,True]:
                                for use_hashtags in [False,True]:
                                    for use_add_rate in [False,True]:
                                        configurations.append(
                                            Configuration(
                                                follow_model, 
                                                use_tweeting, 
                                                use_retweeting,
                                                use_follow,
                                                use_tweet_follow,
                                                use_unfollow,
                                                use_followback,
                                                use_hashtags,
                                                use_add_rate
                                            )
                                        )

#for config in configurations:
#    print(config.description())

#################################################################
# Convert configurations to tests.
#################################################################

def config_to_yaml(C):
    follow_model = C.follow_model

    if follow_model == 'barabasi' or follow_model == 'none':
        follow_model = 'random'

    use_hashtag_probability = 1 if C.use_hashtags else 0
    add_rate = 0.1 if C.use_add_rate else 0
    follow_weight = 0 if C.follow_model == 'none' else 5
    retweet_rate = 0 if C.use_retweeting else 0
    tweet_follow_probability = 0.5 if C.use_tweet_follow else 0
    followback_probability = 0.44 if C.use_followback else 0

    return {
      "analysis": {
        "initial_agents": C.initial_agents,
        "max_agents": MAX_AGENTS,
        "max_time": "unlimited",
        "max_real_time": MAX_REAL_TIME,
        "max_analysis_steps": MAX_ANALYSIS_STEPS,
        "enable_interactive_mode": False,
        "enable_lua_hooks": True,
        # The TEST_INTERACT.lua file gives extra control over the test behaviour.
        # Specifically, the file allows for Lua to save and load the network
        "lua_script": "tests/TEST_INTERACT.lua",
        "use_barabasi": (C.follow_model == 'barabasi'),
        "use_random_time_increment": RAND_TIME_INCR,
        "use_followback": C.use_followback,
        "follow_model": follow_model,
        "model_weights": {
          "random": 0.2,
          "preferential": 0.2,
          "agent": 0.2,
          "preferential_agent": 0.2,
          "twitter_suggest": 0.2,
          "hashtag": 0.2 if C.use_hashtags else 0
        },
        "stage1_unfollow": C.use_unfollow,
        "unfollow_tweet_rate": 0.1,
        "use_hashtag_probability": use_hashtag_probability,
      },
      "rates": {
        "add": {
          "function": "constant",
          "value": add_rate
        }
      },
      "preference_classes": [
        {
          "name": "StandardPref",
          "tweet_transmission": {
            "plain": {
              "Standard": retweet_rate,
              "Celebrity": retweet_rate,
              "else": retweet_rate
            },
            "different_ideology": {
              "Standard": retweet_rate,
              "Celebrity": retweet_rate,
              "else": retweet_rate
            },
            "same_ideology": {
              "Standard": retweet_rate,
              "Celebrity": retweet_rate,
              "else": retweet_rate
            },
            "humorous": {
              "Standard": retweet_rate, 
              "Celebrity": retweet_rate,
              "else": retweet_rate
            }
          },
          "follow_reaction_prob": tweet_follow_probability 
        },
       ],
       "agents": [
        {
          "name": "Standard",
          "weights": {
            "add": 80,
            "follow": 5,
            "tweet_type": {
              "ideological": 1,
              "plain": 1,
              "musical": 1,
              "humorous": 1
            }
          },
          "followback_probability": followback_probability,
          "hashtag_follow_options": {
            "care_about_region": C.use_hashtags,
            "care_about_ideology": C.use_hashtags
          },
          "rates": {
            "follow": {
              "function": "constant",
              "value": 0.1 if C.use_follow else 0
            },
            "tweet": {
              "function": "constant",
              "value": 0.01 if C.use_tweeting else 0
            }
          }
        }
      ]
    }

#################################################################
# Run tests, and gather statistics.
#################################################################

# Clear old logs
call(["bash", "clean_tests.sh"])

def run_config_as_test(test_id, C):
    dict = config_to_yaml(C)
    text = yaml.dump(dict)
    f = open('TEST.yaml', 'w')
    f.write(text)
    f.close()

    code = call(["bash", "wrapper.sh", test_id, C.description(), 'TEST.yaml', RUN_SH_FLAGS, str(ANALYSIS_STEP_SAVE_POINT)])

num = 1
print("Running " + str(len(configurations)) + " tests.")
for config in configurations:
    run_config_as_test("Test_" + str(num), config)
    num += 1
