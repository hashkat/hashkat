#include "lcommon/strformat.h"

#include "analyzer.h"
#include "tweets.h"
#include "entity.h"

double TweetRateDeterminer::get_age(Tweet& tweet) {
    double age = (state.time - tweet.creation_time);
    DEBUG_CHECK(age >= 0.0, "Cannot have negative ages!");
    return age;
}

double TweetRateDeterminer::get_rate(Tweet& tweet, int bin) {
    return state.config.tweet_obs.values[bin];
}

TweetBank::TweetBank(AnalysisState& state) :
        tree(TweetRateDeterminer(state),
                state.config.tweet_obs.initial_resolution, /*n_bins: */
                state.config.tweet_obs.values.size()) {
}
