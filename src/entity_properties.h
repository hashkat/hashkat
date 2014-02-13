#ifndef ENTITY_CHARACTERISTICS_H_
#define ENTITY_CHARACTERISTICS_H_

#include "util.h"

/* This file contains properties that differentiate tweets and entities */

/*
 * An entity type is a static class of entity, determined at creation.
 * Entity types are intended to represent different kinds of network participants,
 *
 * For a network like Twitter, example entity types include 'Standard User', 'Celebrity', etc.
 */

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
    CategoryGrouper follow_ranks;
    std::vector<double> updating_probs;

    EntityEventStats event_stats;

    EntityType() {
        new_entities = 0;
        n_tweets = n_follows = n_followers = n_retweets = 0;
        prob_add = prob_follow = prob_followback = 0;
    }
};

typedef std::vector<EntityType> EntityTypeVector;

// 'EntityTypeVector' acts exactly as a 'vector'
typedef std::vector<EntityType> EntityTypeVector;

enum Language {
    LANG_ENGLISH,
    LANG_FRENCH_AND_ENGLISH,
    LANG_FRENCH, // All languages before LANG_FRENCH contain English
    N_LANGS // Not a real entry; evaluates to amount of languages
};

inline bool contains_english(Language lang) {
    return (lang < LANG_FRENCH); // All languages before LANG_FRENCH contain English
}

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

#endif
