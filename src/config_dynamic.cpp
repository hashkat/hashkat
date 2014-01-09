#include <cstdio>
#include <fstream>
#include <exception>

#include <lcommon/strformat.h>

#include <yaml-cpp/yaml.h>

#include "util.h"
#include "category.h"
#include "config_dynamic.h"

using namespace std;
using namespace YAML;

template<class T>
static inline void parse(const Node& node, const char* key, T& value) {
    if (node.FindValue(key)) {
        node[key] >> value;
    }
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
    } else if (follow_model == "retweet") {
        return RETWEET_FOLLOW;
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
    parse(node, "use_random_increment", config.use_random_increment);

    config.follow_model = parse_follow_model(node);
}

static void parse_rates_configuration(ParsedConfig& config, const Node& node) {
    parse(node, "add", config.rate_add);
    parse(node, "follow", config.rate_follow);
    parse(node, "tweet", config.rate_tweet);
    parse(node, "retweet", config.rate_retweet);
}

static void parse_output_configuration(ParsedConfig& config, const Node& node) {
    parse(node, "stdout_basic", config.output_stdout_basic);
    parse(node, "stdout_summary", config.output_stdout_summary);

    parse(node, "verbose", config.output_verbose);
    parse(node, "visualize", config.output_visualize);

    parse(node, "p_in", config.output_p_in);
    parse(node, "p_out", config.output_p_out);

    parse(node, "tweet_analysis", config.output_tweet_analysis);
    parse(node, "cumulative_analysis", config.output_cumulative_analysis);
}

static CategoryGrouper parse_category_type(const Node& node) {
    CategoryGrouper group;

    vector<double> thresholds, weights;
    parse(node, "thresholds", thresholds);

    double total_weight = 0;
    /* Initialize the thresholds and the weights */
    for (int i = 0; i < thresholds.size(); i++) {
        double w = (weights.size() > i) ? weights[i] : 0;
        total_weight += w;
        group.categories.push_back(CategoryEntityList(thresholds[i], w));
    }
    group.categories.push_back(CategoryEntityList(HUGE_VAL, 0));

    /* Normalize the weights into probabilities */
    if (total_weight > 0) {
        for (int i = 0; i < thresholds.size(); i++) {
            group.categories[i].prob /= total_weight;
        }
    }
    return group;
}
static void parse_category_configurations(ParsedConfig& config, const Node& node) {
    if (config.use_barabasi) {
        for (int i = 1; i < config.max_entities; i ++) {
            CategoryEntityList cat(i-1, double(i) / config.max_entities);
            config.follow_ranks.categories.push_back(cat);
        }
    } else {
        config.follow_ranks = parse_category_type(node["follow_ranks"]);
    }
    config.tweet_ranks = parse_category_type(node["tweet_ranks"]);
    config.retweet_ranks = parse_category_type(node["retweet_ranks"]);
}

static EntityTypeVector parse_entities_configuration(const Node& node) {
    EntityTypeVector vec;
    double add_total = 0, follow_total = 0;
    for (int i = 0; i < node.size(); i++) {
        EntityType et;
        const Node& child = node[i];
        parse(child, "name", et.name);
        const Node& weights = child["weights"];
        parse(weights, "add", et.prob_add);
        parse(weights, "follow", et.prob_follow);

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
    parse_rates_configuration(config, node["rates"]);
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
