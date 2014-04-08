#include "lcommon/strformat.h"

#include <iostream>
#include "analyzer.h"
#include "tweets.h"
#include "entity.h"

using namespace std;

double TweetRateDeterminer::get_age(const Tweet& tweet) {
    double age = (state.time - tweet.creation_time);
    DEBUG_CHECK(age >= 0.0, "Cannot have negative ages!");
    return age;
}

void TweetRateDeterminer::update_rate(TweetReactRateVec& rates, const Tweet& tweet, int bin) {
    rates = get_rate(tweet, bin);
}

double TweetRateDeterminer::get_cat_threshold(int bin) {
    return state.config.tweet_obs.thresholds[bin];
}

TweetReactRateVec TweetRateDeterminer::get_rate(const Tweet& tweet, int bin) {
    Entity& entity = state.network[tweet.id_tweeter];
    Language lang = entity.language;
    FollowerSet& followers = entity.follower_set;

    /********************************************************************
     * Determine the 'Omega' observation PDF.
     * This describes the probability density of a tweet reaction 
     * occurring at a specific time, given that someone reacts to a 
     * retweet eventually.
     ********************************************************************/
    
    double obs_prob = state.config.tweet_obs.values[bin];

    /********************************************************************
     * Determine the (fixed-length) vector 'rates', which stores the rate
     * with which a given category reacts to the tweeter.
     * We scale by 'obs_prob'.
     ********************************************************************/

    // Assumption: react_weights is initialized to the appropriate
    // weights for this tweet.

    double weight_sum = 0;
    for (double d : tweet.react_weights.weights) {
        weight_sum += d;
    }

    return TweetReactRateVec(obs_prob * weight_sum);
}

bool TimeDepRateTree::ElementChecker::check(ref_t id) {
    AnalysisState& state = tree.determiner.state;
    Tweet& t = tree.get(id).data;
//        printf("CHECKING %d time(%f) > t.retweet_next_rebin_time(%f)\n", id, time, t.retweet_next_rebin_time);
    if (time > t.retweet_next_rebin_time) {
        // Move to a new bin:
        t.retweet_time_bin++;
        if (t.retweet_time_bin >= tree.n_bins()) {
            //                    printf("BOOTING NODE %d AT BIN %d\n", id,  t.retweet_time_bin);
            tree.tree.remove(id);
        } else {
            //                    printf("MOVING TO BIN %d\n", t.retweet_time_bin);
//            printf("RATE BEFORE %f\n", tree.get(id).rates.tuple_sum);
            t.retweet_next_rebin_time = t.creation_time
                    + tree.determiner.get_cat_threshold(t.retweet_time_bin);
            TweetReactRateVec rates = tree.determiner.get_rate(t, t.retweet_time_bin);
            tree.tree.replace_rate(id, rates);
//            printf("RATE AFTER %f\n", tree.get(id).rates.tuple_sum);
        }
        return false;
    }
    return true;
}


TweetBank::TweetBank(AnalysisState& state) :
        tree(TweetRateDeterminer(state),
                state.config.tweet_obs.initial_resolution,
                /*n_bins: */
                state.config.tweet_obs.values.size()) {
}
