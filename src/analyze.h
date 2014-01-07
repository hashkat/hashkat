#ifndef __ANALYZE_H_
#define __ANALYZE_H_

#include <string>
#include <map>
#include <vector>
#include "dependencies/mtwist.h"

#include "config_dynamic.h"
#include "network.h"

extern volatile int CTRL_C_ATTEMPTS;

struct AnalysisStats {
    double prob_add;
    double prob_follow;
    double prob_tweet;
    double prob_norm;

    int64 n_steps, n_follows, n_tweets, n_retweets;
    int n_months;
    double event_rate;
    AnalysisStats() {
        prob_add = 0;
        prob_follow = 0;
        prob_tweet = 0;
        prob_norm = 0;
        n_steps = 0, n_follows = 0, n_tweets = 0, n_retweets = 0, n_months = 0;
        event_rate = 0;
    }
};


// All the state passed to - and - from analyze.cpp.
// Essentially this encapsulates all the information required for the post-analysis routines.
// This is 'conceptually cleaner' than passing along the entire contents of the Analyzer struct.

struct AnalysisState {
    ParsedConfig config;

    // Note, 'MemPoolVectorGrower' is required to be in same scope as network, otherwise we will have undefined memory accesses.
    MemPoolVectorGrower<int> follow_set_grower;
    // The full contents of the simulated network.
    Network network;
    // Various categorizations of users.
    CategoryGrouper tweet_ranks;
    CategoryGrouper follow_ranks;
    CategoryGrouper retweet_ranks;
    CategoryGrouper age_ranks;

    // Our distinct entity classes.
    // Entity probabilities are derived from config,
    // while the list of users within is derived from
    EntityTypeVector entity_types;
    std::vector<int> entity_cap;
    // Add any values that must be extracted from 'analyze' here.
    int n_follows;
    double r_follow_norm, end_time;

    AnalysisState(const ParsedConfig& config) :
            config(config) {
        // Let analyze.cpp handle all initialization logic from here.
        n_follows = 0;
        r_follow_norm = end_time = 0;
        tweet_ranks = config.tweet_ranks;
        follow_ranks = config.follow_ranks;
        retweet_ranks = config.retweet_ranks;
        entity_types = config.entity_types;
    }
};

// Run a network simulation using the given input file's parameters
void simulate_network(AnalysisState& analysis_state, int seed);

#endif
