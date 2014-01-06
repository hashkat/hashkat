#ifndef __ANALYZE_H_
#define __ANALYZE_H_

#include <string>
#include <map>
#include <vector>
#include "dependencies/mtwist.h"

#include "config.h"
#include "network.h"

extern volatile int CTRL_C_ATTEMPTS;

struct AnalysisStats {
    double R_ADD_NORM;
    double R_FOLLOW_NORM;
    double R_TWEET_NORM;
    double R_RETWEET_NORM;

    int64 N_STEPS, N_FOLLOWS, N_TWEETS, N_RETWEETS;
    int n_months;
    double R_TOTAL;
    AnalysisStats() {
        R_ADD_NORM = 0;
        R_FOLLOW_NORM = 0;
        R_TWEET_NORM = 0;
        R_RETWEET_NORM = 0;
        N_STEPS = 0, N_FOLLOWS = 0, N_TWEETS = 0, N_RETWEETS = 0, n_months = 0;
        R_TOTAL = 0;
    }
};

// Config variables, and derived rates
struct CachedConfig {
    int MAX_ENTITIES;
    int VERBOSE;
    int RANDOM_INCR;
    int FOLLOW_METHOD;
    int BARABASI;
    double T_FINAL;

    double R_FOLLOW_INI;
    double R_TWEET_INI;
    double R_ADD_INI;
    double R_RETWEET_INI;

    bool output_summary_stats;
    CachedConfig(InfileConfig& config) {
        MAX_ENTITIES = config["MAX_ENTITIES"];
        VERBOSE = config["VERBOSE"];
        RANDOM_INCR = config["RANDOM_INCR"];
        FOLLOW_METHOD = config["FOLLOW_METHOD"];
        BARABASI = config["BARABASI"];

        T_FINAL = config["T_FINAL"];

        R_FOLLOW_INI = config["R_FOLLOW"];
        R_TWEET_INI = config["R_TWEET"];
        R_ADD_INI = config["R_ADD"];
        R_RETWEET_INI = config["R_RETWEET"];
        output_summary_stats = (config["OUTPUT_SUMMARY"] != 0);
    }
};

// All the state passed to - and - from analyze.cpp.
// Essentially this encapsulates all the information required for the post-analysis routines.
// This is 'conceptually cleaner' than passing along the entire contents of the Analyzer struct.

struct AnalysisState {
    InfileConfig config;

    // Note, 'MemPoolVectorGrower' is required to be in same scope as network, otherwise we will have undefined memory accesses.
    MemPoolVectorGrower<int> follow_set_grower;
    // The full contents of the simulated network.
    Network network;
    // Various categorizations of users.
    CategoryGrouper tweet_ranks;
    CategoryGrouper follow_ranks;
    CategoryGrouper retweet_ranks;
    CategoryGrouper age_ranks;

    // struct for the entity classes, see network.h for specifications
    EntityType entity_types[ET_AMOUNT];
    std::vector<int> entity_cap;
    // Add any values that must be extracted from 'analyze' here.
    int n_follows;
    double r_follow_norm, end_time;

    AnalysisState(const std::map<std::string, std::string>& string_map) :
            config(string_map) {
        // Let analyze.cpp handle all initialization logic from here.
        n_follows = 0;
        r_follow_norm = end_time = 0;
    }
};

// Run a network simulation using the given input file's parameters
void simulate_network(AnalysisState& analysis_state, int seed);

#endif
