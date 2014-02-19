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
        tweet_bank.tree.update(state.time);
    }
    
    void update_all_retweets() {
        TweetBank& tweet_bank = state.tweet_bank;
        handle_old_tweet_and_retweet_IDs(tweet_bank);
    }
    
    double total_retweet_rate() {
        return state.tweet_bank.get_total_rate();
    }

    RetweetChoice retweet_entity_selection() {
        TweetBank& tweet_bank = state.tweet_bank;

        double rand_num = rng.rand_real_not0();
        double rate_total = total_retweet_rate();

        Tweet& tweet = tweet_bank.pick_random_weighted(rng);
        UsedEntities& used = tweet.content->used_entities;

        Entity& e = network[tweet.id_tweeter];
        int entity_retweeting = e.follower_set.pick_random_uniform(rng);

        if (used.find(entity_retweeting) == used.end()) {
            // Entity has NOT already retweeted this tweet
            used.insert(entity_retweeting);
            Entity& e = network[entity_retweeting];
            return RetweetChoice(tweet.content->id_original_author, entity_retweeting);
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
