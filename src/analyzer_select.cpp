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
 * the original authors (Isaac Tamblyn, Kevin Ryczko & Adam Domurad), as well, in the case of a derivation,
 * subsequent authors. 
 */

#include <iostream>
#include <iomanip>
#include <vector>

#include "analyzer.h"

using namespace std;

struct AnalyzerSelect {
    //** Note: Only use reference types here!!
    Network& network;
    ParsedConfig& config;
    AnalysisState& state;
    NetworkStats& stats;
    EntityTypeVector& entity_types;
    MTwist& rng;
    // There are multiple 'Analyzer's, they each operate on parts of AnalysisState.
    AnalyzerSelect(AnalysisState& state) :
            network(state.network), state(state), stats(state.stats),
            config(state.config), entity_types(state.entity_types), rng(state.rng) {
    }

    vector<double>& selection_rate_vector(EntityType& type, SelectionType event) {
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
            for (EntityType& et : entity_types) {
                vector<double>& rates = selection_rate_vector(et, event);
                // TODO Comment Why is state.n_months() used?
                rate_sum += rates[state.n_months()] * et.prob_add;
            }
        } else {
            for (EntityType& et : entity_types) {
                vector<double>& rates = selection_rate_vector(et, event);
                for (int i = 0; i <= state.n_months(); i++) {
                    // TODO Comment Why is state.n_months() used?
                    rate_sum += rates[state.n_months()] * et.prob_add;
                }
            }
        }
        return rate_sum;
    }

    int CHECK(int entity) {
        DEBUG_CHECK(network.is_valid_id(entity), "Invalid entity selection!");
        return entity;
    }

    int entity_selection(EntityType& et, vector<double>& rates, double& rand_num, double rate_sum) {
        if (stats.prob_add == 0) {
            // If add rate is 0, we do not need to consider the different months of user addition
            int entity = rng.pick_random_uniform(et.entity_list);
            return CHECK(entity);
        } else {
            vector<int>& caps = et.entity_cap;
            int cap_i = caps.size() - 1; // Iterate backwards
            bool first_month = true;
            for (int i = 0; i <= state.n_months(); i++) {
                double adjusted_add_rate = (rates[i] * et.prob_add) / rate_sum;
                if (rand_num <= adjusted_add_rate) {
                    if (first_month) {
                        int entity = -1;
                        if (state.n_months() == 0) {
                            if (et.new_entities == 0) {
                                return -1;
                            }
                            entity = et.entity_list[rng.rand_int(et.new_entities)];
                            DEBUG_CHECK(network.is_valid_id(entity), "Invalid entity selection!");
                        } else {
                            if (et.entity_list.empty()) {
                                return -1;
                            }
                            entity = et.entity_list[et.entity_list.size() - 1 - rng.rand_int(et.new_entities)];
                        }
                        return CHECK(entity);
                    } else {
                        int range_min = caps.at(cap_i), range_max = caps.at(cap_i + 1);
                        if (range_min == range_max) {
                            return -1; // Have nothing to choose
                        }
                        int entity = et.entity_list[rng.rand_int(range_min, range_max)];
                        return CHECK(entity);
                    }
                }
                // Since probabilities are not cumulative, we subtract the previous add rates.
                // This allows us to choose within the appropriate month.
                rand_num -= adjusted_add_rate;
                first_month = false;
                cap_i--;
            }
        }
        return -1;
    }

    int entity_selection(SelectionType event) {
        double rate_sum = calc_rate_sum(event);
        double rand_num = rng.rand_real_not0();

        for (int e = 0; e < entity_types.size(); e++) {
            if (rand_num < entity_types[e].prob_add) {
                vector<double>& rates = selection_rate_vector(entity_types[e], event);
                int entity = entity_selection(entity_types[e], rates, rand_num, rate_sum);
                if (entity != -1) {
                    return entity;
                }
            }
            rand_num -= entity_types[e].prob_add;
        }

        return -1;
    }
};

int analyzer_select_entity(AnalysisState& state, SelectionType type) {
    PERF_TIMER();
    AnalyzerSelect analyzer(state);
    return analyzer.entity_selection(type);
}
