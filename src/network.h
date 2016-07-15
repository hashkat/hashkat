///////////////////////////////////////////////////////////////////////////////
// This file is part of the #KAT Social Network Simulator.
//
// The #KAT Social Network Simulator is free software: you can redistribute it
// and/or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// The #KAT Social Network Simulator is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// the #KAT Social Network Simulator. If not, see http://www.gnu.org/licenses.
//
// Addendum:
//
// Under this license, derivations of the #KAT Social Network Simulator
// typically must be provided in source form. The #KAT Social Network Simulator
// and derivations thereof may be relicensed by decision of the original
// authors (Kevin Ryczko & Adam Domurad, Isaac Tamblyn), as well, in the case
// of a derivation, subsequent authors.
//


// Network state structures

#ifndef HASHKAT_NETWORK_H_   //** Header guards are necessary because C++ has a very low-level include syntax.
#define HASHKAT_NETWORK_H_   //** Without them, you can get strange multiple-definition errors due to text duplication.

#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <exception>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "util.h"

#include "CategoryGrouper.h"

#include "agent.h"
#include "events.h"
#include "tweets.h"

class Network {
    std::vector<Agent> agents;
    int n_agents = 0, max_agents = 0;
public:
    int size() const {
        return n_agents;
    }
    int max_size() const {
        return max_agents;
    }
    void grow() {
        DEBUG_CHECK(n_agents < max_agents, "Cannot grow network, at max_agents!");
        ++n_agents;
    }

    Agent& operator[](int index) { //** This allows us to index our Network struct as if it were an array.
        DEBUG_CHECK(index >= 0 && index < n_agents, "Network out-of-bounds agent access");
        return agents[index];
    }

    void allocate(int n) {
        max_agents = n;
        agents = std::vector<Agent>(max_agents);
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
        return &agents[0];
    }
    Agent* end() {
        return begin() + n_agents;
    }

    template <typename Archive>
    void save(Archive& ar) const {
        ar(n_agents, max_agents, agents);
    }
    template <typename Archive>
    void load(Archive& ar) {
        ar(n_agents, max_agents, agents);
        for (Agent& agent : *this) {
            agent.post_load(get_state(ar));
        }
    }
};

#endif  // HASHKAT_NETWORK_H_
