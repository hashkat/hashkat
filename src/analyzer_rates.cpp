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

#include "analyzer.h"
#include "io.h"

using namespace std;

struct Rates {
	double overall_follow_rate;
    double overall_tweet_rate;
    Rates(double f, double t) {
		overall_follow_rate = f;
        overall_tweet_rate = t;
    }
    void add(const Rates& o) {
        overall_follow_rate += o.overall_follow_rate;
        overall_tweet_rate += o.overall_tweet_rate;
    }
    double total_rate() {
        return overall_follow_rate + overall_tweet_rate;
    }
};

struct AnalyzerRates {
    //** Note: Only use reference types here!!
    Network& network;
    ParsedConfig& config;
    //** Mind the StatE/StatS difference. They are entirely different structs.
    AnalysisState& state;
    NetworkStats& stats;
    AgentTypeVector& agent_types;
    Add_Rates& add_rates;
    // There are multiple 'Analyzer's, they each operate on parts of AnalysisState.
    AnalyzerRates(AnalysisState& state) :
            network(state.network), state(state), stats(state.stats),
            config(state.config), agent_types(state.agent_types), add_rates(state.config.add_rates) {
    }
	
    bool create_new_month_if_needed(AgentType& et) {
        int n_months = state.n_months();

        if (n_months == et.agent_cap.size() || state.time == 0) {
            // set the thresholds for each agenttype -- they are the same for every agent
            et.age_ranks.categories.push_back(state.time);
            // for every agenttype, go through every agent that is of that agenttype and categorize them based on their creation time
            for (int i = 0; i < et.agent_list.size(); i++) {
                Agent& e = network[et.agent_list[i]];
                et.age_ranks.categorize(et.agent_list[i], e.creation_time);
            }
            et.agent_cap.push_back(et.agent_list.size());
            return true;
        }
        return false;
    }

    void create_new_months_if_needed() {
        bool crossed_month = false;
        for (auto& et : agent_types) {
            crossed_month |= create_new_month_if_needed(et);
        }

        if (crossed_month && config.degree_distributions) {
            if (config.degree_distributions) {
                degree_distributions(network, state);
            }
            if (config.region_connection_matrix) {
                region_stats(network, state);
            }
            

            //fraction_of_connections_distro(network, state, stats);
            
            //cout << "\nNumber of Months = " << state.n_months() << "\n\n";
        }
    }

    void update_rate(AgentType& et, vector<double>& vec, double& rate) {
        int n_months = state.n_months();
        // Iterate two vectors in opposite directions
        rate += et.new_agents * vec[0];
        for (int i = 1, e_i = n_months; i <= n_months; i++, e_i--) {
            rate += vec[i] * (et.agent_cap[e_i] - et.agent_cap[e_i - 1]);
        }
    }

    // after every iteration, make sure the rates are updated accordingly
    Rates set_rates(AgentType& et) {
        double overall_follow_rate = 0, overall_tweet_rate = 0; //,
                //overall_retweet_rate = 0;
        et.new_agents = et.agent_list.size() - et.agent_cap.back();
        if (config.rate_add == 0) {
            overall_follow_rate += et.agent_list.size() * et.RF[0].monthly_rates[state.n_months()];
            overall_tweet_rate += et.agent_list.size() * et.RF[1].monthly_rates[state.n_months()];
        } else {
            update_rate(et, et.RF[0].monthly_rates, overall_follow_rate);
            update_rate(et, et.RF[1].monthly_rates, overall_tweet_rate);
        }
        return Rates(overall_follow_rate, overall_tweet_rate);
    }

    void set_rates() {
        create_new_months_if_needed();
        
        Rates global(0, 0);
        for (auto& et : agent_types) {
            set_rates (et);
            Rates rates = set_rates(et);
            global.add(rates); // Sum the rates
        }
        update_retweets(state);
        config.rate_add = add_rates.RF.monthly_rates[state.n_months()]; 
        double overall_retweet_rate = analyzer_total_retweet_rate(state);
        stats.event_rate = config.rate_add + global.total_rate() + overall_retweet_rate;

        // Normalize the rates
        stats.prob_add = config.rate_add / stats.event_rate;
        stats.prob_follow = global.overall_follow_rate / stats.event_rate;
        stats.prob_tweet = global.overall_tweet_rate / stats.event_rate;
        stats.prob_retweet = overall_retweet_rate / stats.event_rate;
    }
};

void analyzer_rate_update(AnalysisState& state) {
    AnalyzerRates analyzer(state);
    analyzer.set_rates();
}
