#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>
#include <vector>

#include "cat_classes.h"
#include "cat_nodes.h"
#include "CategoryGrouper.h"

#include "lcommon/smartptr.h"
#include "mtwist.h"

#include "events.h"

#include "config_static.h"

#include "entity_properties.h"

#include "tweets.h"

typedef cats::LeafNode<int> FollowSet;

struct AnalysisState;

namespace follower_set {
    struct Context {
        Context(AnalysisState& N, int owner_id) :
                N(N), owner_id(owner_id) {
        }
        AnalysisState& N; // Analysis state
        int owner_id; // Owner of the follower set
    };

    // Change tweets.h -> RETWEET_RATE_ELEMENTS if the categories
    // are rearranged!

    struct PreferenceClassComponent: cats::StaticLeafClass<int, N_BIN_PREFERENCE_CLASS> {
        int classify(Context& N, int entity_id); // Defined in entity.cpp
        std::string cat_name(Context& N, int bin);
    };
    // The distance you are to your followers is a categorization dimension
    struct DistanceComponent: cats::StaticTreeClass<PreferenceClassComponent, N_BIN_DISTANCE> {
        std::string cat_name(Context& N, int bin);
        int classify(Context& N, int entity_id); // Defined in entity.cpp
    };
    struct LanguageComponent: cats::StaticTreeClass<DistanceComponent, N_LANGS> {
        std::string cat_name(Context& N, int bin);
        int classify(Context& N, int entity_id); // Defined in entity.cpp
    };
}

typedef follower_set::PreferenceClassComponent::CatGroup FollowerSetLeafBin;
typedef follower_set::DistanceComponent::CatGroup FollowerSetDistanceLayer;
typedef follower_set::LanguageComponent::CatGroup FollowerSetLanguageLayer;
typedef FollowerSetLeafBin::iterator FollowerSetLeafIter;

typedef follower_set::LanguageComponent FollowerSetRates;
typedef FollowerSetRates::CatGroup FollowerSet;


// Helper to achieve syntactic terseness below
template <typename Layer, typename OuterLayer>
static inline bool for_lang_helper(Layer& layer, int id, OuterLayer& outer) {
    if (id < outer.n_bins()) {
        // Avoid operator-bound problem
        layer = &outer[id];
        return true;
    }
    return false;
}

/* FOR_LANG_LAYER:
    A macro to iterate over the language layer of a follower_set.
    NOTE: 'bin_id', 'layer' are declared by the macro!
 Sample usage:
 FOR_LANG_LAYER(follower_set, bin_id, layer)
       // Do something with the 'layer' pointer
 } */
#define FOR_LANG_LAYER(fset, bin_id, layer) \
    int bin_id = 0; \
    FollowerSetDistanceLayer* layer = NULL; /* Assignment happens in for_lang_helper*/ \
    FollowerSet& outer = fset; /*==fset, just future-proof this way */ \
    for (; for_lang_helper(layer, bin_id, outer); bin_id++)\
        // For loop body goes here

/* FOR_LEAF_BINS:
    A macro to iterate all the nested components of FollowerSet,
    of which there are 3 layers -- Language X Distance X PreferenceClass
    NOTE: 'outer_id', 'outer', 'bin_id' are declared by the macro!
 Sample usage:
 FOR_ALL_BINS(follower_set,
    outer_id, outer, bin_id) {
       FollowerSetLeafBin& leaf = outer[bin_id];
       FollowerSetLeafIter iter;
       while (leafptr->iterate(iter)) {
          int element = iter.get();
          // Do something with element
       }
 } */
#define FOR_LEAF_BINS(fset, dist_id, dist, pref_id) \
    int dist_id = 0, pref_id = 0; \
    for (dist_id = 0; dist_id < fset.n_bins(); dist_id++, \
            /*Reset for loop below:*/ pref_id = 0) \
        for (FollowerSetDistanceLayer& dist = fset[dist_id]; \
                pref_id < dist.n_bins(); pref_id++)
            // For loop body goes here

inline int language_entity_amount(FollowerSet& set, Language language) {
    return set[language].size();
}

inline int language_entity_random(MTwist& rng, FollowerSet& set, Language language) {
    // Required that language_entity_amount > 0
    return set[language].pick_random_uniform(rng);
}

struct Entity {
    int entity_type;
    int preference_class;
    int n_tweets, n_retweets;
    double creation_time;
    Location location;
    Language language;
    int humour_bin;

    // Store the two directions of the follow relationship
    FollowSet follow_set;
    FollowerSet follower_set;

    Entity() {
        entity_type = 0;
        preference_class = 0;
        creation_time = 0.0;
        n_tweets = 0;
        language = (Language)-1; // Initialize with invalid language
        n_retweets = 0;
        humour_bin = -1; // Initialize with invalid humour bin
    }
};

#endif
