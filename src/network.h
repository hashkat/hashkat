/* Network state structures */

#ifndef __NETWORK_H_ //** Header guards are necessary because C++ has a very low-level include syntax.
#define __NETWORK_H_ //** Without them, you can get strange multiple-definition errors due to text duplication.

#include <cstdlib>
#include <vector>
#include <exception>
#include "util.h"

#include "CircularBuffer.h"
#include "MemPoolVector.h"
#include "CategoryGrouper.h"

#include "cat_classes.h"
#include "cat_nodes.h"

typedef cats::LeafNode<int> FollowSet;
typedef cats::LeafNode<int> FollowerSet;

typedef google::sparse_hash_set<int, cats::Hasher> HashSet;

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
    double decay_time;
    //** make a copy of a follow_set, if it's not in use, the size will
    //** ALWAYS be 0, if not we can easily remove entity ID's from this
    //** list and still have our actual follow_set (below) in tact.
    std::vector<int> follower_set_cp;

    // Store the two directions of the follow relationship
    FollowSet follow_set;
    FollowerSet follower_set;

    RetweetBuffer retweets;

    Entity() {
        entity = 0;
        creation_time = 0.0;
        x_location = -1, y_location = -1;
        n_tweets = 0;
        n_retweets = 0;
        decay_time = 5; // 5 minutes
    }
};

struct Network {
    Entity* entities; //** This is a pointer - used to create a dynamic array
    int n_entities, max_entities;
    std::vector<int> recent_tweet_ID;
    std::vector<double> recent_tweet_times;
    std::vector<int> recent_retweet_ID;
    std::vector<double> recent_retweet_times;
    std::vector<double> decay_rates;
    Network() {
        entities = NULL;
        n_entities = 0;
        max_entities = 0;
    }
    ~Network() { //** This defines how to clean-up our Network object; we free the dynamic array
        free(entities);
    }
    Entity& operator[](int index) { //** This allows us to index our Network struct as if it were an array.
        DEBUG_CHECK(index >= 0 && index <= n_entities, "Network out-of-bounds entity access");
        return entities[index];
    }

    void preallocate(int n) {
        max_entities = n;
        entities = (Entity*)malloc(sizeof(Entity) * max_entities);
        // This is very likely to be a large allocation, check for failures:
        if (entities == NULL) {
            error_exit("Network::preallocate failed");
        }
        for (int i = 0; i < max_entities; i++) {
            // Placement new due to use of malloc
            new (&entities[i]) Entity();
        }
    }

    void perform_cleanup() {
        // TODO: Clean-up is no longer required as follow-sets do not permit duplicates anymore
    }

    // Convenient network queries:
    FollowSet& follow_set(int id) {
        return entities[id].follow_set;
    }
    FollowerSet& follower_set(int id) {
        return entities[id].follower_set;
    }

    size_t n_following(int id) {
        return follow_set(id).size();
    }
    size_t n_followers(int id) {
        return follower_set(id).size();
    }
};

// 0 - add, 1 - follow, 2 - tweet, 3 - retweet
const int number_of_diff_events = 3;

struct Rate_Function {
	std::string function_type;
	double slope, y_intercept, const_val, amplitude, exp_factor;
	std::vector<double> monthly_rates;
	Rate_Function() {
		slope = y_intercept = const_val = amplitude = exp_factor = -1;
		function_type = "not specified";
	}
};

struct EntityType {
    std::string name;
    double prob_add; // When a entity is added, how likely is it that it is this entity type ?
    double prob_follow; // When a entity is followed, how likely is it that it is this entity type ?
	double prob_followback;
	int new_entities; // the number of new users in this entity type
	int n_tweets, n_follows, n_followers, n_retweets;
	// 0 - add, 1 - follow, 2 - tweet, 3 - retweet
	int number_of_events;
	Rate_Function RF[number_of_diff_events];
	
	// number of entities for each discrete value of the rate(time)
	std::vector<int> entity_cap;
    // list of entity ID's
	std::vector<int> entity_list;
	// categorize the entities by age
	CategoryGrouper age_ranks;

    EntityType() {
        n_tweets = n_follows = n_followers = n_retweets = 0;
        prob_add = prob_follow = prob_followback = 0;
		number_of_events = 3;
	}
};

typedef std::vector<EntityType> EntityTypeVector;

#endif
