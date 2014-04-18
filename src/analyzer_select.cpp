#include <iostream>
#include <iomanip>
#include <vector>

#include "analyzer.h"

using namespace std;

struct AnalyzerSelect {
    //** Note: Only use reference types here!!
    Network& network;
    ParsedConfig& config;
    //** Mind the StatE/StatS difference. They are entirely different structs.
    AnalysisState& state;
    NetworkStats& stats;
    EntityTypeVector& entity_types;
    MTwist& rng;
    // There are multiple 'Analyzer's, they each operate on parts of AnalysisState.
    AnalyzerSelect(AnalysisState& state) :
            network(state.network), state(state), stats(state.stats),
            config(state.config), entity_types(state.entity_types), rng(state.rng) {
    }

    vector<double>& selection_vector(EntityType& type, SelectionType event) {
        if (event == FOLLOW_SELECT) {
            return type.RF[0].monthly_rates;
        } else if (event == TWEET_SELECT) {
            return type.RF[1].monthly_rates;
        }
        throw "selection_vector: Logic Error";
    }

    double calc_rate_sum(SelectionType event) {
        double rate_sum = 0;
        if (stats.prob_add == 0) {
            for (int e = 0; e < entity_types.size(); e++) {
                vector<double>& vec = selection_vector(entity_types[e], event);
                rate_sum += vec[state.n_months()] * entity_types[e].prob_add;
            }
        } else {
            for (int e = 0; e < entity_types.size(); e++) {
                vector<double>& vec = selection_vector(entity_types[e], event);
                for (int i = 0; i <= state.n_months(); i++) {
                    rate_sum += vec[i] * entity_types[e].prob_add;
                }
            }
        }
        return rate_sum;
    }

    int CHECK(int entity) {
        DEBUG_CHECK(network.is_valid_id(entity), "Invalid entity selection!");
        return entity;
    }

    int entity_selection(EntityType& et, vector<double>& vec, double& rand_num, double rate_sum) {
        if (stats.prob_add == 0) {
            if (rand_num <= ((vec[state.n_months()] * et.prob_add) / rate_sum)) {
                int entity = rng.pick_random_uniform(et.entity_list);
                return CHECK(entity);
            } else {
                rand_num -= ((vec[state.n_months()] * et.prob_add) / rate_sum);
            }
        } else {
            vector<int>& e_vec = et.entity_cap;
            for (int i = 0, e_i = e_vec.size() - 1; i <= state.n_months(); i++, e_i--) {
                if (rand_num <= ((vec[i] * et.prob_add) / rate_sum) && i == 0) {
                    int entity = -1;
                    if (state.n_months() == 0) {
                        entity = et.entity_list[rng.rand_int(et.new_entities)];
                        DEBUG_CHECK(network.is_valid_id(entity), "Invalid entity selection!");
                    } else {
                        entity = et.entity_list[et.entity_list.size() - 1 - rng.rand_int(et.new_entities)];
                    }
                    return CHECK(entity);
                } else if (rand_num <= ((vec[i] * et.prob_add) / rate_sum)) {
                    int range_min = e_vec.at(e_i), range_max = e_vec.at(e_i + 1);
                    int entity = et.entity_list[rng.rand_int(range_min, range_max)];
                    return CHECK(entity);
                }
                rand_num -= ((vec[i] * et.prob_add)/ rate_sum);
            }
        }
        return -1;
    }

    int entity_selection(SelectionType event) {
        double rate_sum = calc_rate_sum(event);
        double rand_num = rng.rand_real_not0();
    
        for (int e = 0; e < entity_types.size(); e++) {
            vector<double>& vec = selection_vector(entity_types[e], event);
            int entity = entity_selection(entity_types[e], vec, rand_num, rate_sum);
            if (entity != -1) {
                return entity;
            }
        }   
    
    error_exit("Logic error selecting entity to act or target");
    return -1;
    }
};

int analyzer_select_entity(AnalysisState& state, SelectionType type) {
    AnalyzerSelect analyzer(state);
    return analyzer.entity_selection(type);
}
