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

#ifndef FOLLOWERSET_H_
#define FOLLOWERSET_H_

#include <cstdio>
#include <vector>
#include <functional>
#include <cmath>
#include "util.h"

#include "util/HashedEdgeSet.h"

// For bin limits:
#include "config_static.h"

// Forward declarations to prevent circular imports:
struct Agent;
struct Tweet;
struct TweetContent;

/*****************************************************************************
 * Categorization layers of the follower set.
 * The categorization layers are:
 *   Language
 *   X Preference class
 *   X Ideology
 *
 * The weight determiner layers are:
 *   Agent preference class
 *   X Tweet type (for ideological tweets, whether ideologies match)
 *   X Original tweeter agent type
 *****************************************************************************/

// Leaf layer
struct IdeologyLayer {
    static const int N_SUBLAYERS = N_BIN_IDEOLOGIES;

    struct Weights {
        double weights[N_SUBLAYERS] = {0};
        double total_weight = 0;
    };

    static int classify(Agent& agent);

    int n_elems = 0; // Total
    HashedEdgeSet<int> sublayers[N_SUBLAYERS];
};

struct RegionLayer {
    typedef IdeologyLayer ChildLayer;
    static const int N_SUBLAYERS = N_BIN_REGIONS;

    struct Weights {
        ChildLayer::Weights subweights[N_SUBLAYERS];
        double total_weight = 0;
    };

    static int classify(Agent& agent);

    int n_elems = 0; // Total
    ChildLayer sublayers[N_SUBLAYERS];
};

struct PreferenceClassLayer {
    typedef RegionLayer ChildLayer;
    static const int N_SUBLAYERS = N_BIN_PREFERENCE_CLASS;

    struct Weights {
        ChildLayer::Weights subweights[N_SUBLAYERS];
        double total_weight = 0;
    };

    static int classify(Agent& agent);

    int n_elems = 0; // Total
    ChildLayer sublayers[N_SUBLAYERS];
};

// Top layer
struct LanguageLayer {
    typedef PreferenceClassLayer ChildLayer;
    static const int N_SUBLAYERS = N_LANGS;

    struct Weights {
        ChildLayer::Weights subweights[N_SUBLAYERS];
        double total_weight = 0;

        template <typename Archive>
        void save(Archive& ar) const {
            ar.saveBinary(this, sizeof(*this));
        }
        template <typename Archive>
        void load(Archive& ar) {
            ar.loadBinary(this, sizeof(*this));
        }
    };

    static int classify(Agent& agent);

    int n_elems = 0; // Total
    ChildLayer sublayers[N_SUBLAYERS];
};

/*****************************************************************************
 * Implementation of the follower set:
 *****************************************************************************/

struct FollowerSet {
    const static int MAGIC_CONSTANT_BEFORE_SERIALIZATION = 0xbadbeef;
    typedef LanguageLayer TopLayer;
    typedef TopLayer::Weights Weights;

    // Weights for determining whether a tweet has a reaction (follow/retweet).
    // Note that this is primarily decided by a tweet type, observer preference class,
    // and author's agent type. Note that for TWEET_IDEOLOGICAL, we resolve first whether it should be
    // TWEET_IDEOLOGICAL_DIFFERENT, otherwise tweet ideology is matching.
    // Language layer does not need its own weight determiner layer,
    // it is a hard all-or-nothing cutoff for tweet reaction.
    struct WeightDeterminer {
        double weights[N_BIN_PREFERENCE_CLASS][N_TWEET_TYPES][N_BIN_AGENT_TYPES];
        WeightDeterminer() {
            memset(weights, 0, sizeof(double) * N_BIN_PREFERENCE_CLASS * N_TWEET_TYPES * N_BIN_AGENT_TYPES);
        }

        double get_weight(Agent& author, TweetContent& content);
    };

    template <typename Function>
    void for_each(Function func) {
        // Reach into all the layers:
        auto& a = followers;
        for (auto& b : a.sublayers) {
            for (auto& c : b.sublayers) {
                for (auto& d : c.sublayers) {
                    for (HashedEdgeSet<int>& set : d.sublayers) {
                        HashedEdgeSet<int>::iterator iter;
                        while (set.iterate(iter)) {
                            func(iter.get());
                        }
                    }
                }
            }
        }
    }
    std::vector<int> as_vector() {
        std::vector<int> vec;
        for_each([&](int agent_id) {
            vec.push_back(agent_id);
        });
        return vec;
    }

    /* Returns false if the element already existed */
    bool add(Agent& agent);

    /* Returns true if the element already existed */
    bool remove(Agent& agent);

    /* Returns an element, provided the given weights */
    bool pick_random_weighted(MTwist rng, Weights& weights, int& id_result);

    /* Returns an element, weighing all options equally */
    bool pick_random_uniform(MTwist& rng, int& id);

    void print();

    size_t size() const {
        return followers.n_elems;
    }

    double determine_tweet_weights(Agent& author, TweetContent& content, WeightDeterminer& determiner, /*Weights placed here: */ Weights& output);

    // Do a 'flexible' serialization, upholding semantic meaning but not exact binary compability, allowing for reloading differing configs. 
    // Note that because exact binary compatibility is not held, stopping a network has a reshuffling effect on data.
    // This does not affect the validity of rates, but does mean that serializing a network does not cause it to resume in exactly the same way.
    // Restarting a network will, however, be deterministic if done repeatedly.
    template <typename Archive>
    void save(Archive& ar) const {
        ar( cereal::make_size_tag( (size_t) size() ) );
        size_t n_written = 0; // For debugging
        ((FollowerSet*)this)->for_each([&ar, &n_written](int agent_id){
            ar(agent_id);
            n_written++;
        });
        ASSERT(n_written == size(), "size error in follower set; detected upon serialization");
    }

    template <typename Archive>
    void load(Archive& ar) {
        // Helper for deserializing in two phases, to allow for agent population
        size_t size;
        ar( cereal::make_size_tag( size ) );
        serialization_cache = new std::vector<int>(size);
        for (int& edge : *serialization_cache) {
            ar(edge);
        }
    }

    void post_load(AnalysisState& state);
private:
    // Holds the actual followers:
    TopLayer followers;
    // For serialization:
    std::vector<int>* serialization_cache;
};

#endif /* FOLLOWERSET_H_ */
