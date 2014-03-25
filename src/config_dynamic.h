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
    std::vector<double> thresholds;
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


struct Subregion {
    std::string name;
    std::vector<double> ideology_probs;
    std::vector<double> language_probs;
    std::vector<double> preference_class_probs;
};

struct Region {
    std::string name;
    std::vector<double> add_probs; // For the subregions
    std::vector<Subregion> subregions;
};

struct Regions {
    std::vector<double> add_probs; // For the regions
    std::vector<Region> regions;
};

// Config variables, read from INFILE.yaml
struct ParsedConfig {
    // Values here are just in lieu of garbage -- many of them are mandatory config variables.

    // Used for serialization to be *extra sure* we aren't simulating a corrupted network.
    // If the entire config file being used does not match, we do not attempt to restart execution.
    // This is only safe.
    std::string entire_config_file;

    // 'analysis' config options
    int initial_entities = 0, max_entities = 1;
    double max_sim_time = INFINITY;
    double max_real_time = INFINITY;
    bool use_random_time_increment = false;
    bool use_preferential_follow = false;
    bool use_flawed_followback = false;
    bool use_barabasi = false;
    bool entity_stats = false;
    bool stage1_unfollow = false;
    FollowModel follow_model = RANDOM_FOLLOW;
    std::vector<double> model_weights;

    bool save_network_on_timeout = false, load_network_on_startup = false;
    bool ignore_load_config_check = false;
    std::string save_file;

    // 'rates' config options
    double rate_add = 0;
    double unfollow_tweet_rate = 0;

    // 'output' config options
    bool output_stdout_basic = false, output_stdout_summary = false;
    bool output_visualize = false;
    bool degree_distributions = false;
    bool output_tweet_analysis = false;
    bool output_verbose = false;
    bool retweet_viz = false;
    // 'X_category' config options

    // Thresholds are filled, entity lists empty
    CategoryGrouper tweet_ranks, follow_ranks;
    CategoryGrouper retweet_ranks;
    LanguageProbabilities lang_probs;

    std::vector<double> follow_probabilities;
    Regions regions;
    // 'entities' config options
    // Note: Weights are filled, entity lists empty
    EntityTypeVector entity_types;
    Add_Rates add_rates;

    std::vector<EntityPreferenceClass> pref_classes;


    bool enable_interactive_mode = false;

    // command-line config options
    bool handle_ctrlc = false;

    // tweet_obs: 
    //  An observation probability density function that gives 
    //  the probability that a tweet is observed at a certain time by an 'ideal observer'. 
    //  An 'ideal observer' is one which always sees a tweet, eventually.'
    //  The observation PDF is used for both retweeting and follow-from-tweet.
    //  We combine this with a relevance factor, r, where 0 <= r <= 1.0, we in turn
    //  determines the probability that a given entity will act on a given tweet, with enough time.
    TweetObservationPDF tweet_obs;

    // follower_rates: 
    //  The transmission probability for a person of a certain preference class towards
    //  a tweet of a given origin & content.

    FollowerSetRatesDeterminer follower_rates;
};

ParsedConfig parse_yaml_configuration(const char* file_name);

#endif
