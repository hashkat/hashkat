#ifndef __ANALYZE_H_
#define __ANALYZE_H_

#include <string>
#include <map>
#include <vector>
#include "dependencies/mtwist.h"

#include "config_dynamic.h"
#include "network.h"

#include "DataReadWrite.h"

extern volatile int SIGNAL_ATTEMPTS;

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

    VISIT0(rw) {
        rw << prob_add << prob_follow << prob_retweet << prob_tweet;
        rw << n_outputs << n_steps << n_follows << n_tweets << n_retweets;
        rw << event_stats.stats_array;
        rw << event_rate;
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

    TweetBank tweet_bank;
    MostPopularTweet most_pop_tweet;
    std::vector<double> follow_probabilities, updating_follow_probabilities;

    MTwist rng;

    AnalysisStats stats;
    AnalysisState(const ParsedConfig& config, int seed) :
            config(config), tweet_bank(*this){
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
    VISIT0(rw) {
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
bool analyzer_follow_entity(AnalysisState& state, int entity, int n_entities, double time_of_follow);
// Implements a follow-back
bool analyzer_followback(AnalysisState& state, int follower, int followed);
// Run a network simulation using the given input file's parameters
void analyzer_main(AnalysisState& state);
// this returns the total retweet rate
double analyzer_total_retweet_rate(AnalysisState& state);
// this is for the retweet entity selection
RetweetChoice analyzer_select_tweet_to_retweet(AnalysisState& state, SelectionType type);
void update_retweets(AnalysisState& state);

#endif
