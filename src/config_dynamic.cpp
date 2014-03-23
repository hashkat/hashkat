#include <cstdio>
#include <fstream>
#include <exception>

#include <lcommon/strformat.h>

#include <yaml-cpp/yaml.h>

#include "util.h"

#include "CategoryGrouper.h"
#include "config_dynamic.h"
#include "entity.h"

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
    } else if (follow_model == "preferential-entity") {
        return PREFERENTIAL_ENTITY_FOLLOW;
    } else if (follow_model == "twitter" ) {
        return TWITTER_FOLLOW;
    } else {
        throw YAML::RepresentationException(node.GetMark(),
                format("'%s' is not a valid follow model!", follow_model.c_str()));
    }
}

static LanguageProbabilities parse_language_configuration(const Node& node) {
    map<int,string> lang_map;
    lang_map[LANG_ENGLISH] = "English";
    lang_map[LANG_FRENCH] = "French";
    lang_map[LANG_FRENCH_AND_ENGLISH] = "French+English";

    const Node& weights = node["weights"];
    LanguageProbabilities probs;
    double total = 0.0; // For normalization
    for (int i = 0; i < N_LANGS; i++) {
        string str = lang_map[i];
        parse(weights, str.c_str(), probs[i]);
        total += probs[i];
    }
    ASSERT(total > 0, "Total must be greater than 0");
    for (int i = 0; i < N_LANGS; i++) {
        probs[i] /= total; // Normalize
    }

    return probs;
}

static std::vector<EntityPreferenceClass> parse_preference_classes(const Node& node) {
    const Node& tweet_rel = node["tweet_relevance"];
    const Node& pref_classes = tweet_rel["preference_classes"];
    std::vector<EntityPreferenceClass> ret;
    for (int i = 0; i < pref_classes.size(); i++) {
        EntityPreferenceClass pref_class;
        parse(pref_classes[i], "name", pref_class.name);
        ret.push_back(pref_class);
    }
    return ret;
}

// Terminal case of vector nesting:
static void parse_vector(const Node& node, double& val, int tab =0) {
    node >> val;
}

template <typename T>
static void parse_vector(const Node& node, std::vector<T>& vec, int tab =0) {
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

    for (int i = 0; i < ret.values.size(); i++ ) {
        printf("VALUE IS %g (< %g)\n", ret.values[i], ret.thresholds[i]);
    }
    return ret;
}

static FollowerSetRatesDeterminer parse_tweet_react_rates(const Node& node) {
    // Nesting is as follows:
    // Each (entity-type X humour-bin) has its own rate object which has rates for:
    //  (language X distance X pref-class)
    typedef vector< vector< double > > entity_humour_rel_function_t;
    typedef vector<entity_humour_rel_function_t> entity_rel_function_t;
    typedef vector<entity_rel_function_t> rel_function_t;

    const Node& generated = node["GENERATED"];
    // Assumption: The vector we read from state.config.follower_rates
    // is in the same order as expected by FollowerSetRates (which is a category component type)

    rel_function_t input_func; // Multi-dimensional vector, see above
    parse_vector(generated["rel_function"], input_func   );

    // Rates for every entity-type X humour-bin
    vector< vector<FollowerSet::Rates> > total_rates;

    // Loop over entity-type layer:
    for (int i = 0; i < input_func.size(); i++) {
        vector<FollowerSet::Rates> evec;
        entity_rel_function_t& entity_func = input_func[i];

        // Loop over humour-bin layer:
        for (int j = 0; j < entity_func.size(); j++) {
            entity_humour_rel_function_t& eh_func = entity_func[j];
            FollowerSet::Rates react_rates;
            // Fill the same rates for every language (slightly memory inefficient):
            for (int k = 0; k < eh_func.size(); k++) {
                react_rates.get(/*Dummy value:*/react_rates, k).fill_rates(eh_func);
            }
//            react_rates.print(/*Dummy value:*/react_rates);
            evec.push_back(react_rates);
        }

        total_rates.push_back(evec);
    }
    return FollowerSetRatesDeterminer(total_rates);
}
static vector<double> parse_follow_weights(const Node& node) {
    vector<double> weights(N_FOLLOW_MODELS);
    parse(node, "random", weights[0]);
    parse(node, "preferential", weights[1]);
    parse(node, "entity", weights[2]);
    parse(node, "preferential_entity", weights[3]);
    double weight_sum = 0;
    for (auto& w : weights) {
        weight_sum += w;
    } for (auto& w : weights) {
        w /= weight_sum;
    }
    return weights;
}

static void parse_analysis_configuration(ParsedConfig& config, const Node& node) {
    parse(node, "max_entities", config.max_entities);
    parse(node, "initial_entities", config.initial_entities);
    parse(node, "max_time", config.max_sim_time);
    parse(node, "max_real_time", config.max_real_time);
    parse(node, "unfollow_tweet_rate", config.unfollow_tweet_rate);
    parse(node, "stage1_unfollow", config.stage1_unfollow);
    parse(node, "use_barabasi", config.use_barabasi);
    parse(node, "use_flawed_followback", config.use_flawed_followback);
    parse(node, "use_random_time_increment", config.use_random_time_increment);
    parse(node, "enable_interactive_mode", config.enable_interactive_mode);
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

    parse(node, "visualize", config.output_visualize);
    parse(node, "verbose", config.output_verbose);
    parse(node, "entity_stats", config.entity_stats);

    parse(node, "save_network_on_timeout", config.save_network_on_timeout);
    parse(node, "load_network_on_startup", config.load_network_on_startup);
    parse(node, "ignore_load_config_check", config.ignore_load_config_check);
    parse(node, "save_file", config.save_file);
    
    parse(node, "degree_distributions", config.degree_distributions);

    parse(node, "tweet_analysis", config.output_tweet_analysis);
    parse(node, "retweet_visualization", config.retweet_viz);
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
    ASSERT(!config.entity_types.empty(), "Must have entity types!");
    if (config.use_barabasi) {
        for (int i = 1; i < config.max_entities; i ++) {
            CategoryEntityList cat(i-1, i);
            config.follow_ranks.categories.push_back(cat);
            config.follow_probabilities.push_back(i);
            for (int j = 0; j < config.entity_types.size(); j++ ) {
                EntityType& type = config.entity_types[j];
                type.follow_ranks.categories.push_back(cat);
            }
        }
    } else {   
        config.follow_ranks = parse_category_thresholds(node["follow_ranks"]["thresholds"]);                
        parse_category_weights(node["follow_ranks"]["weights"], config.follow_ranks);
        for (int i = 0; i < config.entity_types.size(); i ++) {
            config.entity_types[i].follow_ranks = parse_category_thresholds(node["follow_ranks"]["thresholds"]);                
            parse_category_weights(node["follow_ranks"]["weights"], config.entity_types[i].follow_ranks);
        }
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
    // Normalize the entity weights into probabilities
    for (int i = 0; i < node.size(); i++) {
        vec[i].prob_add /= add_total;
        vec[i].prob_follow /= follow_total;
    }
    return vec;
}

static void parse_subregion(Region& r, const Node& node) {
    Subregion s;
    parse(node, "name", s.name);
    parse_vector(node["ideology_probs"], s.ideology_probs);
    parse_vector(node["language_probs"], s.language_probs);
    parse_vector(node["preference_class_probs"], s.preference_class_probs);

    double add_prob = -1;
    parse(node, "add_prob", add_prob);

    r.add_probs.push_back(add_prob);
    r.subregions.push_back(s);
}

static void parse_subregions(Region& r, const Node& node) {
    for (int i = 0; i < node.size(); i++) {
        parse_subregion(r, node[i]);
    }
}

static void parse_region(Regions& ret, const Node& node) {
    Region r;
    parse(node, "name", r.name);
    parse_subregions(r, node["subregions"]);

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

static void parse_all_configuration(ParsedConfig& config, const Node& node) {
    parse_analysis_configuration(config, node["analysis"]);
    config.lang_probs = parse_language_configuration(node["languages"]);
    config.pref_classes = parse_preference_classes(node);
    config.tweet_obs = parse_tweet_obs_pdf(node);
    config.follower_rates = parse_tweet_react_rates(node);

    config.regions = parse_regions(node["GENERATED"]["regions"]);
    config.add_rates = parse_rates_configuration(config, node["rates"]["add"]);
    parse_output_configuration(config, node["output"]);
    config.entity_types = parse_entities_configuration(node["entities"]);
    parse_category_configurations(config, node);
}

static void save_file_contents(ParsedConfig& config, const char* file_name) {
    // Re-read file, this time merely storing its entire contents
    // for sanity-check purposes on network resume:
    fstream file(file_name, fstream::in);
    string file_contents = "";
    string line;
    while (std::getline(file, line)) {
        file_contents += line + "\n";
    }
    config.entire_config_file = file_contents;
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
        return config;
    } catch (const exception& e) {
        printf("Exception occurred while reading '%s': %s\n", file_name,
                e.what());
        throw;
    }
}

// Declared in .h file
FollowerSet::Rates& FollowerSetRatesDeterminer::get_rates(Entity& entity) {
    return rates[entity.entity_type][entity.humour_bin];
}
