#ifndef ENTITY_CHARACTERISTICS_H_
#define ENTITY_CHARACTERISTICS_H_

#include <cmath>
#include <algorithm>
#include "util.h"

#include "config_static.h"

/* This file contains properties that differentiate tweets and entities */

// An X-Macro, allowing for syntatic construction via defining X before resolution
#define X_ALL_LANGUAGES \
    X(LANG_ENGLISH)\
    X(LANG_FRENCH_AND_ENGLISH)\
    X(LANG_FRENCH)
/* All languages before LANG_FRENCH contain English */

// Helper for location distance:
inline double wrap_dist(double d1, double d2) {
    double distA = fabs(d2 - d1);
    double distB = fabs(d2 - d1 + 1);
    double distC = fabs(d2 - d1 - 1);
    return std::min(distA, std::min(distB, distC));
}

/* Represents a Location object that wraps around in (x = 0 to 1) X (y = 0 to 1) space*/
struct Location {
    double x, y;
    Location() {
        x = 0, y = 0;
    }
    double distance(const Location& o) {
        double dx = wrap_dist(x, o.x);
        double dy = wrap_dist(y, o.y);
        double d= sqrt(dx*dx + dy*dy);
        return d;
    }
    int distance_bin(const Location& o) {
        // We normalize to 0..1 by multiplying by 2, as
        // 0.5 is the maximum distance with modular coordinates.
        int bin = (int) (distance(o) * 2.0 * N_BIN_DISTANCE);
        DEBUG_CHECK(bin >= 0, "Cannot have negative bin!");
        bin = std::min(bin, N_BIN_DISTANCE - 1);
        return bin;
    }

    VISIT0(rw) {
        rw << x << y;
    }
};

#define X(x) x, /* Resolve for enum */
enum Language {
    // Resolves using X above, fills enum:
    X_ALL_LANGUAGES
    N_LANGS // Not a real entry; evaluates to amount of languages
};
#undef X


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

    VISIT0(rw) {
        rw << name << prob_add << prob_follow << prob_followback;
        rw << new_entities << n_tweets << n_follows << n_followers << n_retweets;
        for (int i = 0; i < number_of_diff_events; i++){
            rw << RF[i];
        }
        rw << entity_cap << entity_list;
        age_ranks.visit(rw);
        follow_ranks.visit(rw);
        rw << updating_probs << event_stats.stats_array;
    }
};

// 'EntityTypeVector' acts exactly as a 'vector'
typedef std::vector<EntityType> EntityTypeVector;

#endif
