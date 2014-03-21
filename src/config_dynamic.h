#ifndef CONFIG_DYNAMIC_H_
#define CONFIG_DYNAMIC_H_

#include <cmath>
#include "network.h"

enum FollowModel {
    RANDOM_FOLLOW,
    PREFERENTIAL_FOLLOW,
    ENTITY_FOLLOW,
    PREFERENTIAL_ENTITY_FOLLOW,
        TWITTER_FOLLOW
};

struct EntityPreferenceClass {
    std::string name;
};

// Also referred to as the Omega function, see INFILE.yaml for details:
struct TweetObservationPDF {
    double initial_resolution;
    std::vector<double> values;

    // Inverse of our exponential function:
    int get_probability_bin(double time) {
        const double log2 = 0.30102999566;
        double R = initial_resolution;
        double L = log( (R+time) / (3.0 * R));
        int bin = (int)ceil((L - log2) / log2);
        if (bin >= values.size()) {
            return values.size() - 1;
        }
        return bin;
    }
};

//TODO AD: Better name
// First layer: entity_type
// Second layer: humour bin
typedef std::vector< std::vector<FollowerSet::Rates> > FollowerSetRatesVec;

//TODO AD: Better name
struct FollowerSetRatesDeterminer {
    FollowerSetRatesDeterminer() {
    }

    FollowerSetRatesDeterminer(const FollowerSetRatesVec& rates) : rates(rates){
    }

    // Defined in .cpp file
    FollowerSet::Rates& get_rates(Entity& entity);

private:
    FollowerSetRatesVec rates;
};

// Config variables, read from INFILE.yaml
struct ParsedConfig {
    // Used for serialization to be *extra sure* we aren't simulating a corrupted network.
    // If the entire config file being used does not match, we do not attempt to restart execution.
    // This is only safe.
    std::string entire_config_file;

    // 'analysis' config options
    int initial_entities, max_entities;
    double max_sim_time;
    double max_real_time;
    bool use_random_time_increment;
    bool use_preferential_follow;
    bool use_flawed_followback;
    bool use_barabasi;
    bool entity_stats;
    bool stage1_unfollow;
    FollowModel follow_model;

    FollowerSetRatesDeterminer follower_rates;

    bool save_network_on_timeout, load_network_on_startup;
    bool ignore_load_config_check;
    std::string save_file;

    // 'rates' config options
    double rate_add;
    double unfollow_tweet_rate;

    // 'output' config options
    bool output_stdout_basic, output_stdout_summary;
    bool output_visualize;
    bool degree_distributions;
    bool output_tweet_analysis;
    bool output_verbose;
    // 'X_category' config options

    // Thresholds are filled, entity lists empty
    CategoryGrouper tweet_ranks, follow_ranks;
    CategoryGrouper retweet_ranks;
    LanguageProbabilities lang_probs;

    std::vector<double> follow_probabilities;
    // 'entities' config options
    // Note: Weights are filled, entity lists empty
    EntityTypeVector entity_types;
    Add_Rates add_rates;

    std::vector<EntityPreferenceClass> pref_classes;

    bool enable_interactive_mode;

    // command-line config options
    bool handle_ctrlc;

    // Tweet relevance functions
    TweetObservationPDF tweet_obs;

    /* Most config values are optional -- place defaults here. */
    ParsedConfig() {
        initial_entities = 0;
        max_entities = 1000;
        max_sim_time = INFINITY;
        max_real_time = INFINITY;

        entity_stats = false;
        use_random_time_increment = false;
        use_preferential_follow = false;
        use_barabasi = false;
        use_flawed_followback = false;
        output_verbose = false;

        ignore_load_config_check = false;
        save_network_on_timeout = false;
        load_network_on_startup = false;

        unfollow_tweet_rate = 0.0;
        follow_model = RANDOM_FOLLOW;

        rate_add = 0;
        enable_interactive_mode = false;

        output_stdout_basic = output_stdout_summary = true;
        output_visualize = false;
        output_tweet_analysis = false;
		degree_distributions = true;
		handle_ctrlc = false;
    }
};

ParsedConfig parse_yaml_configuration(const char* file_name);

#endif
