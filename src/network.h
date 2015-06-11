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

#include "agent.h"
#include "events.h"
#include "tweets.h"

struct Network {
    Agent* agents; //** This is a pointer - used to create a dynamic array
    int n_agents, max_agents;
    Network() {
        agents = NULL;
        n_agents = 0;
        max_agents = 0;
    }
    ~Network() { //** This defines how to clean-up our Network object; we free the dynamic array
        delete[] agents;
    }
    Agent& operator[](int index) { //** This allows us to index our Network struct as if it were an array.
        DEBUG_CHECK(index >= 0 && index < n_agents, "Network out-of-bounds agent access");
        return agents[index];
    }

    void allocate(int n) {
        if (agents) {
            delete[] agents;
        }
        max_agents = n;
        agents = new Agent[max_agents];
    }

    // Convenient network queries:
    FollowingSet& following_set(int id) {
        return agents[id].following_set;
    }

    bool is_valid_id(int id) {
        return (id >= 0 && id < n_agents);
    }

    FollowerSet& follower_set(int id) {
        return agents[id].follower_set;
    }

    // Return last agent:
    Agent& back() {
        return (*this)[n_agents - 1];
    }

    size_t n_followings(int id) {
        return following_set(id).size();
    }
    size_t n_followers(int id) {
        return follower_set(id).size();
    }

    // To allow for-each style loops:
    Agent* begin() {
        return agents;
    }
    Agent* end() {
        return agents + n_agents;
    }
    // 'Visits' every node, eg for serialization or testing
    READ_WRITE(rw) {
        rw << n_agents << max_agents;
        if (rw.is_reading()) {
            allocate(max_agents);
        }
        /* First, 'previsit' every agent so that we can init their basic data, needed by their visit() method afterwards. */
        for (int i = 0; i < n_agents; i++) {
            agents[i].previsit(rw);
        }
        for (int i = 0; i < n_agents; i++) {
            agents[i].visit(rw);
        }
    }
};

#endif
