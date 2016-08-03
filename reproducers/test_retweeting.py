import unittest
from collections import defaultdict 
from hashkat_util import HashkatTestCase, hashkat_dump_summary, hashkat_dump_stats

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
    #print "Ran " + str(self.runs + 1) + " tests."
    counts = defaultdict(int)
    def add_count(t): 
        counts[t] += 1
    for k in tweets:
        v = tweets[k]
        for key in 'author_region', 'content_type', 'language', 'author_agent_type':
            add_count(v[key])
        if v['has_hashtag']:
            add_count("with_hashtag")
        else:
            add_count("without_hashtag")
    return counts

class Retweets_should_not_only_occur_from_region0(HashkatTestCase, unittest.TestCase):
    ''' Reproducing github issue #109, which noted correctly that retweets had only occurred from region0.
        Asserts that retweets from other regions occur.'''

    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    n_runs = 1
    def on_start_all(self):
        self.tweets = {}
        self.retweets = {}
    def on_tweet(self, tweet):
        self.tweets[tweet["content_id"]] = tweet
    def on_retweet(self, tweet):
        self.retweets[tweet["content_id"]] = tweet
    # Configure the base configuration
    def on_start(self, yaml):
        self.args = ["--seed", str(self.runs + 25)]
        yaml["analysis"]["max_time"] = 300000
        # Deactivate region 0
        yaml["regions"][0]["add_weight"] = 0
    def on_exit_all(self):
        tweet_counts = get_tweet_feature_counts(self.tweets)
        retweet_counts = get_tweet_feature_counts(self.retweets)
        self.assertTrue(tweet_counts["Region0"] == 0,
            "Region0 should not retweet in this simulation.")
        self.assertTrue(reasonably_close(tweet_counts["LANG_ENGLISH"], tweet_counts["LANG_FRENCH"]),
            "English and French tweets should be within 50% of each other.")
        self.assertTrue(retweet_counts["Region1"] > 0,
            "Retweets should occur from Region1.")

class Hashtags_should_not_prevent_retweets(HashkatTestCase, unittest.TestCase):
    '''Investigating github issue #110: 
       Does using hashtags incorrectly prevent retweets in a bare case?'''

    base_infile = "base_infiles/simple-base.yaml"
    n_runs = 1
    def on_start_all(self):
        self.steps = 0
    # Configure the base configuration
    def on_start(self, yaml):
        yaml["analysis"]["use_hashtag_probability"] = 1.0
    def on_exit(self):
        stats = hashkat_dump_stats(self.state)["global_stats"]
        self.assertTrue(int(stats["n_tweets"]) > 0)
        self.assertTrue(int(stats["n_retweets"]) > 0)

class Mean_retweet_time_should_not_affect_retweeting_directly_from_author(HashkatTestCase, unittest.TestCase):
    '''Generation 1 retweets should not be affected in volume by mean retweet time. 
       Investigating github issue #43.'''

    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    n_runs = 1
    def on_start_all(self):
        self.tweets = {}
        self.retweets = {}
    # Configure the base configuration
    def on_start(self, yaml):
        self.args = ["--seed", str(self.runs + 25)]
        yaml["analysis"]["max_time"] = 300000
    def on_tweet(self, tweet):
        self.tweets[tweet["content_id"]] = tweet
    def on_retweet(self, tweet):
        self.retweets[tweet["content_id"]] = tweet
    def check_tweet_distribution(self, tweets):
        counts = get_tweet_feature_counts(tweets)
        self.assertTrue(reasonably_close(counts["LANG_ENGLISH"], counts["LANG_FRENCH"], 4))
        self.assertTrue(reasonably_close(counts["Region0"], counts["Region1"]))
    def on_exit_all(self):
        print "Testing distribution of all tweets:"
        self.check_tweet_distribution(self.tweets)
        print "Testing distribution of retweeted tweets:"
        self.check_tweet_distribution(self.retweets)

class Tweets_dont_generate_for_agent_tweet_rate_0(HashkatTestCase, unittest.TestCase):
    '''Reproducing github issue #54:
        Do tweets incorrectly generate for 'silenced' agent types (i.e., tweet rate 0)?'''

    base_infile = "base_infiles/silent-agent-type.yaml"
    n_runs = 1
    def on_start_all(self):
        self.tweets = {}
    # Configure the base configuration
    def on_start(self, yaml):
        yaml["analysis"]["max_time"] = 30000
    def on_tweet(self, tweet):
        self.tweets[tweet["content_id"]] = tweet
    def on_exit_all(self):
        counts = get_tweet_feature_counts(self.tweets)
        self.assertTrue(counts["DoesTweet"] > 0, 
            "Agent type with tweet rate > 0 should tweet.")
        self.assertTrue(counts["DoesntTweet"] == 0, 
            "Agent type with tweet rate == 0 should NOT tweet. Had " + str(counts["DoesntTweet"]) + " tweets")

if __name__ == "__main__":
    unittest.main()
