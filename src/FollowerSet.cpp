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

#include "FollowerSet.h"

#include "lcommon/typename.h"
#include "lcommon/perf_timer.h"

#include "entity.h"

using namespace std;

/*****************************************************************************
 * Layer implementations:
 *****************************************************************************/

typedef IdeologyLayer LeafLayer;
int IdeologyLayer::classify(Entity& entity) {
    return entity.ideology_bin;
}

int PreferenceClassLayer::classify(Entity& entity) {
    return entity.preference_class;
}

int RegionLayer::classify(Entity& entity) {
    return entity.region_bin;
}

int LanguageLayer::classify(Entity& entity) {
    return entity.language;
}

/*****************************************************************************
 * as_vector implementation:
 *****************************************************************************/

vector<int> FollowerSet::as_vector() {
    vector<int> ret;

    // Reach into all the layers:
    auto& a = followers;
    for (auto& b : a.sublayers) {
        for (auto& c : b.sublayers) {
            for (auto& d : c.sublayers) {
                for (HashedEdgeSet<int>& set : d.sublayers) {
                    HashedEdgeSet<int>::iterator iter;
                    while (set.iterate(iter)) {
                        ret.push_back(iter.get());
                    }
                }
            }
        }
    }

    return ret;
}

/*****************************************************************************
 * add implementation:
 * The leaf layer inserts to a HashedEdgeSet, while the parent layers
 * delegate insertion to child layers.
 *
 *****************************************************************************/

// Provides additional debug checking:
template <typename Layer>
static int classify(Layer& layer, Entity& entity) {
    int bin = layer.classify(entity);
    DEBUG_CHECK(bin >= 0 && bin < Layer::N_SUBLAYERS, "Logic error!");
    return bin;
}

// Leaf layer specialization
static bool add_follower(LeafLayer& layer, Entity& entity) {
    auto& sub = layer.sublayers[classify(layer, entity)];
    if (sub.insert(entity.id)) {
        layer.n_elems++;
        return true;
    }
    return false;
}

// Parent layers template
template <typename Layer>
static bool add_follower(Layer& layer, Entity& entity) {
    auto& sub = layer.sublayers[classify(layer, entity)];
     if (add_follower(sub, entity)) {
         layer.n_elems++;
         return true;
     }
     return false;
}

bool FollowerSet::add(Entity& entity) {
    return add_follower(followers, entity);
}

/*****************************************************************************
 * remove implementation:
 * The leaf layer removes from a HashedEdgeSet, while the parent layers
 * delegate removal to child layers.
 *****************************************************************************/

// Leaf layer specialization
static bool remove_follower(LeafLayer& layer, Entity& entity) {
    auto& sub = layer.sublayers[layer.classify(entity)];
    if (sub.erase(entity.id)) {
        layer.n_elems--;
        return true;
    }
    return false;
}

// Parent layers template
template <typename Layer>
static bool remove_follower(Layer& layer, Entity& entity) {
    auto& sub = layer.sublayers[layer.classify(entity)];
    if (remove_follower(sub, entity)) {
        layer.n_elems--;
        return true;
    }
    return false;
}

bool FollowerSet::remove(Entity& entity) {
    return remove_follower(followers, entity);
}

/*****************************************************************************
 * pick_random_weighted implementation:
 *****************************************************************************/

// Leaf layer specialization
static bool pick_weighted(MTwist& rng, LeafLayer& layer, LeafLayer::Weights& weights, int& id) {
    int bin = rng.kmc_select(weights.weights, LeafLayer::N_SUBLAYERS);
    return layer.sublayers[bin].pick_random_uniform(rng, id);
}

// Parent layers template
template <typename Layer>
static bool pick_weighted(MTwist& rng, Layer& layer, typename Layer::Weights& weights, int& id) {
    int bin = rng.kmc_select(weights.weights, Layer::N_SUBLAYERS);
    return pick_weighted(rng, layer.sublayers[bin], weights.subweights[bin], id);
}

bool FollowerSet::pick_random_weighted(MTwist rng, Weights& weights, int& id) {
    return pick_weighted(rng, followers, weights, id);
}

/*****************************************************************************
 * pick_random_uniform implementation:
 *****************************************************************************/

// Leaf layer specialization
static bool pick_uniform(MTwist& rng, LeafLayer& layer, int& id) {
    int R = rng.rand_int(layer.n_elems);
    for (auto& sublayer : layer.sublayers) {
        R -= sublayer.size();
        if (R < 0) {
            return sublayer.pick_random_uniform(rng, id);
        }
    }
    return false;
}

// Parent layers template
template <typename Layer>
static bool pick_uniform(MTwist& rng, Layer& layer, int& id) {
    int R = rng.rand_int(layer.n_elems);
    for (auto& sublayer : layer.sublayers) {
        R -= sublayer.n_elems;
        if (R < 0) {
            return pick_uniform(rng, sublayer, id);
        }
    }
    return false;
}

bool FollowerSet::pick_random_uniform(MTwist& rng, int& id) {
    return pick_uniform(rng, followers, id);
}

/*****************************************************************************
 * print implementation:
 *****************************************************************************/

// Leaf layer specialization
static void print_layer(LeafLayer& layer, int depth) {
    for (int i = 0; i < LeafLayer::N_SUBLAYERS; i++) {
        string repr = format("%s (Bin %d)", cpp_type_name(layer).c_str(), i);
        auto& sublayer = layer.sublayers[i];
        if (sublayer.size() > 0) {
            for (int i = 0; i < depth; i++) {
                printf("  ");
            }
            printf("[%s] (N_elems %d)\n", repr.c_str(), (int) sublayer.size());
            for (int i = 0; i < depth + 1; i++) {
                printf("  ");
            }
            sublayer.print();
        }
    }
}

// Parent layers template
template <typename Layer>
static void print_layer(Layer& layer, int depth) {
    for (int i = 0; i < Layer::N_SUBLAYERS; i++) {
        string repr = format("%s (Bin %d)", cpp_type_name(layer).c_str(), i);
        auto& sublayer = layer.sublayers[i];
        if (sublayer.n_elems > 0) {
            for (int i = 0; i < depth; i++) {
                printf("  ");
            }
            printf("[%s] (N_elems %d)\n", repr.c_str(), sublayer.n_elems);
            print_layer(sublayer, depth + 1);
        }
    }
}

void FollowerSet::print() {
    print_layer(followers, 0);
}

double FollowerSet::determine_tweet_weights(Entity& author, TweetContent& content, WeightDeterminer& d_root, /*Weights placed here:*/ Weights& w_root) {
    PERF_TIMER();
    // Weights are assumed to start 0-initialized.

    auto& f_root = followers;

    DEBUG_CHECK(content.language != LANG_FRENCH_AND_ENGLISH, "Invalid tweet language!");

    double total = 0;
    // Language spoken:
    for (int lang = 0; lang < N_LANGS; lang++) {

        if (!language_understandable((Language)lang, content.language)) {
            break;
        }
        double incr0 = 0;
        auto& f_prefs = f_root.sublayers[lang];
        for (int i_pref = 0; i_pref < N_BIN_PREFERENCE_CLASS; i_pref++) {
            auto& f_regions = f_prefs.sublayers[i_pref];
            auto& w_regions = w_root.subweights[i_pref];

            double incr1 = 0;
            for (int i_region = 0; i_region < N_BIN_REGIONS; i_region++) {
                auto& f_bins = f_regions.sublayers[i_region];
                auto& w_bins = w_regions.subweights[i_region];
                double incr2 = 0;
                for (int i_ideo = 0; i_ideo < N_BIN_IDEOLOGIES; i_ideo++) {
                    auto& f_leaf = f_bins.sublayers[i_ideo];
                    auto& w_leaf = w_bins.subweights[i_ideo];
                    TweetType type = content.type;

                    if (type == TWEET_IDEOLOGICAL && i_ideo == content.ideology_bin) {
                        type = TWEET_IDEOLOGICAL_DIFFERENT;
                    }
                    double weight = d_root.weights[i_pref][type][author.entity_type];

                    double incr3 = weight * f_leaf.size();
                    w_leaf.weights[i_ideo] += incr3;
                    incr2 += incr3;
                }
                w_bins.weights[i_region] += incr2;
                incr1 += incr2;
            }
            w_regions.weights[i_pref] += incr1;
            incr0 += incr1;
        }

        w_root.weights[lang] += incr0;
        total += incr0;
    }

    return total;
}
