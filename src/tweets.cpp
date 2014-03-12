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
    Entity entity = state.network[tweet.id_tweeter];
    Language lang = entity.language;
    FollowerSet followers = entity.follower_set;
    int n_elems = 0;

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

    /* A  macro to iterate over the language layer of a follower_set.
        NOTE: 'bin_id', 'layer' are declared by the macro! */
     FOR_LANG_LAYER(followers, bin_id, layer) {
        int rate_bin = 0;
        // All languages are treated equally for now
        for (int i = 0; i < layer->n_bins(); i++) {
            FollowerSetLayer2& layer2 = layer[i];
            for (int j = 0; j < layer2.n_bins(); j++) {
                double rate = (*layer)[j].get_total_rate();
                rates.add(rate_bin, rate);
                rate_bin++;
            }
        }
        DEBUG_CHECK(rate_bin == RETWEET_RATE_ELEMENTS, "Dimensionality mismatch!");
        n_elems += layer->size();
     }
    DEBUG_CHECK(n_elems == followers.size(), "Amount of entities in the follower set don't match up!");

    printf("GETTING RATE:\n");
    rates.print();
    return rates;
}

TweetBank::TweetBank(AnalysisState& state) :
        tree(TweetRateDeterminer(state),
                state.config.tweet_obs.initial_resolution,
                /*n_bins: */
                state.config.tweet_obs.values.size()) {
}
