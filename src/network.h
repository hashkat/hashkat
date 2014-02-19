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
#include "tweets.h"

//[[AD -- TEMPORARY NOTE]] TweetBank was moved to tweets.h

struct Network {
    Entity* entities; //** This is a pointer - used to create a dynamic array
    int n_entities, max_entities;
    Network() {
        entities = NULL;
        n_entities = 0;
        max_entities = 0;
    }
    ~Network() { //** This defines how to clean-up our Network object; we free the dynamic array
        delete[] entities;
    }
    Entity& operator[](int index) { //** This allows us to index our Network struct as if it were an array.
        // Note that we allow accessing the tail unallocated element (hence the <= instead of <):
        DEBUG_CHECK(index >= 0 && index <= n_entities, "Network out-of-bounds entity access");
        return entities[index];
    }

    void allocate(int n) {
        max_entities = n;
        entities = new Entity[max_entities];
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
