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
#include <cmath>
#include "util.h"

#include "util/HashedEdgeSet.h"

// For bin limits:
#include "config_static.h"

// Forward declarations to prevent circular imports:
struct Entity;
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
 *   Entity preference class
 *   X Tweet type (for ideological tweets, whether ideologies match)
 *   X Original tweeter entity type
 *****************************************************************************/

// Leaf layer
struct IdeologyLayer {
    static const int N_SUBLAYERS = N_BIN_IDEOLOGIES;

    struct Weights {
        double weights[N_SUBLAYERS] = {0};
    };

    static int classify(Entity& entity);

    int n_elems = 0; // Total
    HashedEdgeSet<int> sublayers[N_SUBLAYERS];
};

struct RegionLayer {
    typedef IdeologyLayer ChildLayer;
    static const int N_SUBLAYERS = N_BIN_REGIONS;

    struct Weights {
        ChildLayer::Weights subweights[N_SUBLAYERS];
        double weights[N_SUBLAYERS] = {0};
    };

    static int classify(Entity& entity);

    int n_elems = 0; // Total
    ChildLayer sublayers[N_SUBLAYERS];
};

struct PreferenceClassLayer {
    typedef RegionLayer ChildLayer;
    static const int N_SUBLAYERS = N_BIN_PREFERENCE_CLASS;

    struct Weights {
        ChildLayer::Weights subweights[N_SUBLAYERS];
        double weights[N_SUBLAYERS] = {0};
    };

    static int classify(Entity& entity);

    int n_elems = 0; // Total
    ChildLayer sublayers[N_SUBLAYERS];
};

// Top layer
struct LanguageLayer {
    typedef PreferenceClassLayer ChildLayer;
    static const int N_SUBLAYERS = N_LANGS;

    struct Weights {
        ChildLayer::Weights subweights[N_SUBLAYERS];
        double weights[N_SUBLAYERS] = {0};
    };

    static int classify(Entity& entity);

    int n_elems = 0; // Total
    ChildLayer sublayers[N_SUBLAYERS];
};

/*****************************************************************************
 * Implementation of the follower set:
 *****************************************************************************/

struct FollowerSet {
    typedef LanguageLayer TopLayer;
    typedef TopLayer::Weights Weights;

    // Weights for determining whether a tweet has a reaction (follow/retweet).
    // Note that this is primarily decided by a tweet type, observer preference class,
    // and author's entity type. Note that for TWEET_IDEOLOGICAL, we resolve first whether it should be
    // TWEET_IDEOLOGICAL_DIFFERENT, otherwise tweet ideology is matching.
    // Language layer does not need its own weight determiner layer,
    // it is a hard all-or-nothing cutoff for tweet reaction.
    struct WeightDeterminer {
        double weights[N_BIN_PREFERENCE_CLASS][N_TWEET_TYPES][N_BIN_ENTITY_TYPES];
        WeightDeterminer() {
            memset(weights, 0, sizeof(double) * N_BIN_PREFERENCE_CLASS * N_TWEET_TYPES * N_BIN_ENTITY_TYPES);
        }

        double get_weight(Entity& author, TweetContent& content);
    };

    std::vector<int> as_vector();

    /* Returns false if the element already existed */
    bool add(Entity& entity);

    /* Returns true if the element already existed */
    bool remove(Entity& entity);

    /* Returns an element, provided the given weights */
    bool pick_random_weighted(MTwist rng, Weights& weights, int& id);

    /* Returns an element, weighing all options equally */
    bool pick_random_uniform(MTwist& rng, int& id);

    void print();

    size_t size() const {
        return followers.n_elems;
    }

    double determine_tweet_weights(Entity& author, TweetContent& content, WeightDeterminer& determiner, /*Weights placed here: */ Weights& output);

    READ_WRITE(rw) {
        // Reach into all the layers:
        auto& a = followers;
        for (auto& b : a.sublayers) {
            for (auto& c : b.sublayers) {
                for (auto& d : c.sublayers) {
                    for (HashedEdgeSet<int>& set : d.sublayers) {
                        set.visit(rw);
                    }
                    rw << d.n_elems;
                }
                rw << c.n_elems;
            }
            rw << b.n_elems;
        }
        rw << a.n_elems;
    }
private:
    // Holds the actual followers:
    TopLayer followers;
};

#endif /* FOLLOWERSET_H_ */
