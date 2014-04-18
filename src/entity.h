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
    double avg_chatiness = -1.0;
    Language language = (Language)-1;

   // Abstract ideology:
    int ideology_bin = -1;
    
    // list of flagged chatty people
    std::vector<int> chatty_entities;

    // Store the two directions of the follow relationship
    FollowingSet following_set;
    FollowerSet follower_set;

    PREREAD_WRITE(rw) {
        rw << entity_type << preference_class 
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
        if (rw.is_reading()) {
            std::vector<int> followings;
            rw << followings;
            for (int id_fol : followings) {
                following_set.add(rw.state, id_fol); 
            }

            int size = 0;
            rw << size;
            for (int i = 0; i < size; i++) {
                int id_fol = -1;
                rw << id_fol;
                follower_set.add(rw.state.network[id_fol]);
            }

        } else if (rw.is_writing()) {
            rw.write_container(following_set);
            int size = follower_set.size();
            rw << size;
            int n = 0;
            for (int id_fol : follower_set.as_vector()) {
                ASSERT(id_fol >= 0 && id_fol < rw.state.network.n_entities, "Bad id!");
                rw << id_fol;
                n++;
            }
            ASSERT(n == size, "Follower set iteration did not result in size() entries!");
        }
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
    double prob_add; // When a entity is added, how likely is it that it is this entity type ?
    double prob_follow; // When a entity is followed, how likely is it that it is this entity type ?
    double prob_followback;
    int new_entities; // the number of new users in this entity type
    Rate_Function RF[number_of_diff_events];

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

    EntityType() {
        new_entities = 0;
        prob_add = prob_follow = prob_followback = 0;
    }

    READ_WRITE(rw) {
        rw << name << prob_add << prob_follow << prob_followback;
        rw << new_entities;
        for (auto& rf : RF) {
            rf.visit(rw);
        }
        rw << entity_cap << entity_list;
        age_ranks.visit(rw);
        follow_ranks.visit(rw);
        rw << updating_probs << stats;
    }
};

// 'EntityTypeVector' acts exactly as a 'vector'
typedef std::vector<EntityType> EntityTypeVector;


#endif
