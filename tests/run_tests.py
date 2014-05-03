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

MAX_ENTITIES = 1000
MAX_SIM_TIME = "minute/2" # Increase for longer test simulation duration
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
                use_unfollow, 
                use_followback,
                use_saving,
                use_hashtags,
                use_add_rate):

            self.follow_model = follow_model
            self.use_tweeting = use_tweeting 
            self.use_retweeting = use_retweeting 
            self.use_unfollow = use_unfollow 
            self.use_followback = use_followback
            self.use_saving = use_saving
            self.use_hashtags = use_hashtags
            self.use_add_rate = use_add_rate
            self.initial_entities = MAX_ENTITIES if not use_add_rate else 0

    def description(self):
        tweet = "+Tweets: on" if self.use_tweeting else "-Tweets: OFF"
        retweet = "+Retweets: on" if self.use_retweeting else "-Retweets: OFF"
        unfollow = "+Unfollows: on" if self.use_unfollow else "-Unfollows: OFF"
        followback = "+Followback: on" if self.use_followback else "-Retweets: OFF"
        saving = "+Saving: on" if self.use_saving else "-Saving: OFF"
        hashtags = "+Hashtags: on" if self.use_hashtags else "-Hashtags: OFF"
        add_rate = "+Add Rate: on" if self.use_add_rate else "-Add Rate: OFF"

        return "[Follow-model=%s, %s %s %s %s %s, Initial=%d, Max=%d]" % (
            follow_model, 
            tweet, retweet, followback, saving, add_rate,
            self.initial_entities, MAX_ENTITIES
        )

FOLLOW_MODELS = ['barabasi', 'random', 'preferential', 'entity', 'preferential-entity', 'hashtag', 'twitter']

configurations = []

for follow_model in FOLLOW_MODELS:
#    for use_tweeting in [False,True]: # TODO implement tweet toggle
#        for use_retweeting in [False,True]: # TODO implement retweet toggle
            for use_unfollow in [False,True]:
                for use_followback in [False,True]:
#                    for use_saving in [False,True]: #TODO implement saving tests
                        for use_hashtags in [False,True]:
                            for use_add_rate in [False,True]:
                                configurations.append(
                                    Configuration(
                                        follow_model, 
                                        True, #use_tweeting, 
                                        True, #use_retweeting, 
                                        use_unfollow,
                                        use_followback,
                                        False, #use_saving,
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

    if follow_model == 'barabasi':
        follow_model = 'random'

    use_hashtag_probability = 1 if C.use_hashtags else 0
    add_rate = 0.1 if C.use_add_rate else 0

    return {
      "analysis": {
        "initial_entities": C.initial_entities,
        "max_entities": MAX_ENTITIES,
        "max_time": MAX_SIM_TIME,
        "max_real_time": MAX_REAL_TIME,
        "enable_interactive_mode": False,
        "use_barabasi": (C.follow_model == 'barabasi'),
        "use_random_time_increment": RAND_TIME_INCR,
        "use_followback": C.use_followback,
        "follow_model": follow_model,
        "model_weights": {
          "random": 0.2,
          "preferential": 0.2,
          "entity": 0.2,
          "preferential_entity": 0.2,
          "hashtag": 0.2
        },
        "stage1_unfollow": C.use_unfollow,
        "unfollow_tweet_rate": 10,
        "use_hashtag_probability": use_hashtag_probability,
      },
      "rates": {
        "add": {
          "function": "constant",
          "value": add_rate
        }
      }
    }

#################################################################
# Run tests, and gather statistics.
#################################################################

def run_config_as_test(test_id, C):
    dict = config_to_yaml(C)
    text = yaml.dump(dict)
    f = open('TEST.yaml', 'w')
    f.write(text)
    f.close()

    code = call(["bash", "wrapper.sh", test_id, C.description(), 'TEST.yaml', RUN_SH_FLAGS])

num = 1
print("Running " + str(len(configurations)) + " tests.")
for config in configurations:
    run_config_as_test("Test_" + str(num), config)
    num += 1

