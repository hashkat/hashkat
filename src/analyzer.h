/*
 * This file is part of the #KAT Social Network Simulator.
 *
 * The #KAT Social Network Simulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The #KAT Social Network Simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the #KAT Social Network Simulator.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Addendum:
 *
 * Under this license, derivations of the #KAT Social Network Simulator typically must be provided in source
 * form. The #KAT Social Network Simulator and derivations thereof may be relicensed by decision of 
 * the original authors (Kevin Ryczko & Adam Domurad, Isaac Tamblyn), as well, in the case of a derivation,
 * subsequent authors. 
 */

#ifndef __ANALYZE_H_
#define __ANALYZE_H_

#include <string>
#include <map>
#include <vector>
#include <memory>

#include <lcommon/Timer.h>

#include <dependencies/mtwist.h>

extern "C" {
#include "capi.h"
}

#include "config_dynamic.h"
#include "network.h"
#include "agent.h"
#include "tweets.h"

#include "serialization.h"
#include "TweetBank.h"

extern volatile int SIGNAL_ATTEMPTS;

// Global network stats
struct NetworkStats {
    double prob_add = 0;
    double prob_follow = 0;
    double prob_tweet = 0;
    double prob_retweet = 0;

    double event_rate = 0;

    int n_steps = 0, n_outputs = 0;
    bool user_did_exit = false;

    AgentStats global_stats;

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(NVP(prob_add), NVP(prob_follow), NVP(prob_retweet), NVP(prob_tweet));
        ar(NVP(event_rate), NVP(n_steps), NVP(n_outputs));
        // Don't serialize 'user_did_exit'
        // Valid because only full of primitive types:
        ar(NVP(global_stats), NVP(user_did_exit));
    }
};

// Records in the AgentStats member found both in the agent type struct and global struct:
#define RECORD_STAT(state, agent_type, stat) \
    state.agent_types[agent_type].stats. stat ++; \
    state.stats.global_stats. stat ++;

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

    AnalysisState( const AnalysisState& other ) = delete; // non construction-copyable
    AnalysisState& operator=( const AnalysisState& ) = delete; // non copyable

    // A back-pointer to the Analyzer structure.
    // Empty if analysis is has not started!
    // Used to call Analyzer methods outside of analyzer_main.cpp.
    std::shared_ptr<Analyzer> analyzer;

    // The current simulation time
    double time;
    ParsedConfig config;

    // Used for running code on every hashkat event.
    // This is used to communicate with hashkat during the event loop.
    EventCallbacks event_callbacks;

    // The full contents of the simulated network.
    Network network;

    // Various categorizations of users.
    CategoryGrouper tweet_ranks;
    CategoryGrouper follow_ranks;
    CategoryGrouper retweet_ranks;
    CategoryGrouper age_ranks;

    // Our distinct agent classes.
    // Agent probabilities are derived from config,
    // while the list of users within is derived from
    AgentTypeVector agent_types;
    std::vector<int> agent_cap;

    // Add any values that must be extracted from 'analyze' here.
    // This will be copied at the end of the simulation, exposing it.
    int n_follows;
    double end_time;

    /* tweet_bank: The central tweeting data-structure.
   
     Holds all the incoming 'tweets', abstract packages of information that are 
     propagated through an agent'network by agent connections. 
     
     Those subscribing to (following) another agent are elligible for tweet 
     reaction upon observing their 'feed'
    
     and organizes them into a tree-structure.
     This structure contains rate summaries for each node of the tree, allowing
     for selection via a number of weighted steps.

     Complexity:
       - O(log N), insertion into the tweet bank via add(tweet object)
       - O(log N), weighted selection for a tweet via pick_random_uniform(RNG object) */

    TweetBank tweet_bank;

    // old tweets being stored in a vector to be accessed after sim
    std::vector<Tweet> oldTweets;

    /* most_pop_tweet: The most popular tweet, by number of retweets. */

    MostPopularTweet most_pop_tweet;

    /* HashTags: 
     An abstraction of global, topic-oriented discussion lists taking place 
     in the social network. This can result in information passing across the 
     entire network. 
     
     A small window of 'tweets' are kept for each topic, and every agent has
     a chance of propagating a given tweet in their own immediate network. */

    HashTags hashtags;
    std::vector<double> updating_follow_probabilities;

    /* InteractiveModeState: 
     State for determining when to begin interactive mode. See above. */
    InteractiveModeState interactive_mode_state;

    MTwist rng;

    /* AnalysisStats:
     Various statistics gathered for analysis purposes. */
    NetworkStats stats;

    AnalysisState(const ParsedConfig& config, int seed) :
            config(config), tweet_bank(*this){
        n_follows = 0;
        end_time = 0;

        // CategoryGrouper carries both state and configuration.
        // We copy over the data so that we can operate on it.
        tweet_ranks = config.tweet_ranks;
        follow_ranks = config.follow_ranks;
        retweet_ranks = config.retweet_ranks;
        agent_types = config.agent_types;
        // Fill callbacks with NULL
        memset(&event_callbacks, 0, sizeof(EventCallbacks));

        rng.init_genrand(seed);
        time = 0.0;
        // Let analyze.cpp handle any additional initialization logic from here.
    }

    AgentType& agent_type(int agent_id) {
        return agent_types[network[agent_id].agent_type];
    }

    int n_months() {
        return time / APPROX_MONTH;
    }


    /* Synchronize rates from a loaded configuration.
     * This is done because, although we can load a new configuration,
     * some rates remain duplicated in our state object. */
    void sync_rates() {
        // We must sync anything copied from the 'config' object in the constructor.
        tweet_ranks.sync_rates(config.tweet_ranks);
        follow_ranks.sync_rates(config.follow_ranks);
        retweet_ranks.sync_rates(config.retweet_ranks);
        for (int i = 0; i < agent_types.size(); i++) {
            agent_types[i].sync_configuration(config.agent_types[i]);
        }
    }

    // For network reading/writing:
    template <typename Archive>
    void serialize(Archive& ar) {
        ar(NVP(network));
        ar(NVP(time));
        // Don't serialize config
        // Don't serialize event_callbacks
        // Don't serialize analyzer

        ar(NVP(tweet_ranks));
        ar(NVP(follow_ranks));
        ar(NVP(retweet_ranks));
        ar(NVP(age_ranks));
        ar(NVP(tweet_bank));
        ar(NVP(oldTweets));
        ar(NVP(stats));
        ar(NVP(hashtags));
        ar(NVP(agent_types));
        ar(NVP(agent_cap));

        ar(NVP(n_follows), NVP(end_time));
        ar(NVP(updating_follow_probabilities));
        // Don't serialize interactive_mode_state
        ar(NVP(rng));
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

    std::shared_ptr<TweetContent>* content;
    RetweetChoice() :
            id_author(-1), id_observer(-1), id_link(-1), generation(-1), content(NULL) {
    }
    RetweetChoice(int id_author, int id_observer, int id_link, int generation, std::shared_ptr<TweetContent>* tweet) :
            id_author(id_author), id_observer(id_observer), id_link(id_link), generation(generation), content(tweet) {
    }
    bool valid() {
        return (id_author != -1);
    }
};

/** Function prototypes **/

// 'analyzer_select_agent' and 'analyzer_set_rates' implement time-dependent rates
// Select based on any SelectionType
int analyzer_select_agent(AnalysisState& state, SelectionType type);
void analyzer_rate_update(AnalysisState& state);

// Follow a specific user
bool analyzer_handle_follow(AnalysisState& state, int id_actor, int id_target, int follow_method);
double preferential_weight(AnalysisState& state);
bool analyzer_handle_unfollow(AnalysisState& state, int id_target, int id_actor);

bool analyzer_sim_time_check(AnalysisState& state);
bool analyzer_real_time_check(AnalysisState& state);
void analyzer_save_network_state(AnalysisState& state, const char* fname);
void analyzer_load_network_state(AnalysisState& state, const char* fname);
bool analyzer_follow_agent(AnalysisState& state, int agent, double time_of_follow);

// Implements a follow-back
bool analyzer_followback(AnalysisState& state, int follower, int followed);

// Run a network simulation using the given input file's parameters
void analyzer_main(AnalysisState& state);

// this returns the total retweet rate
double analyzer_total_retweet_rate(AnalysisState& state);

// this is for the retweet agent selection
RetweetChoice analyzer_select_tweet_to_retweet(AnalysisState& state, SelectionType type);

// Create an agent
bool analyzer_create_agent(AnalysisState& state);

void update_retweets(AnalysisState& state);

#endif
