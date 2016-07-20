from collections import defaultdict 
from hashkat_util import TestBase, hashkat_test, hashkat_dump_summary, hashkat_dump_stats

###############################################################################
## Hash tag retweet test
###############################################################################

# Investigatin github Issue #110: 
#   Does using hashtags incorrectly prevent retweets in a bare case?
class Hashtags_dont_prevent_retweet_simple(TestBase):
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

# Reproducing github Issues #109 & #110 et al.: 
#   Are retweeted tweets not a representative sample? 
class Tweets_generate_with_expected_attribute_distribution(TestBase):
    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    def __init__(self):
        self.tweets = {}
        self.retweets = {}
    # Configure the base configuration
    def infile_setup(self, yaml):
        self.args = ["--seed", str(self.iteration + 25)]
        yaml["analysis"]["max_time"] = 300000
    def on_tweet(self, tweet):
        self.tweets[tweet["content_id"]] = tweet
    def on_retweet(self, tweet):
        self.retweets[tweet["content_id"]] = tweet
    # Get counts of all unique strings that appear in the text.
    def get_counts(self, tweets):
        #print "Ran " + str(self.iteration + 1) + " tests."
        counts = defaultdict(int)
        def add_count(t): 
            counts[t] += 1
        for k in tweets:
            v = tweets[k]
            for key in 'author_region', 'content_type', 'language':
                add_count(v[key])
            if v['has_hashtag']:
                add_count("with_hashtag")
            else:
                add_count("without_hashtag")
        return counts
    def test_tweet_distribution(self, tweets):
        counts = self.get_counts(tweets)
        print counts
        # 'factor': How many times bigger is 'a' than 'b'?
        def reasonably_close(a,b, factor = 1):
            if counts[b] * factor < counts[a] / 4.0:
                return False
            if counts[a] < counts[b] * factor / 4.0:
                return False
            return True
        assert reasonably_close("LANG_ENGLISH", "LANG_FRENCH", 4)
        assert reasonably_close("Region0", "Region1")
        counts = self.get_counts(self.retweets)
    def on_exit_all(self):
        print "Testing distribution of all tweets:"
        self.test_tweet_distribution(self.tweets)
        print "Testing distribution of retweeted tweets:"
        self.test_tweet_distribution(self.retweets)

hashkat_test(Hashtags_dont_prevent_retweet_simple())
hashkat_test(Tweets_generate_with_expected_attribute_distribution(), 2)
