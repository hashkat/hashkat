#include "FollowerSet.h"

#include "lcommon/typename.h"

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
                for (FlexibleSet<int>& set : d.sublayers) {
                    FlexibleSet<int>::iterator iter;
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
 * The leaf layer inserts to a FlexibleSet, while the parent layers
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
 * The leaf layer removes from a FlexibleSet, while the parent layers
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
            return pick_uniform(rng, layer, id);
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
            printf("[%s] (N_elems %d)\n", repr.c_str(), sublayer.size());
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

void determine_tweet_weights(FollowerSet& set, FollowerSet::Weights& weights, Tweet& tweet) {
//
//    // Reach into all the layers:
//    auto& a = followers;
//    for (auto& b : a.sublayers) {
//        for (auto& c : b.sublayers) {
//            for (auto& d : c.sublayers) {
//                for (FlexibleSet<int>& set : d.sublayers) {
//                    FlexibleSet<int>::iterator iter;
//                    while (set.iterate(iter)) {
//                        ret.push_back(iter.get());
//                    }
//                }
//            }
//        }
//    }
}

void FollowerSet::determine_tweet_weights(WeightDeterminer& determiner, Tweet& tweet, /*Weights placed here:*/ Weights& output) {
    for (double& w : output.weights) { w = 1.0; }
    for (auto& s1 : output.subweights) {
        for (double& w : s1.weights) { w = 1.0; }
        for (auto& s2 : s1.subweights) {
            for (double& w : s2.weights) { w = 1.0; }
            for (auto& s3 : s2.subweights) {
                for (double& w : s3.weights) { w = 1.0; }
            }
        }
    }
}
