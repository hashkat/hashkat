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
    AgentTypeVector& agent_types;
    MTwist& rng;
    // There are multiple 'Analyzer's, they each operate on parts of AnalysisState.
    AnalyzerSelect(AnalysisState& state) :
            network(state.network), state(state), stats(state.stats),
            config(state.config), agent_types(state.agent_types), rng(state.rng) {
    }

    vector<double>& selection_rate_vector(AgentType& type, SelectionType event) {
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
            for (AgentType& et : agent_types) {
                vector<double>& rates = selection_rate_vector(et, event);
                // TODO Comment Why is state.n_months() used?
                rate_sum += rates[state.n_months()] * et.prob_add;
            }
        } else {
            for (AgentType& et : agent_types) {
                vector<double>& rates = selection_rate_vector(et, event);
                for (int i = 0; i <= state.n_months(); i++) {
                    // TODO Comment Why is state.n_months() used?
                    rate_sum += rates[state.n_months()] * et.prob_add;
                }
            }
        }
        return rate_sum;
    }

    int CHECK(int agent) {
        DEBUG_CHECK(network.is_valid_id(agent), "Invalid agent selection!");
        return agent;
    }

    int agent_selection(AgentType& et, vector<double>& rates, double& rand_num, double rate_sum) {
        if (stats.prob_add == 0) {
            // If add rate is 0, we do not need to consider the different months of user addition
	    if (et.agent_list.empty()) {
		return -1;
            }
            int agent = rng.pick_random_uniform(et.agent_list);
            return CHECK(agent);
        } else {
            vector<int>& caps = et.agent_cap;
            int cap_i = caps.size() - 1; // Iterate backwards
            bool first_month = true;
            for (int i = 0; i <= state.n_months(); i++) {
                double adjusted_add_rate = (rates[i] * et.prob_add) / rate_sum;
                if (rand_num <= adjusted_add_rate) {
                    if (first_month) {
                        int agent = -1;
                        if (state.n_months() == 0) {
                            if (et.new_agents == 0) {
                                return -1;
                            }
                            agent = et.agent_list[rng.rand_int(et.new_agents)];
                            DEBUG_CHECK(network.is_valid_id(agent), "Invalid agent selection!");
                        } else {
                            if (et.agent_list.empty()) {
                                return -1;
                            }
                            agent = et.agent_list[et.agent_list.size() - 1 - rng.rand_int(et.new_agents)];
                        }
                        return CHECK(agent);
                    } else {
                        int range_min = caps.at(cap_i), range_max = caps.at(cap_i + 1);
                        if (range_min == range_max) {
                            return -1; // Have nothing to choose
                        }
                        int agent = et.agent_list[rng.rand_int(range_min, range_max)];
                        return CHECK(agent);
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

    int agent_selection(SelectionType event) {
        double rate_sum = calc_rate_sum(event);
        double rand_num = rng.rand_real_not0();

        for (int e = 0; e < agent_types.size(); e++) {
            if (rand_num < agent_types[e].prob_add) {
                vector<double>& rates = selection_rate_vector(agent_types[e], event);
                int agent = agent_selection(agent_types[e], rates, rand_num, rate_sum);
                if (agent != -1) {
                    return agent;
                }
            }
            rand_num -= agent_types[e].prob_add;
        }

        return -1;
    }
};

int analyzer_select_agent(AnalysisState& state, SelectionType type) {
    PERF_TIMER();
    AnalyzerSelect analyzer(state);
    return analyzer.agent_selection(type);
}
