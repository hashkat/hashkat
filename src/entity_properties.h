#ifndef ENTITY_CHARACTERISTICS_H_
#define ENTITY_CHARACTERISTICS_H_

#include <cmath>
#include <algorithm>
#include "util.h"

#include "config_static.h"

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
    double tweet_type_probs[N_TWEET_TYPES];

    EntityEventStats event_stats;

    EntityType() {
        new_entities = 0;
        n_tweets = n_follows = n_followers = n_retweets = 0;
        prob_add = prob_follow = prob_followback = 0;
    }

    READ_WRITE(rw) {
        rw << name << prob_add << prob_follow << prob_followback;
        rw << new_entities << n_tweets << n_follows << n_followers << n_retweets;
        for (auto& rf : RF) {
            rf.visit(rw);
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
