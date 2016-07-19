from hashkat_util import TestBase, hashkat_test, hashkat_dump_summary, hashkat_dump_stats

###############################################################################
## Hash tag retweet test
###############################################################################

# Test for Github Issue #110: 
#   Does using hashtags incorrectly prevent retweets in a bare case?
class HashtagsDontPreventRetweetSimple(TestBase):
    base_infile = "base_infiles/simple-base.yaml"
    def __init__(self):
        self.steps = 0
    # Configure the base configuration
    def infile_setup(self, yaml):
        yaml["analysis"]["use_hashtag_probability"] = 1.0
    def on_exit(self):
        stats = hashkat_dump_stats(self.state)["global_stats"]
        assert int(stats["n_tweets"]) > 0
        assert int(stats["n_retweets"]) > 0

# Reproducer for Github Issue #110: 
#   Does using hashtags incorrectly prevent retweets in the provided INFILE from https://github.com/hashkat/hashkat/issues/110?
class HashtagsDontPreventRetweetReproducer(TestBase):
    base_infile = "bug_infiles/gh110-retweet-hashtags.yaml"
    def __init__(self):
        self.steps = 0
    # Configure the base configuration
    def infile_setup(self, yaml):
        yaml["analysis"]["use_hashtag_probability"] = 1.0
    def on_retweet(self, id, id_tweet):
        print id, id_tweet
    def on_exit(self):
        stats = hashkat_dump_stats(self.state)["global_stats"]
        assert int(stats["n_tweets"]) > 0
        assert int(stats["n_retweets"]) > 0

hashkat_test(HashtagsDontPreventRetweetSimple())
hashkat_test(HashtagsDontPreventRetweetReproducer())
