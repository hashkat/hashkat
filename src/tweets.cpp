#include "lcommon/strformat.h"

#include "analyzer.h"
#include "tweets.h"
#include "entity.h"

double TweetRateDeterminer::get_age(const Tweet& tweet) {
    double age = (state.time - tweet.creation_time);
    DEBUG_CHECK(age >= 0.0, "Cannot have negative ages!");
    return age;
}

void TweetRateDeterminer::update_rate(TweetReactRateVec& rates, const Tweet& tweet, int bin) {
    rates = get_rate(tweet, bin);
}

TweetReactRateVec TweetRateDeterminer::get_rate(const Tweet& tweet, int bin) {
    Entity& entity = state.network[tweet.id_tweeter];
    Language lang = entity.language;
    FollowerSet followers = entity.follower_set;

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
    
    TweetReactRateVec rates;

    // Assumption: Our rate vector is in the same order as
    // our follower-set traversal.
    //
    // In other words, we assume that we can fill 'rates'
    // with the rates for each terminal bin in follower-set,
    // and will decode it in the same order when making a retweet decision.

    int n_elems = 0;
    for (auto& lang_cat : followers.children()) {
        for (auto& dist_cat : lang_cat.children()) {
            for (auto& pref_cat : dist_cat.children()) {
                double rate = pref_cat.get_total_rate();
                rates.add(0, rate); // TODO: Remove 'magic' 0 (only element in rates 'vector')
                n_elems += pref_cat.size();
            }
        }
    }

    DEBUG_CHECK(n_elems == followers.size(), "Amount of entities in the follower set don't match up!");
    return rates;
}

TweetBank::TweetBank(AnalysisState& state) :
        tree(TweetRateDeterminer(state),
                state.config.tweet_obs.initial_resolution,
                /*n_bins: */
                state.config.tweet_obs.values.size()) {
}
