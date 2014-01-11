#include <iostream>
#include "analyzer.h"

using namespace std;

struct Rates {
    double overall_follow_rate;
    double overall_tweet_rate;
    double overall_retweet_rate;
    Rates(double f, double t, double r) {
        overall_follow_rate = f;
        overall_tweet_rate = t;
        overall_retweet_rate = r;
    }
    void add(const Rates& o) {
        overall_follow_rate += o.overall_follow_rate;
        overall_tweet_rate += o.overall_tweet_rate;
        overall_retweet_rate += o.overall_retweet_rate;
    }
    double total_rate() {
        return overall_follow_rate + overall_tweet_rate + overall_retweet_rate;
    }
};

struct AnalyzerRates {
    //** Note: Only use reference types here!!
    Network& network;
    ParsedConfig& config;
    //** Mind the StatE/StatS difference. They are entirely different structs.
    AnalysisState& state;
    AnalysisStats& stats;
    EntityTypeVector& entity_types;
    // There are multiple 'Analyzer's, they each operate on parts of AnalysisState.
    AnalyzerRates(AnalysisState& state) :
            network(state.network), state(state), stats(state.stats),
            config(state.config), entity_types(state.entity_types) {
    }

    bool create_new_month_if_needed(EntityType& et) {
        int N = network.n_entities;
        int n_months = state.n_months();

        if (n_months >= et.entity_cap.size() || state.time == 0) {
            // set the thresholds for each entitytype -- they are the same for every entity
            et.age_ranks.categories.push_back(state.time);
            // for every entitytype, go through every entity that is of that entitytype and categorize them based on their creation time
            for (int i = 0; i < et.entity_list.size(); i++) {
                Entity& e = network[et.entity_list[i]];
                et.age_ranks.categorize(et.entity_list[i], e.creation_time);
            }

            CategoryEntityList& C = et.age_ranks.categories.at(n_months);
            et.entity_cap.push_back(C.entities.size());

            // CHANGE YOUR RATES ACCORDINGLY - they are constant right now
            double change_follow_rate = config.rate_follow;
            double change_tweet_rate = config.rate_tweet;
            double change_retweet_rate = config.rate_retweet;
            double change_add_rate = config.rate_add;

            //all users will have the same constant rates for now
            et.r_follow.push_back(change_follow_rate);
            et.r_tweet.push_back(change_tweet_rate);
            et.r_retweet.push_back(change_retweet_rate);

            ASSERT(change_follow_rate >= 0, "The follow rate can't be < 0");
            ASSERT(change_tweet_rate >= 0, "The tweet rate can't be < 0");
            ASSERT(change_retweet_rate >= 0, "The retweet rate can't be < 0");
            return true;
        }
        return false;
    }

    void create_new_months_if_needed() {
        bool crossed_month = false;
        for (int e = 0; e < entity_types.size(); e++) {
            EntityType& et = entity_types[e];
            crossed_month |= create_new_month_if_needed(et);
        }

        if (crossed_month && config.output_stdout_basic) {
            cout << "\nNumber of Months = " << state.n_months() << "\n\n";
        }
    }

    void update_rate(EntityType& et, vector<double>& vec, double& rate) {
        int n_months = state.n_months();
        // Iterate two vectors in opposite directions
        rate += et.new_entities * vec[0];
        for (int i = 1, e_i = n_months; i <= n_months; i++, e_i--) {
            rate += vec[i] * (et.entity_cap[e_i - 1] - et.entity_cap[e_i]);
        }
    }

    // after every iteration, make sure the rates are updated accordingly
    Rates set_rates(EntityType& et) {
        double overall_follow_rate = 0, overall_tweet_rate = 0,
                overall_retweet_rate = 0;
        et.new_entities = et.entity_list.size() - et.entity_cap.back();
        if (config.rate_add == 0) {
            overall_follow_rate += et.entity_list.size() * et.r_follow.back();
            overall_tweet_rate += et.entity_list.size() * et.r_tweet.back();
            overall_retweet_rate += et.entity_list.size() * et.r_retweet.back();
        } else {
            update_rate(et, et.r_follow, overall_follow_rate);
            update_rate(et, et.r_tweet, overall_tweet_rate);
            update_rate(et, et.r_retweet, overall_retweet_rate);
        }
        return Rates(overall_follow_rate, overall_tweet_rate, overall_retweet_rate);
    }

    void set_rates() {
        create_new_months_if_needed();

        Rates global(0, 0, 0);
        for (int e = 0; e < entity_types.size(); e++) {
            set_rates (entity_types[e]);
            Rates rates = set_rates(entity_types[e]);
            global.add(rates); // Sum the rates
        }

        stats.event_rate = config.rate_add + global.total_rate();

        // Normalize the rates
        stats.prob_add = config.rate_add / stats.event_rate;
        stats.prob_follow = global.overall_follow_rate / stats.event_rate;
        stats.prob_tweet = global.overall_tweet_rate / stats.event_rate;
        stats.prob_norm = global.overall_retweet_rate / stats.event_rate;
    }
};

void analyzer_rate_update(AnalysisState& state) {
    AnalyzerRates analyzer(state);
    analyzer.set_rates();
}