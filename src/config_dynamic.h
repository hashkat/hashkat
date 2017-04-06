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

#ifndef CONFIG_DYNAMIC_H_
#define CONFIG_DYNAMIC_H_

#include <cmath>
#include <memory>
#include "network.h"
#include "events.h"

#include "FollowerSet.h"

struct PreferenceClass {
    std::string name;
    // Probability that a tweet reaction results in a follow, rather than a retweet:
};

// Also referred to as the Omega function, see INFILE.yaml for details:
struct TweetObservationPDF {
    double initial_resolution;

    std::vector<double> values;
    std::vector<double> thresholds;
};

struct Region {
    std::string name;
    std::vector<double> ideology_probs;
    std::vector<double> language_probs;
    std::vector<double> preference_class_probs;
};

struct Ideology {
    std::string name;
};

struct Regions {
    std::vector<double> add_probs; // For choosing a region, sums to 1
    std::vector<Region> regions;
    size_t size() {
        return regions.size();
    }
};

// Config variables, read from INFILE.yaml
struct ParsedConfig {
    // Values here are just in lieu of garbage -- many of them are mandatory config variables.

    // Used for serialization to be *extra sure* we aren't simulating a corrupted network.
    // If the entire config file being used does not match, we do not attempt to restart execution.
    // This is only safe.
    std::string entire_config_file;

    // 'analysis' config options
    int initial_agents = 0, max_agents = 1;
    double max_sim_time = INFINITY;
    double max_real_time = INFINITY;
    long long max_analysis_steps = (1LL << 53); // 2**53; impossibly large number.
    bool use_random_time_increment = false;
    bool use_preferential_follow = false;
    bool use_followback = false;
    bool use_follow_via_retweets = false;
    bool use_barabasi = false;
    bool region_connection_matrix = false;
    bool enable_query_api = false;
    bool use_susceptibility = false;
    
    int barabasi_connections = 1;
    double barabasi_exponent = 1;
    
    bool agent_stats = false;
    // Controls whether tweet output files are generated.
    // WARNING, having this option turned on requires memory 
    // proportional to the amount of tweets.
    bool full_tweet_stats = false;
    bool stage1_unfollow = false;
    double hashtag_prob = 0;
    FollowModel follow_model = RANDOM_FOLLOW;
    std::vector<double> model_weights;

    bool save_network_on_timeout = false, load_network_on_startup = false;
    bool ignore_load_config_check = false;
    std::string save_file;
    std::string lua_script = "INTERACT.lua";

    // 'rates' config options
    double rate_add = 0;
    double unfollow_tweet_rate = 0;

    // 'output' config options
    bool output_stdout_basic = false, output_stdout_summary = false;
    bool output_visualize = false;
    bool degree_distributions = false;
    bool output_tweet_analysis = false;
    bool retweet_viz = false;
    bool main_stats = false;
    bool dd_by_follow_model = false;
    bool summary_output_rate_real_minutes = false;
    double summary_output_rate = 1;
    bool categories_distro = false;
    bool most_popular_tweet_content = false;
    bool tweet_info = false;

    // 'X_category' config options

    // Thresholds are filled, agent lists empty
    CategoryGrouper tweet_ranks, follow_ranks;
    CategoryGrouper retweet_ranks;
    Rate_Function referral_rate_function;

    std::vector<double> follow_probabilities;
    Regions regions;
    // 'agents' config options
    // Note: Weights are filled, agent lists empty
    AgentTypeVector agent_types;
    Add_Rates add_rates;

    std::vector<PreferenceClass> pref_classes;
    std::vector<Ideology> ideologies;

    bool enable_interactive_mode = false;
    bool enable_lua_hooks = false;

    // command-line config options
    bool handle_ctrlc = false;

    // tweet_obs: 
    //  An observation probability density function that gives 
    //  the probability that a tweet is observed at a certain time by an 'ideal observer'. 
    //  An 'ideal observer' is one which always sees a tweet, eventually.'
    //  The observation PDF is used for both retweeting and follow-from-tweet.
    //  We combine this with a relevance factor, r, where 0 <= r <= 1.0, we in turn
    //  determines the probability that a given agent will act on a given tweet, with enough time.
    TweetObservationPDF tweet_obs;

    // tweet_react_rates:
    //  The rates with which a tweet is reacted to, accounting for both
    //  characteristics of the tweeter, and the agent.
    FollowerSet::WeightDeterminer tweet_react_rates;
};

ParsedConfig parse_yaml_configuration(const char* file_name);

#endif
