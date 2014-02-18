#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>
#include <vector>

#include "cat_classes.h"
#include "cat_nodes.h"
#include "CategoryGrouper.h"

#include "dependencies/mtwist.h"
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
    struct PreferenceClassComponent: cats::StaticLeafClass<int, N_BIN_PREFERENCE_CLASS> {
        int classify(Context& N, int entity_id); // Defined in entity.cpp
        std::string cat_name(Context& N, int bin);
        template <typename AnyT>
        double get(AnyT& notused, int bin) {
            return 1; // TODO: Not used
        }
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

typedef follower_set::LanguageComponent FollowerSetRates;
typedef FollowerSetRates::CatGroup FollowerSet;

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

    // Store the two directions of the follow relationship
    FollowSet follow_set;
    FollowerSet follower_set;
    
    //info needed for retweeting
    OriginalTweet tweet_info;
    Tweet retweet_info;

    Entity() {
        entity_type = 0;
        preference_class = 0;
        creation_time = 0.0;
        n_tweets = 0;
        language = (Language)-1; // Initialize with invalid language
        n_retweets = 0;
    }
};

#endif
