/* Network state structures */

#ifndef __NETWORK_H_ //** Header guards are necessary because C++ has a very low-level include syntax.
#define __NETWORK_H_ //** Without them, you can get strange multiple-definition errors due to text duplication.

#include <cstdlib>
#include <vector>
#include <exception>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "util.h"

#include "CircularBuffer.h"
#include "CategoryGrouper.h"

#include "cat_classes.h"
#include "cat_nodes.h"

#include "entity.h"
#include "events.h"


struct TweetBank {
    TweetList active_tweet_list;
    RetweetList active_retweet_list; 
   
    double half_life;
    double tolerance;
    std::vector<double> half_life_function;
    
    TweetBank() {
        half_life = 90; // 90 minutes
        tolerance = 0.01;
    }

    // gives the value for Omega(t) discussed
    double get_omega(double time_of_tweet, double actual_time) {
        double value = exp((time_of_tweet - actual_time) / half_life);
        if (value < tolerance) {
            return -1;
        }
        return value;
    }
    
};

struct Network {
    Entity* entities; //** This is a pointer - used to create a dynamic array
    int n_entities, max_entities;
    TweetBank tweet_bank;
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
