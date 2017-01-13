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

#include <cmath>

#include "FollowerSet.h"

#include "lcommon/typename.h"
#include "lcommon/perf_timer.h"

#include "analyzer.h"
#include "agent.h"

using namespace std;

/*****************************************************************************
 * Layer implementations:
 *****************************************************************************/

typedef IdeologyLayer LeafLayer;
int IdeologyLayer::classify(Agent& agent) {
    return agent.ideology_bin;
}

int PreferenceClassLayer::classify(Agent& agent) {
    return agent.preference_class;
}

int RegionLayer::classify(Agent& agent) {
    return agent.region_bin;
}

int LanguageLayer::classify(Agent& agent) {
    return agent.language;
}

/*****************************************************************************
 * add implementation:
 * The leaf layer inserts to a HashedEdgeSet, while the parent layers
 * delegate insertion to child layers.
 *
 *****************************************************************************/

// Provides additional debug checking:
template <typename Layer>
static int classify(Layer& layer, Agent& agent) {
    int bin = layer.classify(agent);
    DEBUG_CHECK(bin >= 0 && bin < Layer::N_SUBLAYERS, "Logic error!");
    return bin;
}

// Leaf layer specialization
static bool add_follower(LeafLayer& layer, Agent& agent) {
    auto& sub = layer.sublayers[classify(layer, agent)];
    if (sub.insert(agent.id)) {
        layer.n_elems++;
        return true;
    }
    return false;
}

// Parent layers template
template <typename Layer>
static bool add_follower(Layer& layer, Agent& agent) {
    auto& sub = layer.sublayers[classify(layer, agent)];
     if (add_follower(sub, agent)) {
         layer.n_elems++;
         return true;
     }
     return false;
}

bool FollowerSet::add(Agent& agent) {
    return add_follower(followers, agent);
}

/*****************************************************************************
 * remove implementation:
 * The leaf layer removes from a HashedEdgeSet, while the parent layers
 * delegate removal to child layers.
 *****************************************************************************/

// Leaf layer specialization
static bool remove_follower(LeafLayer& layer, Agent& agent) {
    auto& sub = layer.sublayers[layer.classify(agent)];
    if (sub.erase(agent.id)) {
        layer.n_elems--;
        return true;
    }
    return false;
}

// Parent layers template
template <typename Layer>
static bool remove_follower(Layer& layer, Agent& agent) {
    auto& sub = layer.sublayers[layer.classify(agent)];
    if (remove_follower(sub, agent)) {
        layer.n_elems--;
        return true;
    }
    return false;
}

bool FollowerSet::remove(Agent& agent) {
    return remove_follower(followers, agent);
}

/*****************************************************************************
 * pick_random_weighted implementation:
 *****************************************************************************/

// Leaf layer specialization
static bool pick_weighted(MTwist& rng, LeafLayer& layer, LeafLayer::Weights& weights, int& id) {
    int bin = rng.kmc_select(weights.weights, LeafLayer::N_SUBLAYERS, weights.total_weight);
    bool picked_valid = layer.sublayers[bin].pick_random_uniform(rng, id);
    bool had_non0 = false;
    for (double w : weights.weights) {
        if (w != 0) had_non0 = true;
    }
    ASSERT(!(!picked_valid && had_non0), "If weight was not 0, should not pick empty!");
    return picked_valid;
}

// Parent layers template
template <typename Layer>
static bool pick_weighted(MTwist& rng, Layer& layer, typename Layer::Weights& weights, int& id_result) {
    auto* subweight = rng.general_kmc_select(weights.subweights, Layer::N_SUBLAYERS, weights.total_weight,
        [](typename Layer::ChildLayer::Weights& subweights) {
            return subweights.total_weight;
    });
    ASSERT(subweight->total_weight > 0, "Picked a 0 weight bin!");
    int layer_index = (subweight - weights.subweights);
    return pick_weighted(rng, layer.sublayers[layer_index], *subweight, id_result);
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

void FollowerSet::post_load(AnalysisState& state) {
    ASSERT(serialization_cache != NULL, "No serialization cache (from serialize)!");
    for (int agent_id : *serialization_cache) {
        add(state.network[agent_id]);
    }
    delete serialization_cache;
    serialization_cache = NULL;
}


// Reproduces github issue 109.
// Does the total weight for a subtree of the retweet rates check out?
// Leaf layer specialization
static void assert_weight_integrity(LeafLayer& layer, LeafLayer::Weights& weights) {
    double total_weight = 0;
    for (double weight : weights.weights) {
        total_weight += weight;
    }
    if (total_weight > 0) {
        ASSERT(layer.n_elems > 0, "Should not have weight where we do not have elements!");
    }
    ASSERT(fabs(weights.total_weight - total_weight) <= ZEROTOL, "Weight integrity failed!");
}

// Reproduces github issue 109.
// Does the total weight for a subtree of the retweet rates check out?
// Parent layer
template <typename Layer, typename Weights>
static void assert_weight_integrity(Layer& layer, Weights& weights) {
    // These checks are somewhat expensive, only enable in debug mode:
#ifndef NDEBUG
    double total_weight = 0;
    int i = 0;
    for (auto& subweight : weights.subweights) {
        assert_weight_integrity(layer.sublayers[i], subweight);
        total_weight += subweight.total_weight;
        i++;
    }
    ASSERT(fabs(weights.total_weight - total_weight) <= ZEROTOL, "Weight integrity failed!");
    if (total_weight > 0) {
        ASSERT(layer.n_elems > 0, "Should not have weight where we do not have elements!");
    }
#endif
}

double FollowerSet::determine_tweet_weights(Agent& author, TweetContent& content, WeightDeterminer& d_root, /*Weights placed here:*/ Weights& w_root) {
    PERF_TIMER();
    // Weights are assumed to start 0-initialized.

    auto& f_root = followers;

    DEBUG_CHECK(content.language != LANG_FRENCH_AND_ENGLISH, "Invalid tweet language!");

    if (followers.n_elems == 0) {
        return 0;
    }
    // Sum over retweet weights of all the language layers and the sublayers contained within

    /* Start language weight sum calculation */
    double total_lang_weight_sum = 0;
    // Iterate over all possible spoken languages:
    for (int i_lang = 0; i_lang < N_LANGS; i_lang++) {
        if (!language_understandable((Language)i_lang, content.language)) {
            continue;
        }
        /* Start preference class weight sum calculation */
        double pref_class_weight_sum = 0;
        auto& f_prefs = f_root.sublayers[i_lang];
        auto& w_prefs = w_root.subweights[i_lang];
        // Iterate all the preference class layers:
        for (int i_pref = 0; i_pref < N_BIN_PREFERENCE_CLASS; i_pref++) {
            auto& f_regions = f_prefs.sublayers[i_pref];
            auto& w_regions = w_prefs.subweights[i_pref];
            /* Start region weight sum calculation */
            double region_weight_sum = 0;
            // Iterate all the region layers:
            for (int i_region = 0; i_region < N_BIN_REGIONS; i_region++) {
                auto& f_ideo = f_regions.sublayers[i_region];
                auto& w_ideo = w_regions.subweights[i_region];
                /* Start leaf weight sum calculation */
                double leaf_ideo_weight_sum = 0;
                // Iterate all the leaf ideology layers:
                for (int i_ideo = 0; i_ideo < N_BIN_IDEOLOGIES; i_ideo++) {
                    TweetType type = content.type;
                    if (type == TWEET_IDEOLOGICAL && i_ideo == content.ideology_bin) {
                        type = TWEET_IDEOLOGICAL_DIFFERENT;
                    }
                    // Set the weight in the final layer:
                    double leaf_weight = d_root.weights[i_ideo][type][author.agent_type] * f_ideo.sublayers[i_ideo].size();
                    leaf_ideo_weight_sum += (w_ideo.weights[i_ideo] = leaf_weight);
                }
                region_weight_sum += (w_ideo.total_weight = leaf_ideo_weight_sum);
                /* End leaf  ideology weight sum calculation */
                assert_weight_integrity(f_ideo, w_ideo);
            }
            pref_class_weight_sum += (w_regions.total_weight = region_weight_sum);
            /* End region weight sum calculation */
            assert_weight_integrity(f_regions, w_regions);
        }
        total_lang_weight_sum += (w_prefs.total_weight = pref_class_weight_sum);
        /* End preference class weight sum calculation */
        assert_weight_integrity(f_prefs, w_prefs);
    }
    /* End language weight sum calculation */
    w_root.total_weight = total_lang_weight_sum;
    assert_weight_integrity(f_root, w_root);
    return total_lang_weight_sum;
}
