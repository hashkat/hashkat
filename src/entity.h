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

#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>
#include <vector>

#include "CategoryGrouper.h"

#include "lcommon/smartptr.h"
#include "mtwist.h"

#include "events.h"

#include "FollowingSet.h"
#include "FollowerSet.h"

#include "tweets.h"

#include "DataReadWrite.h"

// Forward declare, to prevent circular header inclusion:
struct AnalysisState;

struct Entity {
    // Storing the id is redundant (since it can be inferred), but provides convenient handling
    int id = -1;

    //** AD: The ability to put initializers right in the class
    //** is new in C++11

    int entity_type = -1;
    int preference_class = -1;
    int n_tweets = 0, n_retweets = 0;

    // Abstract location:
    int region_bin = -1;

    double ideology_tweet_percent = 0;
    double creation_time = 0;

    // this is the average chatiness of the entities following list
    double avg_chatiness = 0.0;
    Language language = (Language)-1;

   // Abstract ideology:
    int ideology_bin = -1;
    
    // list of flagged chatty people
    std::vector<int> chatty_entities;

    // Store the two directions of the follow relationship
    FollowingSet following_set;
    FollowerSet follower_set;
    // these store how someone followed you, or how you followed someone
    std::vector<int> following_method_counts;
    std::vector<int> follower_method_counts;

    Entity() {
        following_method_counts.resize(N_FOLLOW_MODELS + 2);
        follower_method_counts.resize(N_FOLLOW_MODELS + 2);
        for (int i = 0; i < N_FOLLOW_MODELS + 2; i ++) {
            following_method_counts[i] = 0;
            follower_method_counts[i] = 0;
        }
    }

    PREREAD_WRITE(rw) {
        rw << id << entity_type << preference_class
           << n_tweets << n_retweets 
           << region_bin
           << ideology_tweet_percent << creation_time
           << avg_chatiness
           << language
           << ideology_bin
           << chatty_entities;
        // following_set and follower_set are specially handled below
    }

    // 'Visits' every node, eg for serialization or testing
    READ_WRITE(rw) {
        following_set.visit(rw);
        follower_set.visit(rw);
    }

};

struct EntityStats {
    int64 n_follows = 0, n_followers = 0, n_tweets = 0, n_retweets = 0, n_unfollows = 0;
    int64 n_followback = 0;
    int64 n_random_follows = 0, n_preferential_follows = 0;
    int64 n_entity_follows = 0, n_pref_entity_follows = 0;
    int64 n_retweet_follows = 0, n_hashtag_follows = 0;
    int64 n_hashtags = 0;
};

/*
 * An entity type is a static class of entity, determined at creation.
 * Entity types are intended to represent different kinds of network participants,
 *
 * For a network like Twitter, example entity types include 'Standard User', 'Celebrity', etc.
 */

struct EntityType {
    std::string name;
    double prob_add = 0; // When a entity is added, how likely is it that it is this entity type ?
    double prob_follow = 0; // When a entity is followed, how likely is it that it is this entity type ?
    double prob_followback = 0;
    int new_entities = 0; // the number of new users in this entity type
    Rate_Function RF[number_of_diff_events];
    bool care_about_region = false, care_about_ideology = false;
    // number of entities for each discrete value of the rate(time)
    std::vector<int> entity_cap;
    // list of entity ID's
    std::vector<int> entity_list;
    // categorize the entities by age
    CategoryGrouper age_ranks;
    CategoryGrouper follow_ranks;
    std::vector<double> updating_probs;
    double tweet_type_probs[N_TWEET_TYPES];

    EntityStats stats;

    /* Synchronize rates and preferences from a loaded configuration.
     * This is done because, although we can load a new configuration,
     * some rates remain duplicated in our state object. */
    void sync_configuration(EntityType& E) {
        ASSERT(name == E.name, "Attempting sync_configuration on different entity types! The number and relative orderings of entity types must remain constant.");
        prob_add = E.prob_add;
        prob_follow = E.prob_follow;
        prob_followback = E.prob_followback;
        for (int i = 0; i < number_of_diff_events; i++) {
            RF[i] = E.RF[i];
        }

        // Created on demand, not data (in other words, do not uncomment):
        // age_ranks.sync_rates(E.age_ranks);
        follow_ranks.sync_rates(E.follow_ranks);

        for (int i = 0; i < N_TWEET_TYPES; i++) {
            tweet_type_probs[i] = E.tweet_type_probs[i];
        }
    }

    READ_WRITE(rw) {
        rw << name << prob_add << prob_follow << prob_followback;
        rw << new_entities;
        for (auto& rf : RF) {
            rf.visit(rw);
        }
        rw << care_about_region << care_about_ideology;
        rw << entity_cap << entity_list;
        age_ranks.visit(rw);
        follow_ranks.visit(rw);
        rw << updating_probs << stats;
        for (auto& ttp : tweet_type_probs) {
            rw << ttp;
        }
    }
};

// 'EntityTypeVector' acts exactly as a 'vector'
typedef std::vector<EntityType> EntityTypeVector;


#endif
