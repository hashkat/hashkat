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

struct AnalyzerRetweet {
    //** Note: Only use reference types here! State will not persist.
    Network& network;
    ParsedConfig& config;
    AnalysisState& state;
    NetworkStats& stats;
    AgentTypeVector& agent_types;
    MTwist& rng;
    // There are multiple 'Analyzer's, they each operate on parts of AnalysisState.
    AnalyzerRetweet(AnalysisState& state) :
            network(state.network), state(state), stats(state.stats),
            config(state.config), agent_types(state.agent_types), rng(state.rng) {
    }
    void update_all_retweets() {
        PERF_TIMER();
        TweetBank& tweet_bank = state.tweet_bank;

        tweet_bank.tree.update(state.time);
    }

    double total_retweet_rate() {
        return state.tweet_bank.get_total_rate();
    }

    RetweetChoice tweet_to_retweet_selection() {
        PERF_TIMER();
        TweetBank& tweet_bank = state.tweet_bank;

        Tweet& tweet = tweet_bank.pick_random_weighted(rng);
        UsedAgents& used = tweet.content->used_agents;

        Agent& e = network[tweet.id_tweeter];
        int agent_retweeting = -1;

        if (!e.follower_set.pick_random_weighted(rng, tweet.react_weights, agent_retweeting)) {
            return RetweetChoice();
        }

        // Bug fix for #155:
        // Do not retweet if was original author.
        // We could place the original author in used_agents, but this is more efficient.
        bool was_tweet_author = (tweet.content->id_original_author == agent_retweeting);
        if (!was_tweet_author && !used.contains(agent_retweeting)) {
            // Agent has NOT already retweeted this tweet
            used.insert(agent_retweeting);
            return RetweetChoice(
                tweet.content->id_original_author,
                agent_retweeting,
                tweet.id_tweeter,
                tweet.generation + 1,
                &tweet.content // Returning pointer is small optimization
            );
        }

        return RetweetChoice();
    }
};

void update_retweets(AnalysisState& state) {
    AnalyzerRetweet analyzer(state);
    analyzer.update_all_retweets();
}

double analyzer_total_retweet_rate(AnalysisState& state) {
    AnalyzerRetweet analyzer(state);
    return analyzer.total_retweet_rate();
}
RetweetChoice analyzer_select_tweet_to_retweet(AnalysisState& state) {
    AnalyzerRetweet analyzer(state);
    return analyzer.tweet_to_retweet_selection();
}
