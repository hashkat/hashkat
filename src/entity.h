#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>
#include <vector>

#include "CategoryGrouper.h"

#include "lcommon/smartptr.h"
#include "mtwist.h"

#include "events.h"

#include "FollowerSet.h"

#include "entity_follow_sets.h"
#include "entity_properties.h"

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

    // Abstract humourousness:
    int humour_bin = -1;
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
           << humour_bin << ideology_bin 
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

#endif
