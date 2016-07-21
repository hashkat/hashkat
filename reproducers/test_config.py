from collections import defaultdict
from hashkat_util import TestBase, hashkat_test, hashkat_dump_state, hashkat_dump_summary,  hashkat_dump_stats

###############################################################################
## Is the max analysis steps variable honoured?
###############################################################################

class Max_analysis_steps_is_honoured(TestBase):
    base_infile = "base_infiles/simple-base.yaml"
    def __init__(self, EXPECTED_STEPS):
        self.EXPECTED_STEPS = EXPECTED_STEPS
        self.steps = 0
    # Configure the base configuration
    def infile_setup(self, yaml):
        yaml["analysis"]["max_analysis_steps"] = self.EXPECTED_STEPS
    def on_step_analysis(self):
        self.steps += 1
    def on_exit(self):
        stats = hashkat_dump_state(self.state)["stats"]
        # Test that we correctly exit at EXPECTED_STEPS, as specified by max_analysis_steps:
        assert stats["n_steps"] == self.steps
        assert stats["n_steps"] == self.EXPECTED_STEPS 

hashkat_test(Max_analysis_steps_is_honoured(EXPECTED_STEPS = 10))

###############################################################################
## Does changing the tweet_transmission rate increase the amount of tweets
## exponentially?
###############################################################################

class Tweet_transmission_roughly_linear_to_tweet_amount(TestBase):
    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    def __init__(self):
        self.retweets = defaultdict(int)
    # Configure the base configuration
    def infile_setup(self, yaml):
        self.args = ['--seed', str(self.iteration)]
        yaml["analysis"]["max_time"] = 1000000
        tweet_transmission = yaml["preference_classes"][0]["tweet_transmission"]
        for key in tweet_transmission:
            tweet_transmission[key]["all"] = (int(self.iteration / 5) + 1) / 30.0

    def on_exit(self):
        stats = hashkat_dump_stats(self.state)
        self.retweets[int(self.iteration / 5)] += int(stats["global_stats"]["n_retweets"])
    def on_exit_all(self):
        assert self.retweets[0] > 50
        assert self.retweets[1] > self.retweets[0] * 2 
        assert self.retweets[2] > self.retweets[1] * 2 
        # Test that we correctly exit at EXPECTED_STEPS, as specified by max_analysis_steps:
#        assert stats["n_steps"] == self.steps
#        assert stats["n_steps"] == self.EXPECTED_STEPS 

hashkat_test(Tweet_transmission_roughly_linear_to_tweet_amount(), n_simulations=15)
