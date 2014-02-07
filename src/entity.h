#ifndef ENTITY_H_
#define ENTITY_H_

#include <vector>

#include "cat_classes.h"
#include "cat_nodes.h"
#include "CategoryGrouper.h"

#include "dependencies/mtwist.h"
#include "events.h"

enum Language {
    LANG_ENGLISH,
    LANG_FRENCH,
    LANG_FRENCH_AND_ENGLISH,
    N_LANGS // Not a real entry; evaluates to amount of languages
};

struct LanguageProbabilities {
    double& operator[](int index) {
        DEBUG_CHECK(index >= 0 && index < N_LANGS, "Out of bounds!");
        return probs[index];
    }
    LanguageProbabilities() {
        memset(probs, 0, sizeof(double) * N_LANGS);
    }
    Language kmc_select(MTwist& rng) {
        return (Language)rng.kmc_select(probs, N_LANGS);
    }
private:
    double probs[N_LANGS];
};

typedef cats::LeafNode<int> FollowSet;

struct AnalysisState;

namespace follower_set {
    struct LanguageClass: cats::StaticLeafClass<int, N_LANGS> {
        int classify(AnalysisState& N, int entity_id); // Defined in entity.cpp
    };
}

typedef follower_set::LanguageClass FollowerSetRates;
typedef FollowerSetRates::CatGroup FollowerSet;

struct Entity {
    int entity_type;
    int n_tweets, n_retweets;
    double creation_time;
    float x_location, y_location;
    double decay_time;
    Language language;
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
        language = (Language)-1; // Initialize with invalid language
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
