#ifndef TWEETS_H_
#define TWEETS_H_

#include <vector>

#include "lcommon/smartptr.h"
#include "mtwist.h"

#include "TimeDepRateTree.h"
#include "cat_classes.h"

#include "events.h"
#include "entity_properties.h"


// information for when a user tweets
struct TweetContent { // AD: TEMPORARY NOTE was TweetInfo
    double time_of_tweet;
    double starting_rate, updating_rate;
    UsedEntities used_entities;
    Language language;
    int id_original_author; // The entity that created the original content
    TweetContent() {
        starting_rate = updating_rate = -1;
        time_of_tweet = -1;
        language = N_LANGS; // Set to invalid
        id_original_author = -1;
    }
};

// information for when a user retweets

// TODO Clear all temporary comments eventually
// A tweet is an orignal tweet if tweeter_id == content.author_id
struct Tweet { // AD: TEMPORARY NOTE was RetweetInfo
    // The entity broadcasting the tweet
    int id_tweeter;
    double starting_rate, updating_rate;
    // A tweet is an orignal tweet if tweeter_id == content.author_id
    smartptr<TweetContent> content;
    double creation_time;
    explicit Tweet(const smartptr<TweetContent>& content = smartptr<TweetContent>()) : content(content) {
        id_tweeter = -1;
        starting_rate = updating_rate = 0;
        creation_time = 0;
    }
    void print() {
        printf("(Tweeter = %d, Original Author = %d, Created = %.2f\n)",
                id_tweeter, content->id_original_author, creation_time);
    }
};

typedef std::vector<Tweet> TweetList;

struct MostPopularTweet {
    Tweet tweet;
    TweetList list_of_retweets;
    int count;
    int global_tweet_max;
    MostPopularTweet() {
        count = global_tweet_max = 0;
    }
};

struct AnalysisState;

struct TweetRateDeterminer {
    TweetRateDeterminer(AnalysisState& state) : state(state){
    }
    double get_age(Tweet& tweet);
    double get_rate(Tweet& tweet, int bin);

    double get_cat_threshold(int bin) {
        return pow(2, bin) * 90;
    }

    AnalysisState& state;
};

struct TweetBank {
    TimeDepRateTree<Tweet, 1 /*Just one rate for now*/, TweetRateDeterminer> tree;

    double get_total_rate() {
        return tree.rate_summary().tuple_sum;
    }

    TweetBank(AnalysisState& state);

    std::vector<Tweet> as_vector() {
        return tree.as_vector();
    }

    int n_active_tweets() const {
        return tree.size();
    }
    Tweet& pick_random_weighted(MTwist& rng) {
        ref_t ref = tree.pick_random_weighted(rng);
        return tree.get(ref).data;
    }
};

#endif
