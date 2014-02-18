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
    double prob_retweet;

    int n_outputs;
    int64 n_steps, n_follows, n_tweets, n_retweets;
    EntityEventStats event_stats;

    double event_rate;
    AnalysisStats() {
        prob_add = 0;
        prob_follow = 0;
        prob_tweet = 0;
        prob_retweet = 0;

        n_outputs = 0;
        n_steps = 0, n_follows = 0, n_tweets = 0, n_retweets = 0;
        event_rate = 0;
    }
};

const int APPROX_MONTH = 30 * 24 * 60;

// All the state passed to - and - from analyze.cpp.
// Essentially this encapsulates all the information required for the post-analysis routines.
// This is 'conceptually cleaner' than passing along the entire contents of the Analyzer struct.

struct AnalysisState {
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

    std::vector<double> follow_probabilities, updating_follow_probabilities;

    MTwist rng;

    AnalysisStats stats;
    AnalysisState(const ParsedConfig& config, int seed) :
            config(config) {
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

};

enum SelectionType {
    FOLLOW_SELECT,
    RETWEET_SELECT,
    TWEET_SELECT
};

struct RetweetChoice {
    int e_author;
    int e_observer;
    RetweetChoice(int e_author, int e_observer) :
            e_author(e_author), e_observer(e_observer) {
    }
    bool valid() {
        return (e_author != -1);
    }
};

// 'analyzer_select_entity' and 'analyzer_set_rates' implement time-dependent rates
// Select based on any SelectionType
int analyzer_select_entity(AnalysisState& state, SelectionType type);
void analyzer_rate_update(AnalysisState& state);
// Follow a specific user
bool analyzer_follow_entity(AnalysisState& state, int entity, int n_entities, double time_of_follow);
// Implements a follow-back
bool analyzer_followback(AnalysisState& state, int follower, int followed);
// Run a network simulation using the given input file's parameters
void analyzer_main(AnalysisState& state);
// this returns the total retweet rate
double analyzer_total_retweet_rate(AnalysisState& state);
// this is for the retweet entity selection
RetweetChoice analyzer_select_entity_retweet(AnalysisState& state, SelectionType type);
void update_retweets(AnalysisState& state);

const double ZEROTOL = 1e-16; // enough precision for really really low add rate

#endif
