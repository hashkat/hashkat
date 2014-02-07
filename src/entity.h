#ifndef ENTITY_H_
#define ENTITY_H_

#include "cat_classes.h"
#include "cat_nodes.h"

#include "events.h"

typedef cats::LeafNode<int> FollowSet;

//namespace follower_set {
//    struct EntityTypeClass: cats::LeafClass<int> {
//        int classify(AnalysisState& N, int entity_id) {
//            return N[elem].entity_type;
//        }
//    };
//
//    // Also try with just TreeClass
//    struct TimeClass: cats::StaticTreeClass<EntityTypeClass, /*Max bins*/ 2> {
//        TimeClass(double res) {
//            resolution = res;
//        }
//        int classify(MockNetwork& N, int elem) {
//            return int(N[elem].creation_time / resolution);
//        }
//        double resolution; // At what resolution to bin
//    };
//}


typedef cats::LeafNode<int> FollowerSet;

enum Language {
    LANG_ENGLISH,
    LANG_FRENCH
};

struct EntityLiteracy {
    bool uses_english; // Always true for English speakers
    Language main_language;
    EntityLiteracy() {
        uses_english = false;
        main_language = (Language)-1; // Initialize with invalid language
    }
};

struct Entity {
    int entity_type;
    int n_tweets, n_retweets;
    double creation_time;
    float x_location, y_location;
    double decay_time;
    EntityLiteracy literacy;
    //** make a copy of a follow_set, if it's not in use, the size will
    //** ALWAYS be 0, if not we can easily remove entity ID's from this
    //** list and still have our actual follow_set (below) in tact.
    UsedEntities usedentities; //** AD: UsedEntities Moved to events.h

    // Store the two directions of the follow relationship
    FollowSet follow_set;
    FollowerSet follower_set;

    Entity() {
        entity_type = 0;
        creation_time = 0.0;
        x_location = -1, y_location = -1;
        n_tweets = 0;
        n_retweets = 0;
        decay_time = 5; // 5 minutes
    }
};

struct EntityType {
    std::string name;
    double prob_add; // When a entity is added, how likely is it that it is this entity type ?
    double prob_follow; // When a entity is followed, how likely is it that it is this entity type ?
    double prob_followback;
    int new_entities; // the number of new users in this entity type
    int n_tweets, n_follows, n_followers, n_retweets;
    Rate_Function RF[number_of_diff_events];

    // number of entities for each discrete value of the rate(time)
    std::vector<int> entity_cap;
    // list of entity ID's
    std::vector<int> entity_list;
    // categorize the entities by age
    CategoryGrouper age_ranks;

    EntityEventStats event_stats;

    EntityType() {
        new_entities = 0;
        n_tweets = n_follows = n_followers = n_retweets = 0;
        prob_add = prob_follow = prob_followback = 0;
    }
};

typedef std::vector<EntityType> EntityTypeVector;

#endif
