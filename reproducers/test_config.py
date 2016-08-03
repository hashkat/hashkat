import unittest
from collections import defaultdict
from hashkat_util import HashkatTestCase, hashkat_dump_state, hashkat_dump_summary,  hashkat_dump_stats

class Max_analysis_steps_is_honoured(HashkatTestCase, unittest.TestCase):
    '''Is the max analysis steps variable honoured?'''

    base_infile = "base_infiles/simple-base.yaml"
    n_runs = 1
    use_full_checks = True
    # Internal test config:
    EXPECTED_STEPS = 10
    def on_start_all(self):
        self.steps = 0
    # Configure the base configuration
    def on_start(self, yaml):
        yaml["analysis"]["max_analysis_steps"] = self.EXPECTED_STEPS
    def on_step_analysis(self):
        self.steps += 1
    def on_exit(self):
        stats = hashkat_dump_state(self.state)["stats"]
        # Test that we correctly exit at EXPECTED_STEPS, as specified by max_analysis_steps:
        self.assertEqual(stats["n_steps"], self.steps)
        self.assertEqual(stats["n_steps"], self.EXPECTED_STEPS )

class Tweet_transmission_is_exponential(HashkatTestCase, unittest.TestCase):
    '''Does changing the tweet_transmission rate increase the amount of tweets exponentially?'''

    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    n_runs = 15
    use_full_checks = False
    def on_start_all(self):
        self.retweets = defaultdict(int)
    # Configure the base configuration
    def on_start(self, yaml):
        self.args = ['--seed', str(self.runs)]
        yaml["analysis"]["max_time"] = 1000000
        tweet_transmission = yaml["preference_classes"][0]["tweet_transmission"]
        for key in tweet_transmission:
            tweet_transmission[key]["all"] = (int(self.runs / 5) + 1) / 30.0

    def on_exit(self):
        stats = hashkat_dump_stats(self.state)
        self.retweets[int(self.runs / 5)] += int(stats["global_stats"]["n_retweets"])
    def on_exit_all(self):
        self.assertTrue(self.retweets[0] > 50)
        self.assertTrue(self.retweets[1] > self.retweets[0] * 2 )
        self.assertTrue(self.retweets[2] > self.retweets[1] * 2 )
        # Test that we correctly exit at EXPECTED_STEPS, as specified by max_analysis_steps:
#        self.assertTrue(stats["n_steps"] == self.steps)
#        self.assertTrue(stats["n_steps"] == self.EXPECTED_STEPS )

