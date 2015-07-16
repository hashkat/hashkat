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

#ifndef REFACTORING

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

    int size() const
    {   return n_agents;    }
    int max_size() const
    {   return max_agents;  }
    void grow()
    {   ++n_agents;  }

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

#else
template <typename T>
class network
{
    Agent* agents_; // This is a pointer - used to create a dynamic array
    T n_agents_, max_agents_;
// new member variables
    std::vector<std::unordered_set<T>> followers_;
    std::vector<std::unordered_set<T>> following_;

public:
    network()
    :   agents_(nullptr)
    ,   n_agents_(0)
    ,   max_agents_(0)
    {}

    ~network()
    {   delete[] agents_;    }

    T size() const
    {   return n_agents_;    }

    T max_size() const
    {   return max_agents_;  }

    void grow(T n = 1)
    {
        for (auto i = 0; i < n; ++i)
        {
            followers_.emplace_back(std::unordered_set<T>());
            following_.emplace_back(std::unordered_set<T>());
            ++n_agents_;
        }
    }

    Agent& operator[](T index)
    {
        DEBUG_CHECK(index >= 0 && index < n_agents_,
            "Network out-of-bounds agent access");
        return agents_[index];
    }

    void allocate(T n)
    {
        if (agents_)
            delete[] agents_;
        max_agents_ = n;
        agents_ = new Agent[max_agents_];
        followers_.reserve(max_agents_);
        following_.reserve(max_agents_);
    }

    // Convenient network queries:
    FollowingSet& following_set(T id)
    {   return agents_[id].following_set;    }

    bool is_valid_id(T id)
    {   return (id >= 0 && id < n_agents_);  }

    FollowerSet& follower_set(T id)
    {   return agents_[id].follower_set;     }

    // Return last agent:
    Agent& back()
    {   return (*this)[n_agents_ - 1];       }

    T n_followings(T id)
    {   return following_set(id).size();    }
    T n_followers(T id)
    {   return follower_set(id).size();     }

    // To allow for-each style loops:
    Agent* begin()
    {   return agents_;  }
    Agent* end()
    {   return agents_ + n_agents_;   }

    // 'Visits' every node, eg for serialization or testing
    READ_WRITE(rw)
    {
        rw << n_agents_ << max_agents_;
        if (rw.is_reading())
            allocate(max_agents_);

        // First, 'previsit' every agent so that we can init their basic data,
        // needed by their visit() method afterwards
        for (auto i = 0; i < n_agents_; ++i)
            agents_[i].previsit(rw);
        for (auto i = 0; i < n_agents_; ++i)
            agents_[i].visit(rw);
    }

    // new member methods
    bool can_grow() const
    {   return n_agents_ < max_agents_;    }

    void connect(T followed_id, T follower_id)
    {
        if (followed_id != follower_id)
        {
            followers_[followed_id].insert(follower_id);
            following_[follower_id].insert(followed_id);
        }
    }

    void disconnect(T unfollowed_id, T unfollower_id)
    {
        if (unfollowed_id != unfollower_id)
        {
            followers_[unfollowed_id].erase(unfollower_id);
            following_[unfollower_id].erase(unfollowed_id);
        }
    }

    T following_size(T id)
    {   return following_[id].size();   }

    T followers_size(T id)
    {   return followers_[id].size();   }

    std::ostream& print(std::ostream& out) const
    {
        for (auto i = 0; i < n_agents_; ++i)
        {
            out << agents_[i].id << "\t [ ";
            for (auto follower : followers_[i])
                    out << follower << ' ';
            out << "] [ ";
            for (auto following : following_[i])
                    out << following << ' ';
            out << "]\n";

        }
        return out;
    }
};

typedef network<std::size_t> Network;
//typedef network<int> Network;

//std::ostream& operator<<(std::ostream& out, const Network& n)
//{
//    return n.print(out);
//}

#endif  // REFACTORING

#endif  // HASHKAT_NETWORK_H_
