/* Network state structures */

#ifndef __NETWORK_H_ //** Header guards are necessary because C++ has a very low-level include syntax.
#define __NETWORK_H_ //** Without them, you can get strange multiple-definition errors due to text duplication.

#include <cstdlib>
#include <vector>
#include <exception>
#include "util.h"

#include "CircularBuffer.h"
#include "MemPoolVector.h"
#include "category.h"


typedef MemPoolVector<FOLLOW_LIST_THRESHOLD1> FollowList;
typedef MemPoolVector<FOLLOWER_LIST_THRESHOLD1> FollowerList;

struct Retweet {
    int original_tweeter;
    double time;
    Retweet() {
    }
    Retweet(int tweet, double time) :
            original_tweeter(tweet), time(time) {
    }
};

// Store the last 'RETWEETS_STORED' tweets, discarding old ones.
typedef CircularBuffer<Retweet, RETWEETS_STORED> RetweetBuffer;

struct Entity {
    int entity;
    int n_tweets, n_retweets;
    double creation_time;
    float x_location, y_location;

    // Store the two directions of the follow relationship
    FollowList follow_set;
    FollowerList follower_set;

    RetweetBuffer retweets;

    Entity() {
        entity = 0;
        creation_time = 0.0;
        x_location = -1, y_location = -1;
        n_tweets = 0;
        n_retweets = 0;
    }
};

struct Network {
    Entity* entities; //** This is a pointer - used to create a dynamic array
    int n_entities;
    Network() {
        entities = NULL;
        n_entities = 0;
    }
    ~Network() { //** This defines how to clean-up our Network object; we free the dynamic array
        free(entities);
    }
    Entity& operator[](int index) { //** This allows us to index our Network struct as if it were an array.
        return entities[index];
    }
    void preallocate(int n) {
        n_entities = n;
        entities = (Entity*)malloc(sizeof(Entity) * n_entities);
        // This is very likely to be a large allocation, check for failures:
        if (entities == NULL) {
            panic("Network::preallocate failed");
        }
        for (int i = 0; i < n_entities; i++) {
            // Placement new due to use of malloc
            new (&entities[i]) Entity();
        }
    }

    // Convenient network queries:
    int n_following(int person_id) {
        return entities[person_id].follow_set.size;
    }
    int n_followers(int person_id) {
        return entities[person_id].follower_set.size;
    }
    int follow_i(int person_id, int follow_index) {
        return entities[person_id].follow_set[follow_index];
    }
    int following_i(int person_id, int follow_index) {
        return entities[person_id].follower_set[follow_index];
    }
};

struct UserType {
    double R_ADD; // When a user is added, how likely is it that it is this user type ?
    double R_FOLLOW; // When a user is followed, how likely is it that it is this user type ?
    std::vector<int> user_list;
};

// different user types
enum {
    UT_NORMAL_INDEX = 0,
    UT_CELEB_INDEX = 1,
    UT_BOT_INDEX = 2,
    UT_ORG_INDEX = 3,
    N_ENTITIES = 4
};

#endif
