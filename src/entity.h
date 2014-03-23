#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>
#include <vector>

#include "CategoryGrouper.h"

#include "lcommon/smartptr.h"
#include "mtwist.h"

#include "events.h"

#include "entity_follow_sets.h"
#include "entity_properties.h"

#include "tweets.h"

#include "DataReadWrite.h"

// Forward declare, to prevent circular header inclusion:
struct AnalysisState;

struct Entity {
    //** AD: The ability to put initializers right in the class
    //** is new in C++11

    int entity_type = 0;
    int preference_class = 0;
    int n_tweets = 0, n_retweets = 0;

    // Abstract location:
    int region_bin = -1, subregion_bin = -1;

    double ideology_tweet_percent = 0;
    double creation_time = 0;

    // this is the average chatiness of the entities following list
    double avg_chatiness = -1.0;
    Location location;
    Language language = (Language)-1;

    int humour_bin = -1;
    int ideology_bin = -1;
    bool uses_hashtags = false;
    
    // list of flagged chatty people
    std::vector<int> chatty_entities;

    // Store the two directions of the follow relationship
    FollowingSet following_set;
    FollowerSet follower_set;

    PREREAD_WRITE(rw) {
        rw << entity_type << preference_class 
           << n_tweets << n_retweets 
           << region_bin << subregion_bin
           << ideology_tweet_percent << creation_time
           << avg_chatiness
           << location << language
<<<<<<< HEAD
           << humour_bin << chatty_entities;
=======
           << humour_bin << ideology_bin << uses_hashtags
           << chatty_entities;
        // following_set and follower_set are specially handled below
>>>>>>> edd144ecfb5d4aed25639e1b4e7b90af0919a968
    }

    // 'Visits' every node, eg for serialization or testing
    READ_WRITE1(rw, context) {
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
                follower_set.add(context, id_fol); 
            }

            double rate;
            rw << rate;
            ASSERT(fabs(follower_set.get_total_rate() - rate) < ZEROTOL, "Serialization error!");
        } else if (rw.is_writing()) {
            rw.write_container(following_set);
            int size = follower_set.size();
            rw << size;
            int n = 0;
            for (int id_fol : follower_set) {
                rw << id_fol;
                n++;
            }
            ASSERT(n == size, "Follower set iteration did not result in size() entries!");
            rw << follower_set.get_total_rate();
        }
    }

};

#endif
