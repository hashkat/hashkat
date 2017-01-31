import unittest
from collections import defaultdict 
from hashkat_util import HashkatTestCase, hashkat_dump_summary, hashkat_dump_stats, hashkat_dump_agents, KnownFailingTest
from stat_util import stdev

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
        yaml["analysis"]["max_time"] = 60000
        # Deactivate region 0
        yaml["regions"][0]["add_weight"] = 0
    def on_exit_all(self):
        print hashkat_dump_stats(self.state)["global_stats"]
        tweet_counts = get_tweet_feature_counts(self.tweets)
        retweet_counts = get_tweet_feature_counts(self.retweets)
        self.assertTrue(tweet_counts["Region0"] == 0,
            "Region0 should not retweet in this simulation.")
        self.assertTrue(reasonably_close(tweet_counts["LANG_ENGLISH"], tweet_counts["LANG_FRENCH"]),
            "English and French tweets should be within 50% of each other.")
        self.assertTrue(retweet_counts["Region1"] > 0,
            "Retweets should occur from Region1.")

class Network_with_3_agents_should_create_2x_retweets(HashkatTestCase, unittest.TestCase):
    '''Reproducing github issue #155: 
       Does a network with 3 agents behave correctly?
       Bugs reproduced:
           - Retweets can fail to occur reliably if time step too large due to small rate block.
           - Tweets in a 3 agent network should not have a retweet distance (number of retweets since origin) of more than 2.'''

    base_infile = "base_infiles/simple-base.yaml"
    n_runs = 1
    def on_start_all(self):
        self.retweets = []
        self.steps = 0
    # Configure the base configuration
    def on_start(self, yaml):
        yaml["analysis"]["max_time"] = 3000000
        yaml["analysis"]["use_hashtag_probability"] = 0
        yaml["analysis"]["max_agents"] = yaml["analysis"]["initial_agents"] = 3
        tweet_transmission = yaml["preference_classes"][0]["tweet_transmission"]
        for key in tweet_transmission:
            tweet_transmission[key] = {"all": 1.0}
    def on_retweet(self, tweet):
        self.retweets.append(tweet)
    def on_exit(self):
        for agent in hashkat_dump_agents(self.state, dump_follow_sets=True):
            print '<<' + str(agent["id"]) + '>>'
            print "Subs: ", agent["following_set"]
            print "Fans: ", agent["follower_set"]
            print "Region: ", agent["region"] 
            print "Ideology: ", agent["ideology"] 
            print "Language: ", agent["language"] 
            #print json.dumps(agent, indent=4, sort_keys=True)
        stats = hashkat_dump_stats(self.state)["global_stats"]
        self.assertTrue(int(stats["n_followers"]) == 6,
            "Every agent should have 2 followers, totalling 6.")
        print "Tweets: ", int(stats["n_tweets"]) 
        print "Retweets: ", int(stats["n_retweets"]) 
        max_hop = max(int(rt["retweets_since_origin"]) for rt in self.retweets)
        for i in range(1, max_hop+1):
            print "Retweets at follower distance " + str(i) + ": ", sum(1 for rt in self.retweets if int(rt["retweets_since_origin"]) == i)
        self.assertTrue(max_hop == 2,
            "Every tweet should be retweeted at most 2 times from origin! Had max follower distance " + str(max_hop) + ", should be 2")

        self.assertTrue(int(stats["n_retweets"]) > 0,
            "Should have retweets!")
        self.assertTrue(int(stats["n_retweets"]) <= int(stats["n_tweets"] * 2),
            "Should not have more than twice as many retweets as tweets!")

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

class Retweets_generate_with_all_french(HashkatTestCase, unittest.TestCase):
    '''Do retweets generate if a network is only french speakers?'''
    base_infile = "base_infiles/simple-base.yaml"
    use_full_checks = False
    n_runs = 1
    # Configure the base configuration
    def on_start(self, yaml):
        self.tweets = {}
        self.retweets = {}
        yaml["analysis"]["max_time"] = 30000
        for region in yaml["regions"]:
            region["language_weights"]["English"] = 0
            region["language_weights"]["French"] = 100
    def on_tweet(self, tweet):
        self.tweets[tweet["content_id"]] = tweet
    def on_retweet(self, tweet):
        self.retweets[tweet["content_id"]] = tweet
    def check_tweet_distribution(self, tweets):
        counts = get_tweet_feature_counts(tweets)
        print counts["LANG_FRENCH"]
        self.assertTrue(counts["LANG_FRENCH"] > 0, 
            "Should have french tweets/retweets!")
    def on_exit_all(self):
        print "Testing distribution of all tweets:"
        self.check_tweet_distribution(self.tweets)
        print "Testing distribution of retweeted tweets:"
        self.check_tweet_distribution(self.retweets)

class Tweets_generate_with_expected_attribute_distribution(HashkatTestCase, unittest.TestCase):
    '''Reproducing github issues #109 & #110 et al.: 
       Are retweeted tweets not a representative sample for language, region, and hashtaggedness?'''
    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    use_full_checks = False
    n_runs = 2
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
        print counts["LANG_ENGLISH"], counts["LANG_FRENCH"]
        print counts["Region0"], counts["Region1"]
        self.assertTrue(reasonably_close(counts["LANG_ENGLISH"], counts["LANG_FRENCH"], 4))
        self.assertTrue(reasonably_close(counts["Region0"], counts["Region1"]))
    def on_exit_all(self):
        print "Testing distribution of all tweets:"
        self.check_tweet_distribution(self.tweets)
        print "Testing distribution of retweeted tweets:"
        self.check_tweet_distribution(self.retweets)

class Standard_deviation_for_retweet_counts_in_separate_runs_is_not_too_high(HashkatTestCase, unittest.TestCase):
    '''Reports the standard deviation over 20 seeds of a retweet-enabled network. 
       A rough comparison to previous network stats is used under the assumption they are correct.'''

    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    use_full_checks = False
    n_runs = 20
    def on_start_all(self):
        self.initial_retweet_counts = [0 for _ in range(self.n_runs)]
        self.all_retweet_counts = [0 for _ in range(self.n_runs)]
    # Configure the base configuration
    def on_start(self, yaml):
        self.args = ['--seed', str(self.runs + 1)]
        # Use a time-frame that allows for a few thousand retweets at least:
        yaml["analysis"]["max_time"] = 1200000
        # Make retweets relatively infrequent:
        tweet_transmission = yaml["preference_classes"][0]["tweet_transmission"]
        for key in tweet_transmission:
            tweet_transmission[key] = {"all": 0.1}
    def on_exit(self):
        print (self.runs+1), 'initial retweets', self.initial_retweet_counts[self.runs]
        #print (self.runs+1), 'total retweets', self.all_retweet_counts[self.runs]
    def on_retweet(self, tweet):
        if tweet['retweets_since_origin'] == 1:
            self.initial_retweet_counts[self.runs] += 1
        self.all_retweet_counts[self.runs] += 1
    def on_exit_all(self):
	print "Standard deviation for initial retweet amounts:", stdev(self.initial_retweet_counts)
	print "Standard deviation for total retweet amounts:", stdev(self.all_retweet_counts)
        self.assertTrue(stdev(self.initial_retweet_counts) < 3500,
            "Standard deviation for direct-author-follower retweet counts is much higher than expected.")
        self.assertTrue(stdev(self.all_retweet_counts) > 500,
            "Standard deviation for all retweet counts is much lower than expected.")
        self.assertTrue(stdev(self.all_retweet_counts) < 5000,
            "Standard deviation for all retweet counts is much higher than expected.")
            #print (i+1), 'total retweets', self.all_retweet_counts[i]

class X_end_variable_should_not_affect_direct_follower_retweets(HashkatTestCase, KnownFailingTest):
    '''Reproducer for #43. Changing the 'time_span' variable in tweet_observation should not always result in exactly the same retweet amount.'''

    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    use_full_checks = False
    n_runs = 10
    def on_start_all(self):
        self.initial_retweet_counts = [0 for _ in range(self.n_runs)]
        self.all_retweet_counts = [0 for _ in range(self.n_runs)]
    # Configure the base configuration
    def on_start(self, yaml):
        # Use a time-frame that allows for a few thousand retweets at least:
        yaml["analysis"]["max_time"] = 1200000
        # Set up a variable 'x_end', to see its affects on retweeting numbers:
        yaml["tweet_observation"]["x_end"] = self.runs * 100 + 300
        # Make retweets relatively infrequent:
        tweet_transmission = yaml["preference_classes"][0]["tweet_transmission"]
        for key in tweet_transmission:
            tweet_transmission[key] = {"all": 0.1}
    def on_exit(self):
        print (self.runs+1), 'initial retweets', self.initial_retweet_counts[self.runs]
        #print (self.runs+1), 'total retweets', self.all_retweet_counts[self.runs]
    def on_retweet(self, tweet):
        if tweet['retweets_since_origin'] == 1:
            self.initial_retweet_counts[self.runs] += 1
        self.all_retweet_counts[self.runs] += 1
    def on_exit_all(self):
        for i in range(self.n_runs):
            print (i+1), 'initial retweets', self.initial_retweet_counts[i]
            self.assertTrue( 7500 < self.initial_retweet_counts[i] < 9000,
                "x_end should produce approximately the same value, between 7500 and 9000 conservatively. Was " + str(self.initial_retweet_counts[i]) + " for " + str(i))
	print "Standard deviation for initial retweet amounts:", stdev(self.initial_retweet_counts)
	print "Standard deviation for total retweet amounts:", stdev(self.all_retweet_counts)
            #print (i+1), 'total retweets', self.all_retweet_counts[i]

class Mean_retweet_time_should_cause_slightly_different_networks(HashkatTestCase):
    '''Reproducer for #43. Changing the 'time_span' variable in tweet_observation should not always result in exactly the same retweet amount.'''

    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    use_full_checks = False
    n_runs = 10
    def on_start_all(self):
        self.initial_retweet_counts = [0 for _ in range(self.n_runs)]
        self.all_retweet_counts = [0 for _ in range(self.n_runs)]
    # Configure the base configuration
    def on_start(self, yaml):
        # Use a time-frame that allows for a few thousand retweets at least:
        yaml["analysis"]["max_time"] = 900000
        # Set up a variable time-frame:
        yaml["tweet_observation"]["time_span"] = str((self.runs + 1) * 2)
        # Make retweets relatively infrequent:
        tweet_transmission = yaml["preference_classes"][0]["tweet_transmission"]
        for key in tweet_transmission:
            tweet_transmission[key] = {"all": 0.1}
    def on_exit(self):
        print (self.runs+1), 'initial retweets', self.initial_retweet_counts[self.runs]
        print (self.runs+1), 'total retweets', self.all_retweet_counts[self.runs]
    def on_retweet(self, tweet):
        if tweet['retweets_since_origin'] == 1:
            self.initial_retweet_counts[self.runs] += 1
        self.all_retweet_counts[self.runs] += 1
    def on_exit_all(self):
        cmp_amount = self.all_retweet_counts[0]
        one_differs = False
        for i in range(self.n_runs):
            print (i+1), 'initial retweets', self.initial_retweet_counts[i]
            print (i+1), 'total retweets', self.all_retweet_counts[i]
            if self.all_retweet_counts[i] != cmp_amount:
                one_differs = True
        self.assertTrue(one_differs, 
            "Changing time_span resulted in exactly the same amount of retweets! This implies exactly the same network is being created regardless of this parameter!")

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
        print hashkat_dump_stats(self.state)
        counts = get_tweet_feature_counts(self.tweets)
        self.assertTrue(counts["DoesTweet"] > 0, 
            "Agent type with tweet rate > 0 should tweet.")
        self.assertTrue(counts["DoesntTweet"] == 0, 
            "Agent type with tweet rate == 0 should NOT tweet. Had " + str(counts["DoesntTweet"]) + " tweets")

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
        for i in range(3):
            print i, self.retweets[i]
        self.assertTrue(self.retweets[0] > 50, 
            "Should have at least 50 retweets in the first 5 runs totalled")
        self.assertTrue(self.retweets[1] > self.retweets[0] * 1.5, 
            "Should have >= 1.5x as many retweets in the second set of 5 runs as the first 5")
        self.assertTrue(self.retweets[2] > self.retweets[1] * 1.5,
            "Should have >= 1.5x as many retweets in the third set of 5 runs as the second 5")
        # Test that we correctly exit at EXPECTED_STEPS, as specified by max_analysis_steps:
#        self.assertTrue(stats["n_steps"] == self.steps)
#        self.assertTrue(stats["n_steps"] == self.EXPECTED_STEPS )

if __name__ == "__main__":
    unittest.main()
