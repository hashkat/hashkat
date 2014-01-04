#ifndef __ANALYZE_H_
#define __ANALYZE_H_

#include <string>
#include <map>
#include <vector>
#include "dependencies/mtwist.h"

#include "config.h"
#include "network.h"

extern volatile int CTRL_C_ATTEMPTS;

// All the state passed to - and - from analyze.cpp.
// Essentially this encapsulates all the information required for the post-analysis routines.
// This is 'conceptually cleaner' than passing along the entire contents of the Analyzer struct.

struct AnalysisState {
    InfileConfig config;

    // Note, 'MemPoolVectorGrower' is required to be in same scope as network, otherwise we will have undefined memory accesses.
    MemPoolVectorGrower follow_set_grower;
    // The full contents of the simulated network.
    Network network;
    // Various categorizations of users.
    CategoryGrouper tweet_ranks;
    CategoryGrouper follow_ranks;
    CategoryGrouper retweet_ranks;

    // struct for the entity classes, see network.h for specifications
    EntityType entity_types[ET_AMOUNT];
	std::vector<int>entity_cap;
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
