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

#include <cstdio>
#include <fstream>
#include <iostream>
#include <exception>

#include <lcommon/strformat.h>

#include <yaml-cpp/yaml.h>

#include "dependencies/prettyprint.hpp"
#include "util.h"

#include "CategoryGrouper.h"
#include "config_dynamic.h"
#include "agent.h"

using namespace std;
using namespace YAML;

template<class T>
static inline void parse(const Node& node, const char* key, T& value, bool optional = false) {
    if (node.FindValue(key)) {
        node[key] >> value;
    } else if (!optional) {
        throw YAML::RepresentationException(node.GetMark(),
                format("'%s' was not found!", key));
    }
}
// Optionally parse an element, leaving it alone if it was not found
template<class T>
static inline void parse_opt(const Node& node, const char* key, T& value) {
    parse(node, key, value, true);
}

/* Convert from a text node to the number representing
 * the follow model. */
static FollowModel parse_follow_model(const Node& node) {
    string follow_model = "random";
    parse(node, "follow_model", follow_model);
    if (follow_model == "random") {
        return RANDOM_FOLLOW;
    } else if (follow_model == "twitter_suggest") {
        return TWITTER_PREFERENTIAL_FOLLOW;
    } else if (follow_model == "agent") {
        return AGENT_FOLLOW;
    } else if (follow_model == "preferential_agent") {
        return PREFERENTIAL_AGENT_FOLLOW;
    } else if (follow_model == "hashtag") {
        return HASHTAG_FOLLOW;
    } else if (follow_model == "twitter" ) {
        return TWITTER_FOLLOW;
    } else {
        throw YAML::RepresentationException(node.GetMark(),
                format("'%s' is not a valid follow model!", follow_model.c_str()));
    }
}

static vector<PreferenceClass> parse_preference_classes(const Node& node) {
    const Node& pref_classes = node["preference_classes"];
    vector<PreferenceClass> ret;
    for (int i = 0; i < pref_classes.size(); i++) {
        PreferenceClass pref_class;
        parse(pref_classes[i], "name", pref_class.name);
        ret.push_back(pref_class);
    }
    return ret;
}

static vector<Ideology> parse_ideologies(const Node& node) {
    const Node& ideologies = node["ideologies"];
    vector<Ideology> ret;
    for (int i = 0; i < ideologies.size(); i++) {
        Ideology ideology;
        parse(ideologies[i], "name", ideology.name);
        ret.push_back(ideology);
    }
    return ret;
}

// Terminal case of vector nesting:
static void parse_vector(const Node& node, double& val, int tab =0) {
    node >> val;
}

template <typename T>
static void parse_vector(const Node& node, vector<T>& vec, int tab =0) {
    vec.resize(node.size());
    for (int i = 0; i < node.size(); i++) {
        parse_vector(node[i], vec[i], tab + 1);
    }
}

static TweetObservationPDF parse_tweet_obs_pdf(const Node& node) {
    const Node& tweet_obs = node["tweet_observation"];
    const Node& func = node["GENERATED"]["obs_function"];
    const Node& bin_bounds = node["GENERATED"]["obs_bin_bounds"];
    TweetObservationPDF ret; 

    parse(tweet_obs, "initial_resolution", ret.initial_resolution);
    parse_vector(func, ret.values);
    parse_vector(bin_bounds, ret.thresholds);

//    for (int i = 0; i < ret.values.size(); i++ ) {
//        printf("VALUE IS %g (< %g)\n", ret.values[i], ret.thresholds[i]);
//    }
    return ret;
}

static vector<double> parse_follow_weights(const Node& node) {
    vector<double> weights(N_TWITTER_FOLLOW_MODELS, 0);
    parse(node, "random", weights[RANDOM_FOLLOW]);
    parse(node, "twitter_suggest", weights[TWITTER_PREFERENTIAL_FOLLOW]);
    parse(node, "agent", weights[AGENT_FOLLOW]);
    parse(node, "preferential_agent", weights[PREFERENTIAL_AGENT_FOLLOW]);
    parse(node, "hashtag", weights[HASHTAG_FOLLOW]);
    double weight_sum = 0;
    for (auto& w : weights) {
        weight_sum += w;
    } for (auto& w : weights) {
        w /= weight_sum;
    }
    return weights;
}

static void truncate_max_sim_time(double& max_sim_time) {
    const double MAX_YEARS = 100;
    const double SECONDS_TO_YEAR = 60 * 60 * 24 * 365;
    if (max_sim_time > MAX_YEARS * SECONDS_TO_YEAR) {
        cout << "INFO: Simulation time given (" << (max_sim_time / SECONDS_TO_YEAR) << " years)";
        cout << " is more than " << MAX_YEARS << " years. Truncating.\n";
        max_sim_time = MAX_YEARS * SECONDS_TO_YEAR;
    }
}

static void parse_analysis_configuration(ParsedConfig& config, const Node& node) {
    parse(node, "max_agents", config.max_agents);
    parse(node, "initial_agents", config.initial_agents);
    parse(node, "max_time", config.max_sim_time);
    truncate_max_sim_time(config.max_sim_time);

    parse(node, "max_real_time", config.max_real_time);

    // Parse max analysis steps. Since YAML++ doesn't support 'long long' we store 
    // in a double first (the most precise data type available).
    double max_analysis_steps_holder = -1;
    parse(node, "max_analysis_steps", max_analysis_steps_holder);
    config.max_analysis_steps = max_analysis_steps_holder;

    parse(node, "enable_query_api", config.enable_query_api); 
    parse(node, "unfollow_tweet_rate", config.unfollow_tweet_rate);
    parse(node, "stage1_unfollow", config.stage1_unfollow);
    parse(node, "use_hashtag_probability", config.hashtag_prob);
    parse(node, "use_barabasi", config.use_barabasi);
    parse(node, "barabasi_connections", config.barabasi_connections);
    parse(node, "barabasi_exponent", config.barabasi_exponent);
    parse(node, "use_followback", config.use_followback);
    parse(node, "use_follow_via_retweets", config.use_follow_via_retweets);
    parse(node, "use_random_time_increment", config.use_random_time_increment);
    parse(node, "enable_interactive_mode", config.enable_interactive_mode);
    parse(node, "enable_lua_hooks", config.enable_lua_hooks);
    parse(node, "lua_script", config.lua_script);
    parse(node, "use_susceptibility", config.use_susceptibility);
    config.follow_model = parse_follow_model(node);
    const Node& follow_model = node["model_weights"];
    config.model_weights = parse_follow_weights(follow_model);
}

static Add_Rates parse_rates_configuration(ParsedConfig& config, const Node& node) {
    Add_Rates add_rates;
    parse(node, "function", add_rates.RF.function_type);
    if (add_rates.RF.function_type == "constant") {
        parse(node, "value", add_rates.RF.const_val);
    } else if (add_rates.RF.function_type == "linear") {
        parse(node, "y_intercept", add_rates.RF.y_intercept);
        parse(node, "slope", add_rates.RF.slope);
    } 
    return add_rates;
}

static void parse_output_configuration(ParsedConfig& config, const Node& node) {
    parse(node, "stdout_basic", config.output_stdout_basic);
    parse(node, "stdout_summary", config.output_stdout_summary);
    parse(node, "summary_output_rate_real_minutes", config.summary_output_rate_real_minutes);
    parse(node, "summary_output_rate", config.summary_output_rate);

    parse(node, "visualize", config.output_visualize);
    parse(node, "agent_stats", config.agent_stats);
    parse(node, "full_tweet_stats", config.full_tweet_stats);

    parse(node, "save_network_on_timeout", config.save_network_on_timeout);
    parse(node, "load_network_on_startup", config.load_network_on_startup);
    parse(node, "ignore_load_config_check", config.ignore_load_config_check);
    parse(node, "save_file", config.save_file);

    parse(node, "degree_distributions", config.degree_distributions);

    parse(node, "tweet_analysis", config.output_tweet_analysis);
    parse(node, "retweet_visualization", config.retweet_viz);
    parse(node, "main_statistics", config.main_stats);
    parse(node, "degree_distribution_by_follow_model", config.dd_by_follow_model);
    parse(node ,"region_connection_matrix", config.region_connection_matrix);
    parse(node ,"categories_distro", config.categories_distro);
    parse(node ,"most_popular_tweet_content", config.most_popular_tweet_content);
    parse(node ,"tweet_info", config.tweet_info);
}

static CategoryGrouper parse_category_thresholds(const Node& node) {
    CategoryGrouper group;
    string bin_spacing;
    int min_binsize, max_binsize, increment;
    parse(node, "bin_spacing", bin_spacing);
    parse(node, "min", min_binsize);
    parse(node, "max", max_binsize);
    parse(node, "increment", increment);
    vector<double> thresholds, weights;
    /* Initialize the thresholds and the weights */
    if (bin_spacing == "linear") {
        for (int i = min_binsize; i < max_binsize; i+= increment) {
            group.categories.push_back(CategoryAgentList(i, 0));
        }
    } else if (bin_spacing == "quadratic") {
        for (int i = min_binsize; i < max_binsize; i+= increment*increment) {
            group.categories.push_back(CategoryAgentList(i, 0));
        }
    } else if (bin_spacing == "cubic") {
        for (int i = min_binsize; i < max_binsize; i+= increment*increment*increment) {
            group.categories.push_back(CategoryAgentList(i, 0));
        }
    }
    group.categories.push_back(CategoryAgentList(HUGE_VAL, 0));
    return group;
}
void parse_category_weights(ParsedConfig& config, const Node& node, CategoryGrouper& group) {
    string bin_spacing;
    double min_binsize, max_binsize, increment;
    parse(node, "bin_spacing", bin_spacing);
    parse(node, "min", min_binsize);
    parse(node, "max", max_binsize);
    parse(node, "increment", increment);
    vector<double> weights;
    double total_weight = 0;
    /* Initialize the thresholds and the weights */
    if (bin_spacing == "linear") {
        for (int i = min_binsize, j = 0; i < max_binsize ; i+= increment, j++) {
            group.categories[j].prob = pow((double) i,config.barabasi_exponent);
            total_weight += i;
        }
        /* Normalize the weights into probabilities */
        if (total_weight > 0) {
            for (int i = 0; i < group.categories.size(); i++) {
                group.categories[i].prob /= total_weight;                
            }
        }
    } else if (bin_spacing == "quadratic") {
        for (int i = min_binsize; i < max_binsize; i+= increment*increment) {
            total_weight += i;
        }
        double threshold_count = (max_binsize - min_binsize) / (double) increment*increment;
        /* Normalize the weights into probabilities */
        if (total_weight > 0) {
            for (int i = 0; i < (int) threshold_count; i++) {
                group.categories[i].prob /= total_weight;
            }
        }
    } else if (bin_spacing == "cubic") {
        for (int i = min_binsize; i < max_binsize; i+= increment*increment*increment) {
            total_weight += i;
        }
        double threshold_count = (max_binsize - min_binsize) / (double) increment*increment*increment;
        /* Normalize the weights into probabilities */
        if (total_weight > 0) {
            for (int i = 0; i < (int) threshold_count; i++) {
                group.categories[i].prob /= total_weight;
            }
        }
    }
}
static void parse_category_configurations(ParsedConfig& config, const Node& node) {
    ASSERT(!config.agent_types.empty(), "Must have agent types!");
    if (config.use_barabasi) {
        // One category per potential agent:
        for (int i = 1; i < config.max_agents + 1; i ++) {
            CategoryAgentList cat(i-1, i);
            config.follow_ranks.categories.push_back(cat);
            config.follow_probabilities.push_back(pow(i,config.barabasi_exponent));
            for (int j = 0; j < config.agent_types.size(); j++ ) {
                AgentType& type = config.agent_types[j];
                type.follow_ranks.categories.push_back(cat);
            }
        }
    } else {   
        config.follow_ranks = parse_category_thresholds(node["follow_ranks"]["thresholds"]);                
        parse_category_weights(config, node["follow_ranks"]["weights"], config.follow_ranks);
        for (int i = 0; i < config.agent_types.size(); i ++) {
            config.agent_types[i].follow_ranks = parse_category_thresholds(node["follow_ranks"]["thresholds"]);                
            parse_category_weights(config, node["follow_ranks"]["weights"], config.agent_types[i].follow_ranks);
        }
    }
    config.tweet_ranks = parse_category_thresholds(node["tweet_ranks"]["thresholds"]);
    config.retweet_ranks = parse_category_thresholds(node["retweet_ranks"]["thresholds"]);
}

static void parse_agent_tweet_type_probabilities(AgentType& et, const Node& node) {
    double plain = 0, ideological = 0, musical = 0, humorous = 0;

    parse(node, "ideological", ideological, /*Optional:*/ true);
    parse(node, "plain", plain, /*Optional:*/ true);
    parse(node, "musical", musical, /*Optional:*/ true);
    parse(node, "humorous", humorous, /*Optional:*/ true);

    double sum = (ideological + plain + musical + humorous);

    // Must match up with definition of TweetType in config_static.h!
    double probs[N_TWEET_TYPES] = {plain / sum, ideological / sum, musical / sum, humorous / sum, /*Not valid:*/ 0};
    for (int i = 0; i < N_TWEET_TYPES; i++) {
        et.tweet_type_probs[i] = probs[i];
    }
}

static AgentTypeVector parse_agents_configuration(const Node& node) {
    AgentTypeVector vec;
    double add_total = 0, follow_total = 0;
    for (int i = 0; i < node.size(); i++) {
        AgentType et;
        //et.tweet_type_probs;
        const Node& child = node[i];
        parse(child, "name", et.name);
        parse(child, "followback_probability", et.prob_followback);
    	const Node& hashtag_options = child["hashtag_follow_options"];
        parse(hashtag_options, "care_about_region", et.care_about_region);
        parse(hashtag_options, "care_about_ideology", et.care_about_ideology);
        const Node& weights = child["weights"];
        parse(weights, "add", et.prob_add);
        parse(weights, "follow", et.prob_follow);
		const Node& follow_rate = child["rates"]["follow"];
		const Node& tweet_rate = child["rates"]["tweet"];
        parse(child, "susceptibility", et.susceptibility);

		parse_agent_tweet_type_probabilities(et, weights["tweet_type"]);
		
		parse(follow_rate, "function", et.RF[0].function_type);
		if (et.RF[0].function_type == "constant") {
			parse(follow_rate, "value", et.RF[0].const_val);
		} else if (et.RF[0].function_type == "linear") {
			parse(follow_rate, "slope", et.RF[0].slope);
			parse(follow_rate, "y_intercept", et.RF[0].y_intercept);
		}
		
		
		parse(tweet_rate, "function", et.RF[1].function_type);
		if (et.RF[1].function_type == "constant") {
			parse(tweet_rate, "value", et.RF[1].const_val);
		} else if (et.RF[1].function_type == "linear") {
			parse(tweet_rate, "slope", et.RF[1].slope);
			parse(tweet_rate, "y_intercept", et.RF[1].y_intercept);
		}
        
        add_total += et.prob_add, follow_total += et.prob_follow;
        vec.push_back(et);
    }
    // Normalize the agent weights into probabilities
    for (int i = 0; i < node.size(); i++) {
        vec[i].prob_add /= add_total;
        vec[i].prob_follow /= follow_total;
    }

    return vec;
}

static void parse_region(Regions& ret, const Node& node) {
    Region r;
    parse(node, "name", r.name);
    parse_vector(node["ideology_probs"], r.ideology_probs);
    parse_vector(node["language_probs"], r.language_probs);
    parse_vector(node["preference_class_probs"], r.preference_class_probs);

    double add_prob = -1;
    parse(node, "add_prob", add_prob);

    ret.add_probs.push_back(add_prob);
    ret.regions.push_back(r);
}
static Regions parse_regions(const Node& node) {
    Regions ret;
    for (int i = 0; i < node.size(); i++) {
        parse_region(ret, node[i]);
    }
    return ret;
}

static FollowerSet::WeightDeterminer parse_tweet_react_rates(const Node& n_root) {
    FollowerSet::WeightDeterminer w_root;
    ASSERT(n_root.size() <= N_BIN_PREFERENCE_CLASS, "Too many preference classes!");
    for (int i = 0; i < n_root.size(); i++) {
        auto& n_prefs = n_root[i];
        auto& w_prefs = w_root.weights[i];

        // Same region or not:
        ASSERT(n_prefs.size() == N_TWEET_TYPES, "Not enough tweet types in react table!");
        for (int i = 0; i < N_TWEET_TYPES; i++) {
            auto& n_etypes = n_prefs[i];
            auto& w_etypes = w_prefs[i];
            // Agent type bins:
            for (int i = 0; i < n_etypes.size(); i++) {
                n_etypes[i] >> w_etypes[i];
            }
        }
    }

    return w_root;
}

static void parse_all_configuration(ParsedConfig& config, const Node& node) {
    parse_analysis_configuration(config, node["analysis"]);
    config.pref_classes = parse_preference_classes(node);
    config.ideologies = parse_ideologies(node);
    config.tweet_obs = parse_tweet_obs_pdf(node);
    config.tweet_react_rates = parse_tweet_react_rates(node["GENERATED"]["tweet_react_table"]);

    config.regions = parse_regions(node["GENERATED"]["regions"]);
    config.add_rates = parse_rates_configuration(config, node["rates"]["add"]);
    parse_output_configuration(config, node["output"]);
    config.agent_types = parse_agents_configuration(node["agents"]);
    parse_category_configurations(config, node);
}

static void save_file_contents(ParsedConfig& config, const char* file_name) {
    // Re-read file, this time merely storing its entire contents
    // for sanity-check purposes on network resume:
    fstream file(file_name, fstream::in);
    string file_contents, line;
    while (getline(file, line)) {
        file_contents += line + "\n";
    }
    config.entire_config_file = file_contents;
}

static void check_integrity(const char* desc, const char* var, int limit, int val) {
    if (val > limit) {
        printf("%s must not be more than %s (%d)! Please adjust src/config_static.h.\n", desc, var, limit);
        throw "Error";
    }
}
// Does the configuration violate compile-time bounds?
static void check_configuration_integrity(ParsedConfig& config) {
    // These will always be true
    check_integrity("Number of preference classes",
            "N_BIN_PREFERENCE_CLASS", N_BIN_PREFERENCE_CLASS, config.pref_classes.size());
    check_integrity("Number of agent types",
                "N_BIN_AGENT_TYPES", N_BIN_AGENT_TYPES, config.agent_types.size());
    check_integrity("Number of regions",
                    "N_BIN_REGIONS", N_BIN_REGIONS, config.regions.regions.size());
    check_integrity("Number of ideologies",
                    "N_BIN_IDEOLOGIES", N_BIN_IDEOLOGIES, config.ideologies.size());
    if (config.enable_query_api && config.enable_interactive_mode) {
        printf("Cannot enable both query request mode and interactive mode!\n");
        throw "Error";
    }
}

/***************************************************************************
 * Fill the various Add_Rates structures with the data necessary for each month
 ***************************************************************************/

static void configure_add_rates(ParsedConfig& config) {
    int projected_months = config.max_sim_time / APPROX_MONTH;
    auto& add_rates = config.add_rates;
    if (add_rates.RF.function_type == "constant" ) {
        for (int i = 0; i <= projected_months; i ++) {
            add_rates.RF.monthly_rates.push_back(add_rates.RF.const_val);
        }
    } else if (add_rates.RF.function_type == "linear") {
        for (int i = 0; i <= projected_months; i ++) {
            add_rates.RF.monthly_rates.push_back(add_rates.RF.y_intercept + i*add_rates.RF.slope);
        }
    }
}

static void configure_agent_rates(ParsedConfig& config, AgentType& et, int event) {
    int projected_months = config.max_sim_time / APPROX_MONTH;
    if (et.RF[event].function_type == "not specified" && event == 0) {
        error_exit("FATAL: Follow rate function for agent \""+ et.name + "\" was not specified.");
    } else if (et.RF[event].function_type == "not specified" && event == 1) {
        error_exit("FATAL: Follow rate function for agent \""+ et.name + "\" was not specified.");
    } else if (et.RF[event].function_type == "constant") {
        ASSERT(et.RF[event].const_val >= 0, "Check your rates, one of them is < 0");
        for (int i = 0; i <= projected_months; i ++) {
            et.RF[event].monthly_rates.push_back(et.RF[event].const_val);
        }
    } else if (et.RF[event].function_type == "linear") {
        for (int i = 0; i <= projected_months; i ++) {
            if (et.RF[event].y_intercept + i * et.RF[event].slope >= 0) {
                et.RF[event].monthly_rates.push_back(et.RF[event].y_intercept + i * et.RF[event].slope);
            } else {
                et.RF[event].monthly_rates.push_back(0);
            }
        }
    } else if (et.RF[event].function_type == "twitter_follow") {
        // initial spike for the first 2 months
        et.RF[event].monthly_rates.push_back(7.44*0.0008298429200320164);
        et.RF[event].monthly_rates.push_back(7.44*0.0008298429200320164);
        for (int i = 3; i <= projected_months; i ++) {
            // gradual rate after the 2 months
            et.RF[event].monthly_rates.push_back(7.44*5.5360422914604546e-05);
        }
    } else if (et.RF[event].function_type == "quarter_twitter_follow") {
        // initial spike for the first 2 months
        et.RF[event].monthly_rates.push_back(7.44*0.25*0.0008298429200320164);
        et.RF[event].monthly_rates.push_back(7.44*0.25*0.0008298429200320164);
        for (int i = 3; i <= projected_months; i ++) {
            // gradual rate after the 2 months
            et.RF[event].monthly_rates.push_back(7.44*0.25*5.5360422914604546e-05);
        }
    } else {
        ASSERT(false, "Unexpected function type for follow rates!");
    }
}


static void configure_agent_rates(ParsedConfig& config) {
    for (int i = 0; i < config.agent_types.size(); i ++) {
        for (int j = 0; j < number_of_diff_events; j ++) {
            configure_agent_rates(config, config.agent_types[i], j);
        }
    }
}

ParsedConfig parse_yaml_configuration(const char* file_name) {
    try {
        fstream file(file_name, fstream::in);

        ParsedConfig config;
        Parser parser(file);
        Node root;
        parser.GetNextDocument(root);

        parse_all_configuration(config, root);
        file.close();
        save_file_contents(config, file_name);

        /* Fill the various Add_Rates structures with the data necessary for each month */
        configure_add_rates(config);
        configure_agent_rates(config);
        check_configuration_integrity(config);
        return config;
    } catch (const exception& e) {
        printf("Exception occurred while reading '%s': %s\n", file_name,
                e.what());
        throw;
    }
}
