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
    Location location;
    Language language;
    int humour_bin;

    // Store the two directions of the follow relationship
    FollowingSet following_set;
    FollowerSet follower_set;

    Entity() {
        entity_type = 0;
        preference_class = 0;
        creation_time = 0.0;
        n_tweets = 0;
        language = (Language)-1; // Initialize with invalid language
        n_retweets = 0;
        humour_bin = -1; // Initialize with invalid humour bin
    }


    PREVISIT0(rw) {
        rw << entity_type << preference_class
           << n_tweets << n_retweets << creation_time
           << location << language
           << humour_bin;
    }

    // 'Visits' every node, eg for serialization or testing
    VISIT(rw, context) {
        if (rw.is_reading()) {
            std::vector<int> followings;
            std::vector<int> followers;
            rw << followings << followers;
            for (int id_fol : followings) {
                following_set.add(rw.state, id_fol); 
            }
            for (int id_fol : followers) {
                follower_set.add(context, id_fol); 
            }
            double rate;
            rw << rate;
            ASSERT(follower_set.get_total_rate() == rate, "Serialization error!");
        } else if (rw.is_writing()) {
            rw.write_container(following_set);
            rw.write_container(follower_set);
            rw << follower_set.get_total_rate();
        }
    }
private:
    // Ban the copy constructor:
    Entity(const Entity&);
};

#endif
