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
    int entity_type;
    int preference_class;
    int n_tweets, n_retweets;
    double creation_time;
    // this is the average chatiness of the entities following list
    double avg_chatiness;
    Location location;
    Language language;
    int humour_bin;
    
    // list of flagged chatty people
    std::vector<int> chatty_entities;

    // Store the two directions of the follow relationship
    FollowingSet following_set;
    FollowerSet follower_set;

    Entity() {
        entity_type = 0;
        preference_class = 0;
        creation_time = 0.0;
        avg_chatiness = -1.0;
        n_tweets = 0;
        language = (Language)-1; // Initialize with invalid language
        n_retweets = 0;
        humour_bin = -1; // Initialize with invalid humour bin
    }

    PREREAD_WRITE(rw) {
        rw << entity_type << preference_class
           << n_tweets << n_retweets << creation_time << avg_chatiness
           << location << language
           << humour_bin << chatty_entities;
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
private:
    // Ban the copy constructor:
    Entity(const Entity&);
};

#endif
