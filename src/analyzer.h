#ifndef __ANALYZE_H_
#define __ANALYZE_H_

#include <string>
#include <map>
#include <vector>

#include <lcommon/Timer.h>

#include "dependencies/mtwist.h"

#include "config_dynamic.h"
#include "network.h"

#include "DataReadWrite.h"
#include "TweetBank.h"

extern volatile int SIGNAL_ATTEMPTS;

struct EntityStats {
    int64 n_steps = 0, n_follows = 0, n_tweets = 0, n_retweets = 0, n_unfollows = 0;
    int64 n_random_follows = 0, n_preferential_follows = 0;
    int64 n_entity_follows = 0, n_pref_entity_follows = 0;
    int64 n_retweet_follows = 0, n_hashtag_follows = 0;
    int64 n_hashtags = 0;
};

// Records in both entity type struct and global struct:
#define RECORD_STAT(state, , stat) \


struct NetworkStats {
    double prob_add;
    double prob_follow;
    double prob_tweet;
    double prob_retweet;

    int n_outputs;
    EntityStats global_stats;

    double event_rate;
    NetworkStats() {
        prob_add = 0;
        prob_follow = 0;
        prob_tweet = 0;
        prob_retweet = 0;

        n_outputs = 0;
        event_rate = 0;
    }

    READ_WRITE(rw) {
        rw << prob_add << prob_follow << prob_retweet << prob_tweet;
        rw << n_outputs;
        // Valid because only full of primitive types:
        rw << global_stats;
        rw << event_rate;
    }
};

const int APPROX_MONTH = 30 * 24 * 60;

struct Analyzer;

struct InteractiveModeState {
    // Interactive mode allows for controlling the simulation using Lua.
    // we run the interactive_mode Lua function, defined typically
    // in the INTERACT.lua file, or the script file specified in the input file.

    // If real_time_target != 0 then once the real_time_since passes real_time_target,
    // we begin interactive mode.
    double real_time_target = 0;
    Timer real_time_since;

    // If AnalysisState.time > sim_time_target, we begin interactive mode.
    double sim_time_target = 0;
};

// All the state passed to - and - from analyze.cpp.
// Essentially this encapsulates all the information required for the post-analysis routines.
// This is 'conceptually cleaner' than passing along the entire contents of the Analyzer struct.

struct AnalysisState {
    // A back-pointer to the Analyzer structure.
    // NULL if analysis is not active!
    // Used to call Analyzer methods outside of analyzer_main.cpp.
    Analyzer* analyzer;

    // The current simulation time
    double time;
    ParsedConfig config;

    // The full contents of the simulated network.
    Network network;

    // Various categorizations of users.
    CategoryGrouper tweet_ranks;
    CategoryGrouper follow_ranks;
    CategoryGrouper retweet_ranks;
    CategoryGrouper age_ranks;
    Add_Rates add_rates;

    // Our distinct entity classes.
    // Entity probabilities are derived from config,
    // while the list of users within is derived from
    EntityTypeVector entity_types;
    std::vector<int> entity_cap;

    // Add any values that must be extracted from 'analyze' here.
    int n_follows;
    double r_follow_norm, end_time;

    /* tweet_bank: The central tweeting data-structure.
   
     Holds all the incoming 'tweets', abstract packages of information that are 
     propagated through an entity'network by entity connections. 
     
     Those subscribing to (following) another entity are elligible for tweet 
     reaction upon observing their 'feed'
    
     and organizes them into a tree-structure.
     This structure contains rate summaries for each node of the tree, allowing
     for selection via a number of weighted steps.

     Complexity:
       - O(log N), insertion into the tweet bank via add(tweet object)
       - O(log N), weighted selection for a tweet via pick_random_uniform(RNG object) */

    TweetBank tweet_bank;

    /* most_pop_tweet: The most popular tweet, by number of retweets. */

    MostPopularTweet most_pop_tweet;

    /* HashTags: 
     An abstraction of global, topic-oriented discussion lists taking place 
     in the social network. This can result in information passing across the 
     entire network. 
     
     A small window of 'tweets' are kept for each topic, and every entity has
     a chance of propagating a given tweet in their own immediate network. */

    HashTags hashtags;
    std::vector<double> follow_probabilities, updating_follow_probabilities;

    /* InteractiveModeState: 
     State for determining when to begin interactive mode. See above. */
    InteractiveModeState interactive_mode_state;

    MTwist rng;

    /* AnalysisStats:
     Various statistics gathered for analysis purposes. */
    NetworkStats stats;

    AnalysisState(const ParsedConfig& config, int seed) :
            config(config), tweet_bank(*this){
        analyzer = NULL;
        n_follows = 0;
        r_follow_norm = end_time = 0;
        tweet_ranks = config.tweet_ranks;
        follow_ranks = config.follow_ranks;
        retweet_ranks = config.retweet_ranks;
        entity_types = config.entity_types;
        add_rates = config.add_rates;

        follow_probabilities = config.follow_probabilities;

        rng.init_genrand(seed);
        time = 0.0;
        // Let analyze.cpp handle any additional initialization logic from here.
    }

    EntityType& entity_type(int entity_id) {
        return entity_types[network[entity_id].entity_type];
    }

    void event_stat_log(int entity_id, EventID event, double increment = 1.0) {
        EntityType& type = entity_type(entity_id);
        // Increment per-type
        type.event_stats[event] += increment;
        // Increment per-event
        stats.event_stats[event] += increment;
    }

    int n_months() {
        return time / APPROX_MONTH;
    }

    // For network reading/writing:
    READ_WRITE(rw) {
        network.visit(rw);

        rw << time;
        rw << follow_probabilities;
        rw << updating_follow_probabilities;

        rw << entity_cap;
        rw << n_follows << r_follow_norm << end_time;

        rng.visit(rw);
        tweet_ranks.visit(rw);
        follow_ranks.visit(rw);
        retweet_ranks.visit(rw);
        age_ranks.visit(rw);
        tweet_bank.visit(rw);
        stats.visit(rw);
        hashtags.visit(rw);
        rw.visit_objs(entity_types);
    }
};

enum SelectionType {
    FOLLOW_SELECT,
    RETWEET_SELECT,
    TWEET_SELECT
};

struct RetweetChoice {
    // See Tweet for details:
    int id_author;
    int id_observer;
    int id_link;
    int generation;

    // We could have used 'smartptr', however the tweet is not going anywhere
    // in the short while we retweet, so a pointer is safe.
    smartptr<TweetContent>* content;
    RetweetChoice() :
            id_author(-1), id_observer(-1), id_link(-1), generation(-1), content(NULL) {
    }
    RetweetChoice(int id_author, int id_observer, int id_link, int generation, smartptr<TweetContent>* tweet) :
            id_author(id_author), id_observer(id_observer), id_link(id_link), generation(generation), content(tweet) {
    }
    bool valid() {
        return (id_author != -1);
    }
};

// 'analyzer_select_entity' and 'analyzer_set_rates' implement time-dependent rates
// Select based on any SelectionType
int analyzer_select_entity(AnalysisState& state, SelectionType type);
void analyzer_rate_update(AnalysisState& state);

// Follow a specific user
bool analyzer_handle_follow(AnalysisState& state, int id_actor, int id_target);

bool analyzer_follow_entity(AnalysisState& state, int entity, double time_of_follow);

// Implements a follow-back
bool analyzer_followback(AnalysisState& state, int follower, int followed);

// Run a network simulation using the given input file's parameters
void analyzer_main(AnalysisState& state);

// this returns the total retweet rate
double analyzer_total_retweet_rate(AnalysisState& state);

// this is for the retweet entity selection
RetweetChoice analyzer_select_tweet_to_retweet(AnalysisState& state, SelectionType type);

// Create an entity
bool analyzer_create_entity(AnalysisState& state);

void update_retweets(AnalysisState& state);

#endif
