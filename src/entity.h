#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>
#include <vector>

#include "CategoryGrouper.h"

#include "lcommon/smartptr.h"
#include "mtwist.h"

#include "events.h"

#include "entity_follow_sets.h"
#include "entity_properties.h"

#include "tweets.h"

// Forward declare, to prevent circular header inclusion:
struct AnalysisState;

struct Entity {
    int entity_type;
    int preference_class;
    int n_tweets, n_retweets;
    double creation_time;
    Location location;
    Language language;
    int humour_bin;

    // Store the two directions of the follow relationship
    FollowingSet following_set;
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
private:
    // Ban the copy constructor:
    Entity(const Entity&);
};

#endif
