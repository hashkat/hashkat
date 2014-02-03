#include <iostream>
#include <iomanip>
#include <vector>

#include "analyzer.h"

using namespace std;

struct AnalyzerRetweet {
    //** Note: Only use reference types here!!
    Network& network;
    ParsedConfig& config;
    //** Mind the StatE/StatS difference. They are entirely different structs.
    AnalysisState& state;
    AnalysisStats& stats;
    EntityTypeVector& entity_types;
    MTwist& rng;
    // There are multiple 'Analyzer's, they each operate on parts of AnalysisState.
    AnalyzerRetweet(AnalysisState& state) :
            network(state.network), state(state), stats(state.stats),
            config(state.config), entity_types(state.entity_types), rng(state.rng) {
    }
    
    void handle_old_tweet_IDs() {
        for (int i = 0; i < network.recent_tweet_times.size(); i ++) {
            int entity_ID = network.recent_tweet_ID[i];
            Entity& et = network[entity_ID];
            if (state.time - network.recent_tweet_times[i] > et.decay_time) {
                network.recent_tweet_times.erase(network.recent_tweet_times.begin() + i);
                network.recent_tweet_ID.erase(network.recent_tweet_ID.begin() + i);
                et.follower_set_cp.clear();
            }
        }
    }
    
    void handle_active_tweet_IDs() {
        for (int i = 0; i < network.recent_tweet_ID.size(); i ++) {
            int entity_ID = network.recent_tweet_ID[i];    
            Entity& et = network[entity_ID];
            FollowerSet& fs = network.follower_set(entity_ID);
            if (fs.size() == 0 /* if their followerlist is 0, remove them from the list */) {
                network.recent_tweet_times.erase(network.recent_tweet_times.begin() + i);
                network.recent_tweet_ID.erase(network.recent_tweet_ID.begin() + i);
                et.follower_set_cp.clear();
            } else /* make the copy, so we can easily remove ID's without changing the network */ {
                for (FollowerSet::iterator it; fs.iterate(it);) {
                    // make the copy here
                    et.follower_set_cp.push_back(it.get());
                }
            }
        }
    }
    
    void sum_retweet_rates(Entity& et, double& rate_total) {
        double decay_rate = 1/(et.decay_time * et.follower_set_cp.size());
        rate_total += decay_rate * et.follower_set_cp.size();
    }
    
    double total_retweet_rate() {
        handle_old_tweet_IDs();
        handle_active_tweet_IDs();
        double rate_total = 0;
        for (int i = 0; i < network.recent_tweet_ID.size(); i ++) {
            int entity_ID = network.recent_tweet_ID[i];
            sum_retweet_rates(network[entity_ID], rate_total);
        }
        return rate_total;
    }
    
    int retweet_entity_selection() {
        double rate_sum = total_retweet_rate();        
        double rand_num = rng.rand_real_not0();
        
        for (int i = 0; i < network.recent_tweet_ID.size(); i ++) {
            int entity_ID = network.recent_tweet_ID[i];
            Entity& et = network[entity_ID];
            double decay_rate = 1/(et.decay_time * et.follower_set_cp.size());
            if (rand_num <= ( (decay_rate * et.follower_set_cp.size()) / rate_sum)) {
                int element = rng.rand_int(et.follower_set_cp.size());
                int entity = et.follower_set_cp[element];
                // now that this entity has been chosen to do something, remove them from the list
                et.follower_set_cp.erase(et.follower_set_cp.begin() + element);
                return entity;
            }
            rand_num -= ((decay_rate * et.follower_set_cp.size()) / rate_sum);
        }
        return -1;
    }

};

double analyzer_total_retweet_rate(AnalysisState& state) {
    AnalyzerRetweet analyzer(state);
    return analyzer.total_retweet_rate();
}

int analyzer_select_entity_retweet(AnalysisState& state, SelectionType type) {
    AnalyzerRetweet analyzer(state);
    return analyzer.retweet_entity_selection();
}
