/* Network state structures */

#ifndef __NETWORK_H_ //** Header guards are necessary because C++ has a very low-level include syntax.
#define __NETWORK_H_ //** Without them, you can get strange multiple-definition errors due to text duplication.

#include <cstdlib>
#include <vector>
#include <exception>
#include "util.h"

#include "CircularBuffer.h"
#include "CategoryGrouper.h"

#include "cat_classes.h"
#include "cat_nodes.h"

#include "entity.h"
#include "events.h"

struct Network {
    Entity* entities; //** This is a pointer - used to create a dynamic array
    int n_entities, max_entities;
    std::vector<int> recent_tweet_ID;
    std::vector<double> recent_tweet_times;
    std::vector<int> recent_retweet_ID;
    std::vector<double> recent_retweet_times;
    std::vector<double> decay_rates;
    Network() {
        entities = NULL;
        n_entities = 0;
        max_entities = 0;
    }
    ~Network() { //** This defines how to clean-up our Network object; we free the dynamic array
        free(entities);
    }
    Entity& operator[](int index) { //** This allows us to index our Network struct as if it were an array.
        DEBUG_CHECK(index >= 0 && index <= n_entities, "Network out-of-bounds entity access");
        return entities[index];
    }

    void preallocate(int n) {
        max_entities = n;
        entities = (Entity*)malloc(sizeof(Entity) * max_entities);
        // This is very likely to be a large allocation, check for failures:
        if (entities == NULL) {
            error_exit("Network::preallocate failed");
        }
        for (int i = 0; i < max_entities; i++) {
            // Placement new due to use of malloc
            new (&entities[i]) Entity();
        }
    }

    void perform_cleanup() {
        // TODO: Clean-up is no longer required as follow-sets do not permit duplicates anymore
    }

    // Convenient network queries:
    FollowSet& follow_set(int id) {
        return entities[id].follow_set;
    }
    FollowerSet& follower_set(int id) {
        return entities[id].follower_set;
    }

    size_t n_following(int id) {
        return follow_set(id).size();
    }
    size_t n_followers(int id) {
        return follower_set(id).size();
    }
};

#endif
