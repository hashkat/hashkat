///////////////////////////////////////////////////////////////////////////////
// This file is part of the #KAT Social Network Simulator.
//
// The #KAT Social Network Simulator is free software: you can redistribute it
// and/or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// The #KAT Social Network Simulator is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// the #KAT Social Network Simulator. If not, see http://www.gnu.org/licenses.
//
// Addendum:
//
// Under this license, derivations of the #KAT Social Network Simulator
// typically must be provided in source form. The #KAT Social Network Simulator
// and derivations thereof may be relicensed by decision of the original
// authors (Kevin Ryczko & Adam Domurad, Isaac Tamblyn), as well, in the case
// of a derivation, subsequent authors.
//

#ifndef HASHKAT_AGENT_H_
#define HASHKAT_AGENT_H_

#include <string>
#include <vector>
#include <memory>

#include "CategoryGrouper.h"

#include "mtwist.h"

#include "events.h"

#include "FollowingSet.h"
#include "FollowerSet.h"

#include "tweets.h"
#include "serialization.h"

#include "lcommon/Range.h"

// Forward declare, to prevent circular header inclusion:
struct AnalysisState;

struct Agent {
    // Storing the id is somewhat redundant (since it can be inferred from Agent address), but provides convenient handling
    int id = -1;
    int agent_type = -1;
    int preference_class = -1;
    int n_tweets = 0, n_retweets = 0;

    // Abstract location:
    int region_bin = -1;

    double ideology_tweet_percent = 0;
    double creation_time = 0;

    // this is the average chatiness of the agents following list
    double avg_chatiness = 0.0;
    Language language = (Language)-1;

   // Abstract ideology:
    int ideology_bin = -1;

    //Introducing Susceptibility
    double susceptibility = 1.0;
    
    // list of flagged chatty people
    std::vector<int> chatty_agents;

    // Store the two directions of the follow relationship
    FollowingSet following_set;
    FollowerSet follower_set;

    // these store how someone followed you, or how you followed someone
    std::vector<int> following_method_counts = std::vector<int>(N_FOLLOW_MODELS);
    std::vector<int> follower_method_counts = std::vector<int>(N_FOLLOW_MODELS);

    // For streaming agent data in JSON
    void api_serialize(cereal::JSONOutputArchive& ar, bool serialize_follow_sets = false);

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(NVP(id), NVP(agent_type), NVP(preference_class)
          , NVP(n_tweets), NVP(n_retweets)
          , NVP(region_bin)
          , NVP(ideology_tweet_percent), NVP(creation_time)
          , NVP(avg_chatiness)
          , NVP(language)
          , NVP(ideology_bin)
          , NVP(chatty_agents)
          , NVP(following_set)
          , NVP(follower_set)
          , NVP(following_method_counts)
          , NVP(follower_method_counts));
        check_magic(ar, 0x4444);
    }

    void post_load(AnalysisState& state) {
        follower_set.post_load(state);
        following_set.post_load(state);
    }
};

struct AgentStats {
    int64 n_follows = 0, n_followers = 0, n_tweets = 0, n_original_tweets = 0, n_retweets = 0, n_unfollows = 0;
    int64 n_followback = 0;
    int64 n_random_follows = 0, n_preferential_follows = 0;
    int64 n_agent_follows = 0, n_pref_agent_follows = 0;
    int64 n_retweet_follows = 0, n_hashtag_follows = 0;
    int64 n_hashtags = 0;
    template <typename Archive>
    void serialize(Archive& ar) {
        ar(NVP(n_follows), NVP(n_followers), NVP(n_tweets), 
           NVP(n_original_tweets), NVP(n_retweets), NVP(n_unfollows));
        ar(NVP(n_followback));
        ar(NVP(n_random_follows), NVP(n_preferential_follows));
        ar(NVP(n_agent_follows), NVP(n_pref_agent_follows));
        ar(NVP(n_retweet_follows), NVP(n_hashtag_follows));
        ar(NVP(n_hashtags));
    }
};

/* Stores an agent ID list for an agent type, along with 
 * the categorization into different time bins.
 */
struct TimeBinnedAgentList {
    // The agents in this agent type:
    std::vector<int> agent_ids;
    // The max index of agents for each discrete value of time (i.e., monthly bins)
    // in 'agent_ids'. Implicitly partitions 'agent_ids' into time sections.
    std::vector<int> agent_ids_time_caps;
    size_t last_seen_n_months = 0;

    // Update the month categorizations, returning whether a month was made:
    bool update_month(size_t n_months) {
        this->last_seen_n_months = n_months;
        bool made_new_month = false;
        while (n_months > agent_ids_time_caps.size()) { 
            // set the threshold defining this month:
            agent_ids_time_caps.push_back(agent_ids.size());
            made_new_month = true;
        }
        return made_new_month;
    }

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(NVP(agent_ids), NVP(agent_ids_time_caps), NVP(last_seen_n_months));
    }

    Range month_range(int i) {
        size_t previous_cap = (i == 0) ? 0 : agent_ids_time_caps[i - 1];
        size_t current_cap = (i == last_seen_n_months) ? agent_ids.size() : agent_ids_time_caps[i];
        return Range(previous_cap, current_cap);
    }

    double month_rate(Rate_Function& rf, int i) {
        double rate_for_month = rf.monthly_rates.at(last_seen_n_months - i);
        Range range = month_range(i);
        return (range.max - range.min) * rate_for_month;
    }

    double total_rate(Rate_Function& rf) {
        double rate_sum = 0.0;
        for (int i = 0; i <= last_seen_n_months; i++) {
            rate_sum += month_rate(rf, i);
        }
        return rate_sum;
    }

    int rate_month_choice(MTwist& rng, Rate_Function& rf) {
        double rate_sum = total_rate(rf) * rng.rand_real_not1();
        for (int i = 0; i <= last_seen_n_months; i++) {
            rate_sum -= month_rate(rf, i);
            if (rate_sum < ZEROTOL) {
                return i;
            }
        }
        ASSERT(false, "Should choose one of the months!");
        return -1;
    }

    int rate_agent_choice(MTwist& rng, Rate_Function& rf, int month_id) {
//        int i = rate_month_choice(rng, rf);
        size_t previous_cap = (month_id == 0) ? 0 : agent_ids_time_caps[month_id - 1];
        size_t current_cap = (month_id == last_seen_n_months) ? agent_ids.size() : agent_ids_time_caps[month_id];
        return agent_ids[rng.rand_int(previous_cap, current_cap)];
    }
};

/*
 * An agent type is a static class of agent, determined at creation.
 * Agent types are intended to represent different kinds of network participants,
 *
 * For a network like Twitter, example agent types include 'Standard User', 'Celebrity', etc.
 */
struct AgentType {
    std::string name;
    double prob_add = 0; // When an agent is added, how likely is it that it is this agent type ?
    double prob_follow = 0; // When an agent is followed, how likely is it that it is this agent type ?
    double prob_followback = 0;
    Rate_Function RF[number_of_diff_events];
    bool care_about_region = false, care_about_ideology = false;
    TimeBinnedAgentList agents;
    CategoryGrouper follow_ranks;
    std::vector<double> updating_probs;
    double tweet_type_probs[N_TWEET_TYPES];

    //Introducing Susceptibility
    double susceptibility = 1.0;

    AgentStats stats;

    /* Synchronize rates and preferences from a loaded configuration.
     * This is done because, although we can load a new configuration,
     * some rates remain duplicated in our state object. */
    void sync_configuration(AgentType& E) {
        ASSERT(name == E.name, "Attempting sync_configuration on different agent types! The number and relative orderings of agent types must remain constant.");
        prob_add = E.prob_add;
        prob_follow = E.prob_follow;
        prob_followback = E.prob_followback;
        for (int i = 0; i < number_of_diff_events; i++) {
            RF[i] = E.RF[i];
        }

        // Created on demand, not data (in other words, do not uncomment):
        follow_ranks.sync_rates(E.follow_ranks);

        for (int i = 0; i < N_TWEET_TYPES; i++) {
            tweet_type_probs[i] = E.tweet_type_probs[i];
        }
    }

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(NVP(name), NVP(prob_add), NVP(prob_follow), NVP(prob_followback));
        for (auto& rf : RF) {
            ar(rf);
        }
        ar(NVP(care_about_region), NVP(care_about_ideology));
        ar(NVP(agents));
        ar(NVP(follow_ranks));
        ar(NVP(updating_probs), NVP(stats));
        for (auto& ttp : tweet_type_probs) {
            ar(ttp);
        }
    	check_magic(ar, 0x5555);
    }
};

// 'AgentTypeVector' acts exactly as a 'vector'
typedef std::vector<AgentType> AgentTypeVector;


#endif  // HASHKAT_AGENT_H_
