from collections import defaultdict 
from hashkat_util import TestBase, hashkat_test, hashkat_dump_summary, hashkat_dump_stats

# 'factor': How many times bigger is 'a' than 'b'?
def reasonably_close(a,b, factor = 1):
    if b * factor < a / 2.0:
        return False
    if a < b * factor / 2.0:
        return False
    return True

###############################################################################
## Hash tag retweet test
###############################################################################
# Get counts of all unique strings that appear in the text.
def get_tweet_feature_counts(tweets):
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

# Reproducing github issue #109: 
class Retweets_should_generate_for_region0(TestBase):
    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    def __init__(self):
        self.tweets = {}
        self.retweets = {}
    def on_tweet(self, tweet):
        self.tweets[tweet["content_id"]] = tweet
    def on_retweet(self, tweet):
        self.retweets[tweet["content_id"]] = tweet
    # Configure the base configuration
    def infile_setup(self, yaml):
        self.args = ["--seed", str(self.iteration + 25)]
        yaml["analysis"]["max_time"] = 300000
        # Deactivate region 0
        yaml["regions"][0]["add_weight"] = 0
    def on_exit_all(self):
        tweet_counts = get_tweet_feature_counts(self.tweets)
        retweet_counts = get_tweet_feature_counts(self.retweets)
        assert tweet_counts["Region0"] == 0
        assert reasonably_close(tweet_counts["LANG_ENGLISH"], tweet_counts["LANG_FRENCH"])
        assert retweet_counts["Region1"] > 0

hashkat_test(Retweets_should_generate_for_region0(), n_simulations=1)

# Investigating github issue #110: 
#   Does using hashtags incorrectly prevent retweets in a bare case?
class Hashtags_should_not_prevent_retweets(TestBase):
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

hashkat_test(Hashtags_should_not_prevent_retweets(), n_simulations=1)

# Reproducing github issues #109 & #110 et al.: 
#   Are retweeted tweets not a representative sample for language, region, and hashtaggedness? 
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
    def test_tweet_distribution(self, tweets):
        counts = get_tweet_feature_counts(tweets)
        assert reasonably_close(counts["LANG_ENGLISH"], counts["LANG_FRENCH"], 4)
        assert reasonably_close(counts["Region0"], counts["Region1"])
    def on_exit_all(self):
        print "Testing distribution of all tweets:"
        self.test_tweet_distribution(self.tweets)
        print "Testing distribution of retweeted tweets:"
        self.test_tweet_distribution(self.retweets)

hashkat_test(Tweets_generate_with_expected_attribute_distribution(), n_simulations=1)
