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

#ifndef FOLLOWING_SET_H_
#define FOLLOWING_SET_H_


#include "events.h"
#include "config_static.h"

#include "util/HashedEdgeSet.h"

// Forward declare, to prevent circular header inclusion:
struct AnalysisState;

struct FollowingSet {
    typedef HashedEdgeSet<int> Followings;

    void print(AnalysisState& S);

    std::vector<int> as_vector() {
        return implementation.as_vector();
    }

    bool add(AnalysisState& S, int id) {
        return implementation.insert(id);
    }

    bool remove(AnalysisState& S, int id) {
        return implementation.erase(id);
    }

    bool contains(int id) {
        return implementation.contains(id);
    }

    // No such thing as pick_random_weighted for FollowingSet
    bool pick_random_uniform(MTwist& rng, int& id) {
        return implementation.pick_random_uniform(rng, id);
    }

    size_t size() const {
        return implementation.size();
    }

    template <typename Archive>
    void save(Archive& ar) const {
        ar( cereal::make_size_tag( size() ) );
        for (int edge : ((FollowingSet*)this)->implementation.as_vector()) {
            ar(edge);
        }
    }

    template <typename Archive>
    void load(Archive& ar) {
        size_t size;
        ar( cereal::make_size_tag( size ) );
        for (size_t i = 0; i < size ; i++) {
            int edge;
            ar(edge);
            implementation.insert(edge);
        }
    }
    void post_load(AnalysisState& state) {
    }
private:
    Followings implementation;
};

#endif
