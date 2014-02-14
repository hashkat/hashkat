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

typedef cats::LeafNode<int> FollowSet;

struct AnalysisState;

namespace follower_set {
    struct LanguageComponent: cats::StaticLeafClass<int, N_LANGS> {
        int classify(AnalysisState& N, int entity_id); // Defined in entity.cpp
        template <typename AnyT>
        double get(AnyT& notused, int bin) {
            return 1; // TODO: Not used
        }
    };
    struct PreferenceClassComponent: cats::StaticTreeClass<LanguageComponent, MAX_PREFERENCE_CLASSES> {
        int classify(AnalysisState& N, int entity_id); // Defined in entity.cpp
    };
}

typedef follower_set::PreferenceClassComponent FollowerSetRates;
typedef FollowerSetRates::CatGroup FollowerSet;

// information for when a user tweets
struct TweetInfo {
    double starting_rate, updating_rate;
    int entity_ID;
    double time_of_tweet;
    UsedEntities used_entities;
    TweetInfo() {
        starting_rate = updating_rate = 0;
        entity_ID = -1;
    }
};

// information for when a user retweets
struct RetweetInfo {
    double starting_rate, updating_rate;
    int entity_ID;
    double time_of_retweet;
    RetweetInfo() {
        starting_rate = updating_rate = 0;
        entity_ID = -1;
    }
};


typedef std::vector<TweetInfo> TweetList;
typedef std::vector<RetweetInfo> RetweetList;

struct Entity {
    int entity_type;
    int preference_class;
    int n_tweets, n_retweets;
    double creation_time;
    float x_location, y_location;
    Language language;

    // Store the two directions of the follow relationship
    FollowSet follow_set;
    FollowerSet follower_set;
    
    //info needed for retweeting
    TweetInfo tweet_info;
    RetweetInfo retweet_info;

    Entity() {
        entity_type = 0;
        preference_class = 0;
        creation_time = 0.0;
        x_location = -1, y_location = -1;
        n_tweets = 0;
        language = (Language)-1; // Initialize with invalid language
        n_retweets = 0;
    }
};

#endif
