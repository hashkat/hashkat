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
 * the original authors (Isaac Tamblyn, Kevin Ryczko & Adam Domurad), as well, in the case of a derivation,
 * subsequent authors. 
 */

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

#include "CategoryGrouper.h"

#include "cat_classes.h"
#include "cat_nodes.h"

#include "entity.h"
#include "events.h"
#include "tweets.h"

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
        DEBUG_CHECK(index >= 0 && index < n_entities, "Network out-of-bounds entity access");
        return entities[index];
    }

    void allocate(int n) {
        if (entities) {
            delete[] entities;
        }
        max_entities = n;
        entities = new Entity[max_entities];
    }

    // Convenient network queries:
    FollowingSet& following_set(int id) {
        return entities[id].following_set;
    }

    bool is_valid_id(int id) {
        return (id >= 0 && id < n_entities);
    }

    FollowerSet& follower_set(int id) {
        return entities[id].follower_set;
    }

    // Return last entity:
    Entity& back() {
        return (*this)[n_entities - 1];
    }

    size_t n_followings(int id) {
        return following_set(id).size();
    }
    size_t n_followers(int id) {
        return follower_set(id).size();
    }

    // To allow for-each style loops:
    Entity* begin() {
        return entities;
    }
    Entity* end() {
        return entities + n_entities;
    }
    // 'Visits' every node, eg for serialization or testing
    READ_WRITE(rw) {
        rw << n_entities << max_entities;
        if (rw.is_reading()) {
            allocate(max_entities);
        }
        /* First, 'previsit' every entity so that we can init their basic data, needed by their visit() method afterwards. */
        for (int i = 0; i < n_entities; i++) {
            entities[i].previsit(rw);
        }
        for (int i = 0; i < n_entities; i++) {
            entities[i].visit(rw);
        }
    }
};

#endif
