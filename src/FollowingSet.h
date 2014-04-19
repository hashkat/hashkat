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

#ifndef FOLLOWING_SET_H_
#define FOLLOWING_SET_H_

#include "cat_classes.h"
#include "cat_nodes.h"

#include "events.h"
#include "config_static.h"

// Forward declare, to prevent circular header inclusion:
struct AnalysisState;

struct FollowingSet {
    typedef cats::LeafNode<int> Followings;
    typedef cats::NodeIterator<Followings> iterator;

    void print(AnalysisState& S) {
        implementation.print(S, /*Dummy: */ 1.0);
    }

    void print(AnalysisState& S, int id) {
        implementation.add(S, /*Dummy: */ 1.0, id);
    }

    bool add(AnalysisState& S, int id) {
        return implementation.add(S, /*Dummy: */ 1.0, id);
    }

    bool remove(AnalysisState& S, int id) {
        return implementation.remove(S, /*Dummy: */ 1.0, id);
    }

    // No such thing as pick_random_weighted for FollowingSet
    bool pick_random_uniform(MTwist& rng, int& id) {
        return implementation.pick_random_uniform(rng, id);
    }

    iterator begin() {
        return iterator::begin(&implementation);
    }

    iterator end() {
        return iterator::end(&implementation);
    }

    size_t size() const {
        return implementation.size();
    }

private:
    Followings implementation;
};

#endif
