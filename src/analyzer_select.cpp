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

    int agent_selection(SelectionType event) {
        // Depending on the event, use the time-dependent rate functions to
        // either select the appropriate agent to do a follow or tweet event.
        double action_prob = event == FOLLOW_SELECT ? stats.prob_follow : /*TWEET_SELECT*/ stats.prob_tweet;
        // Get the total event rate from the probability, using the total event rate used to calculate it.
        double total_rate = action_prob * stats.adjusted_event_rate;
        // Get a random number to decide where we fall in our monthly bins:
        double rand_num = rng.rand_real(total_rate);
        // Iterate over the different agent types and their respective monthly categorizations to determine
        // the appropriate agent to select:
        int t = 0;
        for (AgentType& agent_type : state.agent_types) {
            TimeBinnedAgentList& agents = agent_type.agents;
            for (int i = 0; i <= agents.last_seen_n_months; i++) {
                rand_num -= agents.month_rate(agent_type.RF[event], i);
                if (rand_num < ZEROTOL) {
                    // Select an agent from the month bin we landed in:
                    return agents.rate_agent_choice(rng, agent_type.RF[event], /*Month bin: */ i);
                }
            }
        }
        ASSERT(false, "Could not make agent selection!");
        return -1;
    }
};

int analyzer_select_agent(AnalysisState& state, SelectionType type) {
    PERF_TIMER();
    AnalyzerSelect analyzer(state);
    return analyzer.agent_selection(type);
}
