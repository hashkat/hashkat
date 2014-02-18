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

    void handle_old_tweet_and_retweet_IDs(TweetBank& tweet_bank) {
        TweetList& arl = tweet_bank.active_retweet_list;
        
        for (int i = 0; i < arl.size(); i ++) {
            Tweet& ri = arl[i];
            int tweeter_id = ri.tweeter_id;
            if (tweet_bank.get_omega(ri.creation_time, state.time) == -1) {
                arl.erase(arl.begin() + i);
            }
        }
    }
    
    void handle_active_tweet_and_retweet_IDs(TweetBank& tweet_bank) {
        TweetList& arl = tweet_bank.active_retweet_list;

        for (int i = 0; i < arl.size(); i ++) {
            arl[i].updating_rate = arl[i].starting_rate * tweet_bank.get_omega(arl[i].creation_time, state.time);
        }
    }
    
    void update_all_retweets() {
        TweetBank& tweet_bank = network.tweet_bank;
        handle_old_tweet_and_retweet_IDs(tweet_bank);
        handle_active_tweet_and_retweet_IDs(tweet_bank);
    }
    
    double total_retweet_rate() {
        double sum = 0;
        TweetBank& tweet_bank = network.tweet_bank;
        TweetList& arl = tweet_bank.active_retweet_list;
        for (int i = 0; i < arl.size(); i ++) {
            Tweet& ri = arl[i]; 
            sum += ri.updating_rate;
        }
        return sum;
    }

    // Returns two values via refererence passing
    RetweetChoice retweet_entity_selection() {
        TweetBank& tweet_bank = network.tweet_bank;
        TweetList& arl = tweet_bank.active_retweet_list;

        double rand_num = rng.rand_real_not0();
        double rate_total = total_retweet_rate();

        for (int i = 0; i < arl.size(); i ++) {
            if (rand_num <= (arl[i].updating_rate / rate_total) ) {
                Entity& e = network[arl[i].tweeter_id];
                int entity_retweeting = e.follower_set.pick_random_uniform(rng);
                if (arl[i].used_entities.find(entity_retweeting) == arl[i].used_entities.end()) {
                    arl[i].used_entities.insert(entity_retweeting);
                    Entity& e = network[entity_retweeting];
                    return RetweetChoice(arl[i].content->author_id, entity_retweeting);
                }
            }
            rand_num -= (arl[i].updating_rate / rate_total);
        }
        return RetweetChoice(-1,-1);
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
RetweetChoice analyzer_select_entity_retweet(AnalysisState& state, SelectionType type) {
    AnalyzerRetweet analyzer(state);
    return analyzer.retweet_entity_selection();
}
