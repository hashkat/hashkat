#ifndef CONFIG_DYNAMIC_H_
#define CONFIG_DYNAMIC_H_

enum FollowModel {
    RANDOM_FOLLOW,
    PREFERENTIAL_FOLLOW,
    ENTITY_FOLLOW,
    RETWEET_FOLLOW
};

#include "network.h"

// Config variables, read from INFILE.yaml
struct ParsedConfig {
    // 'analysis' config options
    int initial_entities, max_entities;
    double max_time;
    bool use_random_increment;
    bool use_preferential_follow;
    bool use_barabasi;
    FollowModel follow_model;

    // 'rates' config options
    double rate_follow, rate_tweet;
    double rate_add, rate_retweet;

    // 'output' config options
    bool output_stdout_basic, output_stdout_summary;
    bool output_verbose, output_visualize;
    bool degree_distributions;
    bool output_tweet_analysis, output_cumulative_analysis;
    // 'X_category' config options

    // Thresholds are filled, entity lists empty
    CategoryGrouper tweet_ranks, follow_ranks;
    CategoryGrouper retweet_ranks;

    // 'entities' config options
    // Note: Weights are filled, entity lists empty
    EntityTypeVector entity_types;

    // command-line config options
    bool handle_ctrlc;

    /* Most config values are optional -- place defaults here. */
    ParsedConfig() {
        initial_entities = 0;
        max_entities = 1000;
        max_time = INFINITY;

        use_random_increment = false;
        use_preferential_follow = false;
        use_barabasi = false;

        follow_model = RANDOM_FOLLOW;

        rate_follow = rate_tweet = 0;
        rate_add = rate_retweet = 0;

        output_stdout_basic = output_stdout_summary = true;
        output_verbose = output_visualize = false;
        output_tweet_analysis = output_cumulative_analysis = false;
		degree_distributions = true;
		handle_ctrlc = false;
    }
};

ParsedConfig parse_yaml_configuration(const char* file_name);

#endif
