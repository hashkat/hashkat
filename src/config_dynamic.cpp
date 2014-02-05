#include <cstdio>
#include <fstream>
#include <exception>

#include <lcommon/strformat.h>

#include <yaml-cpp/yaml.h>

#include "util.h"
#include "CategoryGrouper.h"
#include "config_dynamic.h"

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
static inline void parse_opt(const Node& node, const char* key, T& value, bool optional = false) {
    parse(node, key, value, true);
}


/* Convert from a text node to the number representing
 * the follow model. */
static FollowModel parse_follow_model(const Node& node) {
    std::string follow_model = "random";
    parse(node, "follow_model", follow_model);
    if (follow_model == "random") {
        return RANDOM_FOLLOW;
    } else if (follow_model == "preferential") {
        return PREFERENTIAL_FOLLOW;
    } else if (follow_model == "entity") {
        return ENTITY_FOLLOW;
    } else {
        throw YAML::RepresentationException(node.GetMark(),
                format("'%s' is not a valid follow model!", follow_model.c_str()));
    }
}

static void parse_analysis_configuration(ParsedConfig& config, const Node& node) {
    parse(node, "max_entities", config.max_entities);
    parse(node, "initial_entities", config.initial_entities);
    parse(node, "max_time", config.max_time);
    parse(node, "use_barabasi", config.use_barabasi);
    parse(node, "use_random_time_increment", config.use_random_time_increment);

    config.follow_model = parse_follow_model(node);
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

    parse(node, "verbose", config.output_verbose);
    parse(node, "visualize", config.output_visualize);

    parse(node, "degree_distributions", config.degree_distributions);

    parse(node, "tweet_analysis", config.output_tweet_analysis);
    parse(node, "cumulative_analysis", config.output_cumulative_analysis);
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
            group.categories.push_back(CategoryEntityList(i, 0));
        }
    } else if (bin_spacing == "quadratic") {
        for (int i = min_binsize; i < max_binsize; i+= increment*increment) {
            group.categories.push_back(CategoryEntityList(i, 0));
        }
    } else if (bin_spacing == "cubic") {
        for (int i = min_binsize; i < max_binsize; i+= increment*increment*increment) {
            group.categories.push_back(CategoryEntityList(i, 0));
        }
    }
    group.categories.push_back(CategoryEntityList(HUGE_VAL, 0));
    return group;
}
void parse_category_weights(const Node& node, CategoryGrouper& group) {
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
            group.categories[j].prob = (double) i;
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
    if (config.use_barabasi) {
        for (int i = 1; i < config.max_entities; i ++) {
            CategoryEntityList cat(i-1, i);
            config.follow_ranks.categories.push_back(cat);
            config.follow_probabilities.push_back(i);
        }
    } else {   
        config.follow_ranks = parse_category_thresholds(node["follow_ranks"]["thresholds"]);                
        parse_category_weights(node["follow_ranks"]["weights"], config.follow_ranks);
    }
    config.tweet_ranks = parse_category_thresholds(node["tweet_ranks"]["thresholds"]);
    config.retweet_ranks = parse_category_thresholds(node["retweet_ranks"]["thresholds"]);
}

static EntityTypeVector parse_entities_configuration(const Node& node) {
    EntityTypeVector vec;
    double add_total = 0, follow_total = 0;
    for (int i = 0; i < node.size(); i++) {
        EntityType et;
        const Node& child = node[i];
        parse(child, "name", et.name);
        parse(child, "followback_probability", et.prob_followback);
        const Node& weights = child["weights"];
        parse(weights, "add", et.prob_add);
        parse(weights, "follow", et.prob_follow);
		const Node& follow_rate = child["rates"]["follow"];
		const Node& tweet_rate = child["rates"]["tweet"];
		
		parse(follow_rate, "function", et.RF[1].function_type);
		if (et.RF[1].function_type == "constant") {
			parse(follow_rate, "value", et.RF[1].const_val);
		} else if (et.RF[1].function_type == "linear") {
			parse(follow_rate, "slope", et.RF[1].slope);
			parse(follow_rate, "y_intercept", et.RF[1].y_intercept);
		} else if (et.RF[1].function_type == "exponential") {
			parse(follow_rate, "amplitude", et.RF[1].amplitude);
			parse(follow_rate, "exp_factor", et.RF[1].exp_factor);
		} 
		
		parse(tweet_rate, "function", et.RF[2].function_type);
		if (et.RF[2].function_type == "constant") {
			parse(tweet_rate, "value", et.RF[2].const_val);
		} else if (et.RF[2].function_type == "linear") {
			parse(tweet_rate, "slope", et.RF[2].slope);
			parse(tweet_rate, "y_intercept", et.RF[2].y_intercept);
		} else if (et.RF[2].function_type == "exponential") {
			parse(tweet_rate, "amplitude", et.RF[2].amplitude);
			parse(tweet_rate, "exp_factor", et.RF[2].exp_factor);
		} 
        add_total += et.prob_add, follow_total += et.prob_follow;

        vec.push_back(et);
    }
    // Normalize the entity weights into probabilities
    for (int i = 0; i < node.size(); i++) {
        vec[i].prob_add /= add_total;
        vec[i].prob_follow /= follow_total;
    }
    return vec;
}

static void parse_all_configuration(ParsedConfig& config, const Node& node) {
    parse_analysis_configuration(config, node["analysis"]);
    config.add_rates = parse_rates_configuration(config, node["rates"]["add"]);
    parse_output_configuration(config, node["output"]);
    parse_category_configurations(config, node);
    config.entity_types = parse_entities_configuration(node["entities"]);
}

ParsedConfig parse_yaml_configuration(const char* file_name) {
    try {
        fstream file(file_name, fstream::in);

        ParsedConfig config;
        Parser parser(file);
        Node root;
        parser.GetNextDocument(root);

        parse_all_configuration(config, root);
        return config;
    } catch (const exception& e) {
        printf("Exception occurred while reading '%s': %s\n", file_name,
                e.what());
        throw;
    }
}
