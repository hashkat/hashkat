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

typedef MemPoolVector<int, FOLLOW_LIST_THRESHOLD1> FollowList;
typedef MemPoolVector<int, FOLLOWER_LIST_THRESHOLD1> FollowerList;

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
    int n_entities, max_entities;
    Network() {
        entities = NULL;
        n_entities = 0;
        max_entities = 0;
    }
    ~Network() { //** This defines how to clean-up our Network object; we free the dynamic array
        free(entities);
    }
    Entity& operator[](int index) { //** This allows us to index our Network struct as if it were an array.
        return entities[index];
    }

    void preallocate(int n) {
        max_entities = n;
        entities = (Entity*)malloc(sizeof(Entity) * max_entities);
        // This is very likely to be a large allocation, check for failures:
        if (entities == NULL) {
            panic("Network::preallocate failed");
        }
        for (int i = 0; i < max_entities; i++) {
            // Placement new due to use of malloc
            new (&entities[i]) Entity();
        }
    }

    // Cleanup method. Remove duplicates from everyone's follow lists.
    void perform_cleanup() {
        for (int i = 0 ; i < max_entities; i++) {
            Entity& e = entities[i];
            e.follow_set.sort_and_remove_duplicates();
            e.follower_set.sort_and_remove_duplicates();
        }
    }

    // Convenient network queries:
    int n_following(int id) {
        return entities[id].follow_set.size;
    }
    int n_followers(int id) {
        return entities[id].follower_set.size;
    }
    int follow_i(int id, int follow_index) {
        return entities[id].follow_set[follow_index];
    }
    int following_i(int id, int follow_index) {
        return entities[id].follower_set[follow_index];
    }
};

struct EntityType {
    std::string name;
    double prob_add; // When a entity is added, how likely is it that it is this entity type ?
    double prob_follow; // When a entity is followed, how likely is it that it is this entity type ?
	double prob_followback;
	int new_entities; // the number of new users in this entity type
	
	// changing rates for each user with time
	std::vector<double> r_follow;
	std::vector<double> r_tweet;
	std::vector<double> r_retweet;
	// number of entities for each discrete value of the rate(time)
	std::vector<int> entity_cap;
    // list of entity ID's
	std::vector<int> entity_list;
	// categorize the entities by age
	CategoryGrouper age_ranks;
	
    EntityType() {
        prob_add = prob_follow = prob_followback = 0;
    }
};

typedef std::vector<EntityType> EntityTypeVector;

#endif
