/*
 * This file is part of the #KAT Social Network Simulator.
 *
 * The #KAT Social Network Simulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The #KAT Social Network Simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the #KAT Social Network Simulator.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Addendum:
 *
 * Under this license, derivations of the #KAT Social Network Simulator typically must be provided in source
 * form. The #KAT Social Network Simulator and derivations thereof may be relicensed by decision of 
 * the original authors (Kevin Ryczko & Adam Domurad, Isaac Tamblyn), as well, in the case of a derivation,
 * subsequent authors. 
 */

#include "lcommon/strformat.h"

#include <iostream>
#include "analyzer.h"
#include "tweets.h"
#include "agent.h"

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
    Agent& agent = state.network[tweet.id_tweeter];
    FollowerSet& followers = agent.follower_set;

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

    return TweetReactRateVec(obs_prob * tweet.react_weights.total_weight);
}

void appendOldTweet(AnalysisState& state, Tweet& t) {
    // Only keep track of tweets if we are going to use 
    // the information for later output (io.cpp)
    if (state.config.full_tweet_stats) {
        t.deletion_time = state.time;
        state.old_tweets.push_back(t);
    }
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
            // Here is the hook, the tweet with id = id is about to be kicked
            appendOldTweet(state, t);
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
