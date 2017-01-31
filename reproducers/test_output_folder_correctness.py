import unittest
from collections import defaultdict 
from hashkat_util import HashkatTestCase, hashkat_dump_state, hashkat_dump_summary, hashkat_dump_stats, hashkat_dump_agents, KnownFailingTest

# Which options should we turn on to get full output?
RELEVANT_OUTPUT_OPTIONS = [
    "agent_stats",
    "degree_distributions",
    "tweet_analysis",
    "retweet_visualization",
    "main_statistics",
    "degree_distribution_by_follow_model",
    "region_connection_matrix",
    "categories_distro",
    "most_popular_tweet_content",
    "tweet_info"
]

class Tweet_distro_dat_normalized(HashkatTestCase):
    '''Investigating tweet distro normalization in output folder (issue #32): 
         Does tweet_distro.dat correctly normalize data?
         Does retweet_distro.dat correctly normalize data?'''

    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    use_full_checks = False
    n_runs = 1
    # Configure the base configuration
    def on_start(self, yaml):
        yaml["analysis"]["max_time"] = 300000
        for option in RELEVANT_OUTPUT_OPTIONS:
            yaml["output"][option] = True
    # on_exit() cannot be used, as it is invoked before the output folders are created.
    def on_exit_all(self):
        for dat_path in 'output/tweets_distro.dat', 'output/retweets_distro.dat':
            sum = 0
            # Parse the dat file, assuming that the relevant number is always 
            # the second whitespace-delimited component:
            with open(dat_path) as dat:
                for line in dat.readlines():
                    # Reject comment lines:
                    if '#' in line: continue
                    parts = line.split()
                    # Reject lines without sufficient components:
                    if len(parts) < 2: continue
                    num = float(parts[1])
                    sum += num
            # Check that the sum is close enough to 1
            #print "Sum for " + dat_path + " is " + str(sum)
            self.assertTrue(0.95 < sum < 1.05, 
                "The values in " + dat_path + " should be normalized to sum to 1. Was: " + str(sum))

class Main_stats_dat_correctly_records_barabasi_follow_attempts(HashkatTestCase, unittest.TestCase):
    '''Does main_stats.dat correctly record follow attempts when using use_barabasi? (issue #21)'''

    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    use_full_checks = False
    n_runs = 1
    # Configure the base configuration
    def on_start(self, yaml):
        yaml["analysis"]["max_time"] = 300
        for option in RELEVANT_OUTPUT_OPTIONS:
            yaml["output"][option] = True
        yaml["analysis"]["use_barabasi"] = True
    # on_exit() cannot be used, as it is invoked before the output folders are created.
    def on_exit_all(self):
        # Check that the total follow attempts was not less than the total follows.
        with open('output/main_stats.dat') as dat:
            for line in dat.readlines():
                if 'Total follows' in line:
                    total_follows = int(line.split()[-1])
                elif 'Total follow attempts' in line:
                    total_follow_attempts = int(line.split()[-1])
        print "Total follows: ", total_follows
        print "Total follow attempts: ", total_follow_attempts
        self.assertTrue(total_follow_attempts >= total_follows, 
            "Should not have less follow attempts than follows recorded when using 'use_barabasi: true'!")

class dd_by_follow_model_dat_correctly_handles_twitter_follow_model(HashkatTestCase, unittest.TestCase):
    '''Does dd_by_follow_model.dat incorrectly record the compound twitter follow model as a single follow model? (issue #44)'''

    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    use_full_checks = True # Make sure to try to trip assertions in the code.
    n_runs = 1
    # Configure the base configuration
    def on_start(self, yaml):
        yaml["analysis"]["max_time"] = 300000
        for option in RELEVANT_OUTPUT_OPTIONS:
            yaml["output"][option] = True
        # Enable the twitter follow model, with equal probability between the various options:
        yaml["analysis"]["follow_model"] = 'twitter'
        yaml["analysis"]["model_weights"] = {'random': 1.0, 'twitter_suggest': 1.0, 'agent': 1.0, 'preferential_agent': 1.0, 'hashtag': 1.0}
    # on_exit() cannot be used, as it is invoked before the output folders are created.
    def on_exit_all(self):
        # Check that the total follow attempts was not less than the total follows.
        with open('output/dd_by_follow_model.dat') as dat:
            lines = filter(lambda x: len(x) > 0 and x[0].isdigit(), dat.readlines())
        for line in lines[:5]:
            # Tricky to test, we test for a problematic string copied from dd_by_follow_model.dat in its first few lines:
            self.assertTrue(
                "0	-inf	0	-inf	0	-inf	0	-inf" not in line,
                "dd_by_follow_model.dat should have output for all follow models.")

if __name__ == "__main__":
    unittest.main()
