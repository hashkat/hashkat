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

#ifndef TWEETS_H_
#define TWEETS_H_

#include <vector>
#include <cstdio>
#include <memory>

#include "mtwist.h"

#include "events.h"
#include "CircularBuffer.h"

#include "serialization.h"

#include "FollowerSet.h"

typedef HashedEdgeSet<int> UsedAgents;

// information for when a user tweets
struct TweetContent {
    int id = -1;
    TweetType type = (TweetType)-1;
    double time_of_tweet = -1;
    Language language = N_LANGS; // Set to invalid
    int ideology_bin = -1; // 0 == no ideology
    int hashtag_bin = -1;
    int id_original_author = -1; // The agent that created the original content
    UsedAgents used_agents;

    template <typename Archive>
    void serialize(Archive& ar) {
        // Save/load scalars:
        ar(NVP(id), NVP(type));
        ar(NVP(time_of_tweet), NVP(language), NVP(ideology_bin), NVP(hashtag_bin), NVP(id_original_author));

        // Save/load used_agents:
        ar(NVP(used_agents));
    }
};

// Represents a tweet, either original, or a rebroadcast.
struct Tweet {
    int id_tweet = -1; // The tweet number
    // The agent broadcasting the tweet
    // A tweet is an orignal tweet if tweeter_id == content.author_id
    int id_tweeter = 0;

    // The 'linking' agent the tweet was retweeted from (a following of id_tweeter)
    // Equal to id_tweeter if the tweet was original content.
    int id_link = -1;
    // The generation of the tweet, 0 if the tweet was original content
    int generation = -1;
    // A tweet is an orignal tweet if tweeter_id == content.author_id
    std::shared_ptr<TweetContent> content;

    // The time the tweet was tweeted
    double creation_time = 0;
    double deletion_time = 0;

    // Based on creation_time and the current time
    // Determines the 'omega' observation PDF dropoff rate of
    // the tweet's retweetability.
    int retweet_time_bin = -1;
    
    // does this tweet include a hashtag, set false as default
    bool hashtag = false;

    // Next time to consider rebinning, always more than creation_time
    double retweet_next_rebin_time = -1;

    /* Rates with which this tweet is retweeted: */
    FollowerSet::Weights react_weights;

    explicit Tweet(const std::shared_ptr<TweetContent>& content = {}) {
        this->content = content;
    }

    void print() {
        printf("(#%d, Tweeter = %d, Link = %d, Original Author = %d, Created = %.2f\n)",
                id_tweet, id_tweeter, id_link, content->id_original_author, creation_time);
    }

    // Note: Must be called directly, unlike 'serialize' which is implicitly called when serializing subobjects.
    void api_serialize(cereal::JSONOutputArchive& ar); 

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(NVP(id_tweet), NVP(id_tweeter), NVP(id_link), NVP(generation));
        ar(NVP(content));
        ar(NVP(creation_time), NVP(deletion_time), NVP(retweet_time_bin), NVP(hashtag), NVP(retweet_next_rebin_time));
        // NOTE: Relies on FollowerSet::Weights being a 'plain' (pointer-free) object!
        ar(NVP(react_weights));
    }
};

struct AnalysisState;

struct MostPopularTweet { 
    // this is the most retweeted tweet
    Tweet most_popular_tweet;
    // The max number of retweets for one tweet
    int global_max = 0;

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(NVP(most_popular_tweet));
        ar(NVP(global_max));
    }
};

struct HashtagGroup {
    // circular buffer with 100 elements
    CircularBuffer<int, 100> circ_buffer;
    template <typename Archive>
    void serialize(Archive& ar) {
        ar(NVP(circ_buffer));
    }
};

struct HashTags {
    // this is the set of bins for idealogies and regions
    HashtagGroup hashtag_groups[N_BIN_IDEOLOGIES][N_BIN_REGIONS];

    int choose_bin(MTwist& rng, bool choice, int default_bin, const int n_choices) {
        if (!choice) {
            return rng.rand_int((int) n_choices);
        }
        return default_bin;
    }

    int select_agent(AnalysisState& state, bool region_choice, bool ideology_choice, int default_region, int default_ideology); 

    template <typename Archive>
    void serialize(Archive& ar) {
        for (auto& outer : hashtag_groups) {
            for (auto& groups : outer) {
                // Note: works because HashtagGroup contains no pointers internally:
                ar(groups);
            }
        }
    }
};

#endif
