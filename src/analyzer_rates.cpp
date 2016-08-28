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
    //** Mind the statE/statS difference. They are entirely different structs.
    AnalysisState& state;
    NetworkStats& stats;
    AgentTypeVector& agent_types;
    Add_Rates& add_rates;
    // There are multiple 'Analyzer's, they each operate on parts of AnalysisState.
    AnalyzerRates(AnalysisState& state) :
            network(state.network), state(state), stats(state.stats),
            config(state.config), agent_types(state.agent_types), add_rates(state.config.add_rates) {
    }
	
    bool create_new_months_if_needed(AgentType& et) {
        return et.agents.update_month(state.n_months());
    }

    void create_new_months_if_needed() {
        bool crossed_month = false;
        for (auto& et : agent_types) {
            crossed_month |= create_new_months_if_needed(et);
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

    // after every iteration, make sure the rates are updated accordingly
    Rates get_rates(AgentType& et) {
        double overall_follow_rate = et.agents.total_rate(et.RF[0]);
        double overall_tweet_rate = et.agents.total_rate(et.RF[1]);
        return Rates(overall_follow_rate, overall_tweet_rate);
    }

    void set_rates() {
        create_new_months_if_needed();
        
        Rates global(0, 0);
        for (AgentType& et : agent_types) {
            Rates rates = get_rates(et);
            global.add(rates); // Sum the rates
        }
        update_retweets(state);
        config.rate_add = add_rates.RF.monthly_rates[state.n_months()]; 
        double overall_retweet_rate = analyzer_total_retweet_rate(state);
        stats.event_rate = config.rate_add + global.total_rate() + overall_retweet_rate;
        stats.adjusted_event_rate = std::max(1.0 / MINIMUM_TIME_STEP, stats.event_rate);

        // Normalize the rates
        stats.prob_add = config.rate_add / stats.adjusted_event_rate;
        stats.prob_follow = global.overall_follow_rate / stats.adjusted_event_rate;
        stats.prob_tweet = global.overall_tweet_rate / stats.adjusted_event_rate;
        stats.prob_retweet = overall_retweet_rate / stats.adjusted_event_rate;
        // If adjusted_event_rate > event_rate, the remainder goes into the 'do nothing' block.
        stats.prob_do_nothing = std::max(0.0, (stats.adjusted_event_rate - stats.event_rate) / stats.adjusted_event_rate);
    }
};

void analyzer_rate_update(AnalysisState& state) {
    AnalyzerRates analyzer(state);
    analyzer.set_rates();
}
