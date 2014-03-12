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
    TweetReactRateVec rates;

    /* A  macro to iterate over the language layer of a follower_set.
        NOTE: 'bin_id', 'layer' are declared by the macro! */
     FOR_LANG_LAYER(followers, bin_id, layer) {
           // Do something with 'layer'
        n_elems += layer->size();
     }
    DEBUG_CHECK(n_elems == followers.size(), "Amount of entities in the follower set don't match up!");

    return state.config.tweet_obs.values[bin];
}

TweetBank::TweetBank(AnalysisState& state) :
        tree(TweetRateDeterminer(state),
                state.config.tweet_obs.initial_resolution,
                /*n_bins: */
                state.config.tweet_obs.values.size()) {
}
