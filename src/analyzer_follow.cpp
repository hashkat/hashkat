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

#include <algorithm>
#include <iostream>
#include <iomanip>

#include "lcommon/perf_timer.h"

#include "events.h"
#include "analyzer.h"

using namespace std;

struct AnalyzerFollow {
    //** Note: Only use reference types here! State will not persist.
    Network& network;
    ParsedConfig& config;
    AnalysisState& state;
    NetworkStats& stats;
    CategoryGrouper& follow_ranks;
    vector<double>& follow_probabilities;
    vector<double>& updating_follow_probabilities;

    AgentTypeVector& agent_types;
    MTwist& rng;
    HashTags& hashtags;
    // There are multiple 'Analyzer's, they each operate on parts of AnalysisState.
    AnalyzerFollow(AnalysisState& state) :
            network(state.network), state(state), stats(state.stats),
            config(state.config), follow_ranks(state.follow_ranks),
            follow_probabilities(state.config.follow_probabilities),
            updating_follow_probabilities(state.updating_follow_probabilities),
            agent_types(state.agent_types), rng(state.rng), hashtags(state.hashtags) {
    }

   /***************************************************************************
    * Agent mutation routines
    ***************************************************************************/
 
    void flag_chatty_agent(Agent& actor, int id_target) {
        actor.chatty_agents.push_back(id_target);
    }
    
    void update_chatiness(Agent& actor, int id_target) {
       double targets_chatiness = agent_types[network[id_target].agent_type].RF[1].const_val;
       // arbitrary factor greater than the average chatiness
       actor.avg_chatiness = (actor.avg_chatiness*(actor.following_set.size() - 1) + targets_chatiness) / (double) actor.following_set.size();       
       if (actor.avg_chatiness*2 < targets_chatiness && actor.following_set.size() != 0) {
           flag_chatty_agent(actor, id_target);
           // reset the average chattiness
           actor.avg_chatiness = (actor.avg_chatiness*(actor.following_set.size() - 1) + targets_chatiness) / (double) actor.following_set.size();
       }
    }
    // Returns true if a follow is added that was not already added
   bool handle_follow(int id_actor, int id_target, int follow_method) {
       DEBUG_CHECK(follow_method >= 0 && follow_method < N_FOLLOW_MODELS,
           "Follow method must be a known method other than the compound Twitter model");
       PERF_TIMER();
       Agent& A = network[id_actor];
       Agent& T = network[id_target];
       bool was_added = A.following_set.add(state, id_target);
       // if the follow is possible
       if (was_added) {
           bool was_added = T.follower_set.add(network[id_actor]);
           A.follower_method_counts[follow_method]++;
           T.following_method_counts[follow_method]++;
           ASSERT(was_added, "Follow/follower-set asymmetry detected!");
           if (config.stage1_unfollow) {
               update_chatiness(A, id_target);
           }
           lua_hook_follow(state, id_actor, id_target);
           RECORD_STAT(state, A.agent_type, n_follows);
           RECORD_STAT(state, T.agent_type, n_followers);
           return true;
       }
       return false;
   }

   /***************************************************************************
    * Agent observation routines
    ***************************************************************************/
    
   int random_follow_method(Agent& e, int n_agents) {
       RECORD_STAT(state, e.agent_type, n_random_follows);
       return rng.rand_int(n_agents);
   }
   
   double preferential_weight() {
       double rand_num = rng.rand_real_not0();
       double prob_sum = 0;
       auto& updating_probs = updating_follow_probabilities;
       
       updating_probs.resize(follow_ranks.categories.size());
       for (int i = 0; i < follow_ranks.categories.size(); i ++) {
           CategoryAgentList& C = follow_ranks.categories[i];
           updating_probs[i] = follow_ranks.categories[i].prob * C.agents.size();
           prob_sum += follow_ranks.categories[i].prob * C.agents.size();
       }
       for (auto& p : updating_probs) {
           p /= prob_sum;
       }
       int i = 0;
       double val = 0;
       for (auto& p : updating_probs) {
           if (rand_num <= p) {
               val = p;
               break;
           }
           rand_num -= p;
           i++;
       }
       return val;
   }

   int preferential_barabasi_follow_method() {
       PERF_TIMER();

       double rand_num = rng.rand_real_not0();
       // if we want to use a preferential follow method
       int agent_to_follow = -1;
       double sum_of_weights = 0;
       updating_follow_probabilities.resize(follow_probabilities.size());
       /* search through the probabilities for each threshold and find
       the right bin to land in */

       for (int i = 0; i < follow_probabilities.size(); i++){
           // look at each category
           CategoryAgentList& C = follow_ranks.categories[i];
           updating_follow_probabilities[i] = follow_probabilities[i] * C.agents.size();
           sum_of_weights += C.agents.size()*follow_probabilities[i];
       }
       for (int i = 0; i < follow_probabilities.size(); i++){
           updating_follow_probabilities[i] /= sum_of_weights;
       }

       for (int i = 0; i < updating_follow_probabilities.size(); i++) {
           if (rand_num <= updating_follow_probabilities[i]) {
               // point to the category we landed in
               CategoryAgentList& C = follow_ranks.categories[i];
               // make sure we're not pulling a agent from an empty list
               if (C.agents.size() != 0) {
                   // pull a random agent from whatever bin we landed in and break so we do not continue this loop
                   agent_to_follow = C.agents[rng.rand_int(C.agents.size())];
                   return agent_to_follow;
               }
           }
           // part of the above search
           rand_num -= updating_follow_probabilities[i];
       }
       return -1;
   }

   int twitter_preferential_follow_method(Agent& e, double time_of_follow) {
       PERF_TIMER();
       RECORD_STAT(state, e.agent_type, n_preferential_follows);
       int referral_bin = (time_of_follow - e.creation_time ) / (double) APPROX_MONTH;
       double follow_prob = config.referral_rate_function.monthly_rates[referral_bin];
       if (!rng.random_chance(follow_prob)) {
           return -1;
       }
       
       double rand_num = rng.rand_real_not0();
       double prob_sum = 0;
       auto& updating_probs = updating_follow_probabilities;
       
       updating_probs.resize(follow_ranks.categories.size());
       for (int i = 0; i < follow_ranks.categories.size(); i ++) {
           CategoryAgentList& C = follow_ranks.categories[i];
           updating_probs[i] = follow_ranks.categories[i].prob * C.agents.size();
           prob_sum += follow_ranks.categories[i].prob * C.agents.size();
       }
       for (auto& p : updating_probs) {
           p /= prob_sum;
       }
       int i = 0;
       for (auto& p : updating_probs) {
           if (rand_num <= p) {
               CategoryAgentList& C = follow_ranks.categories[i];
               // pull a random agent from whatever bin we landed in and break so we do not continue this loop
               int agent_to_follow = C.agents[rng.rand_int(C.agents.size())];
               return agent_to_follow;
           }
           rand_num -= p;
           i++;
       }
       return -1;
   }

   int agent_follow_method(Agent& e) {
       PERF_TIMER();

       // if we want to follow by agent class
       /* search through the probabilities for each agent and find the right bin to land in */
       double rand_num = rng.rand_real_not0();
       for (auto& type : agent_types) {
           if (rand_num <= type.prob_follow) {
               // make sure we're not pulling from an empty list
               if (type.agent_list.size() != 0) {
                   // pull the agent from whatever bin we landed in and break so we dont continue this loop
                   int n = rng.rand_int(type.agent_list.size());
                   int agent_to_follow = type.agent_list[n];
                   Agent& try_agent = network[agent_to_follow];
                   if (try_agent.language != e.language) {
                       return -1;
                   }
                   RECORD_STAT(state, e.agent_type, n_agent_follows);
                   return agent_to_follow;
               }
           }
           // part of the above search
           rand_num -= type.prob_follow;
       }
       return -1;
   }
   int preferential_agent_follow_method(Agent& e) {
       PERF_TIMER();

       int agent_to_follow = -1;
       double rand_num = rng.rand_real_not0();
       for (int i = 0; i < agent_types.size(); i++) {
           if (rand_num <= agent_types[i].prob_follow) {

               double another_rand_num = rng.rand_real_not0();
               // make sure we're not pulling from an empty list
               AgentType& et = agent_types[i];
               vector<double>& up = et.updating_probs;
               up.resize(et.follow_ranks.categories.size());
               double prob_sum = 0;
               for (int j = 0; j < et.follow_ranks.categories.size(); j ++) {
                   CategoryAgentList& C = et.follow_ranks.categories[j];
                   up[j] = et.follow_ranks.categories[j].prob * C.agents.size();
                   prob_sum += et.follow_ranks.categories[j].prob * C.agents.size();
               }
               for (int j = 0; j < up.size(); j ++) {                        
                   up[j] /= prob_sum;
               }
               for (int j = 0; j < up.size(); j ++) {
                   if (another_rand_num <= up[j]) {
                       CategoryAgentList& C = et.follow_ranks.categories[j];
                       // pull a random agent from whatever bin we landed in and break so we do not continue this loop                            
                       agent_to_follow = C.agents[rng.rand_int(C.agents.size())];
                       break;
                   }
                   another_rand_num -= up[j];
               }
           }
           if (agent_to_follow != -1){
               Agent& try_agent = network[agent_to_follow];
               if (try_agent.language != e.language) {
                   return -1;
               }
               RECORD_STAT(state, e.agent_type, n_pref_agent_follows);
               return agent_to_follow;
           }
           // part of the above search
           rand_num -= agent_types[i].prob_follow;
       }
       return -1;
   }
   
   int hashtag_follow_method(Agent& e) {
       PERF_TIMER();
       bool region_choice = agent_types[e.agent_type].care_about_region;
       bool ideology_choice = agent_types[e.agent_type].care_about_ideology;
       int default_region = e.region_bin;
       int default_ideology = e.ideology_bin;
       int agent_to_follow = hashtags.select_agent(state, region_choice, ideology_choice, default_region, default_ideology);
       if (agent_to_follow != -1)
           RECORD_STAT(state, e.agent_type, n_hashtag_follows);
       return agent_to_follow;
   }

   // Used with the 'twitter' follow model (an option in INFILE.yaml).
   // Returns the agent followed by a chosen follow model.
   // 'model_chosen' is updated to the model randomly chosen according to a set of configured weights.
   int twitter_follow_model(Agent& e, double time_of_follow, /*Updated after call: */ FollowModel& model_chosen) {
       PERF_TIMER();
       model_chosen = (FollowModel) rng.kmc_select(&config.model_weights[0], N_TWITTER_FOLLOW_MODELS);
       if (model_chosen == RANDOM_FOLLOW) {
           return random_follow_method(e, network.size());
       } else if (model_chosen == TWITTER_PREFERENTIAL_FOLLOW) {
           return twitter_preferential_follow_method(e, time_of_follow);
       } else if (model_chosen == AGENT_FOLLOW) {
           return agent_follow_method(e);
       } else if (model_chosen == PREFERENTIAL_AGENT_FOLLOW) {
           return preferential_agent_follow_method(e);
       } else if (model_chosen == HASHTAG_FOLLOW){
           return hashtag_follow_method(e);
       } 
       ASSERT(false, "Should not pick ");
       return -1;
    }
    
   // Returns false to signify that nothing occurred.
    bool follow_agent(int id_follower, double time_of_follow) {
        Agent& e = network[id_follower];
        int agent_to_follow = -1;

        /* Dispatch to the appropriate follower logic: */
        FollowModel follow_model = config.follow_model;
        if (follow_model == RANDOM_FOLLOW) {
            // find a random agent within [0:number of agents - 1]
            agent_to_follow = random_follow_method(e, network.size());
        } else if (follow_model == TWITTER_PREFERENTIAL_FOLLOW && config.use_barabasi) {
            agent_to_follow = preferential_barabasi_follow_method();
        } else if(follow_model == TWITTER_PREFERENTIAL_FOLLOW && !config.use_barabasi) {
            agent_to_follow = twitter_preferential_follow_method(e, time_of_follow);
        } else if (follow_model == AGENT_FOLLOW) {
            agent_to_follow = agent_follow_method(e);
        } else if (follow_model == PREFERENTIAL_AGENT_FOLLOW) {
            agent_to_follow = preferential_agent_follow_method(e);
        } else if (follow_model == TWITTER_FOLLOW) {
            agent_to_follow = twitter_follow_model(e, time_of_follow, /*Updated after call: */ follow_model);
        } else if (follow_model == HASHTAG_FOLLOW) {
            agent_to_follow = hashtag_follow_method(e);
        } else {
            ASSERT(false, "Unknown follow model!");
        }

        // if the stage1_follow is set to true in the inputfile
        if (config.stage1_unfollow) {
            vector<int>& chatties = e.chatty_agents;
            if (chatties.size() > 0) {
                int id_agent_unfollowed = rng.pick_random_uniform(chatties);
                // Will remove from chattiness list after unfollow:
                handle_unfollow(id_agent_unfollowed, id_follower);
            }
        }

        // Return 'false' if we were unable to find an agent to follow:
        if (UNLIKELY(agent_to_follow == -1)) {
            return false;
        }

        bool same_agent = (id_follower == agent_to_follow);
        bool same_language = (e.language == network[agent_to_follow].language);
        // check and make sure we are not following ourself, or we are following agent -1
        if (LIKELY(!same_agent && same_language)) {
            perf_timer_begin("AnalyzerFollower.follow_agent(handle_follow)");
            // point to the agent who is being followed
            if (handle_follow(id_follower, agent_to_follow, follow_model)) {
                /* FEATURE: Follow-back based on target's prob_followback.
                 * Set in INFILE.yaml as followback_probability. */
                int et_id = network[agent_to_follow].agent_type;
                AgentType& et = agent_types[et_id];
                
                // TODO this followback process has to be another follow method that happens naturally at some other time, possibly another 'spike' in the rate
                // TODO AD -- I think we can just queue an event, at some time frame in the future, and activate it
                // when KMC crosses that time.
                if (config.use_followback && rng.random_chance(et.prob_followback)) {
                    analyzer_followback(state, id_follower, agent_to_follow);
                }
                // based on the number of followers the followed-agent has, check to make sure we're still categorized properly
                Agent& target = network[agent_to_follow];
                // We were able to add the follow:
                et.follow_ranks.categorize(agent_to_follow, target.follower_set.size());
                follow_ranks.categorize(agent_to_follow, target.follower_set.size());

                return true;
            }
            perf_timer_end("AnalyzerFollower.follow_agent(handle_follow)");
        }

        return false; // Return 'false' to signify that nothing happened.
    }

    bool followback(int prev_actor_id, int prev_target_id) {
        // now the previous target will follow the previous actor back
        Agent& prev_actor = network[prev_actor_id];
        Agent& prev_target = network[prev_target_id];
        if (handle_follow(prev_target_id, prev_actor_id, FOLLOW_BACK_FOLLOW)) {
            int et_id = network[prev_actor_id].agent_type;
            AgentType& et = agent_types[et_id];
            et.follow_ranks.categorize(prev_actor_id, prev_actor.follower_set.size());
            follow_ranks.categorize(prev_actor_id, prev_actor.follower_set.size());
            RECORD_STAT(state, prev_target.agent_type, n_followback);
            return true;
        }
        RECORD_STAT(state, prev_target.agent_type, n_follows);
        RECORD_STAT(state, prev_actor.agent_type, n_followers);
        return false; // Return false to signify the network was not mutated.
    }

    void remove_chatty_agent(Agent& unfollowed, Agent& lost_follower) {
        auto& c = lost_follower.chatty_agents;
        auto iter = std::find(c.begin(), c.end(), unfollowed.id);
        bool in_chatty_agents_list = (iter != c.end());
        if (in_chatty_agents_list) {
            c.erase(iter);
        }
    }

    bool handle_unfollow(int id_unfollowed, int id_lost_follower) {
        PERF_TIMER();

        DEBUG_CHECK(id_lost_follower != -1, "Should not be invalid agent after unfollow decision!");
        Agent& unfollowed = network[id_unfollowed], &lost_follower = network[id_lost_follower];

        // Remove the lost follower from the unfollowed's follows:
        bool had_follower = unfollowed.follower_set.remove(lost_follower);
        DEBUG_CHECK(had_follower, "unfollow: Did not exist in follower list");

        // Remove the lost follower from the unfollowed's followers:
        bool had_follow = lost_follower.following_set.remove(state, id_unfollowed);
        DEBUG_CHECK(had_follow, "unfollow: Did not exist in follow list");

        // Remove the unfollowed person from our target's chattiness list, if found there:
        remove_chatty_agent(unfollowed, lost_follower);

        RECORD_STAT(state, lost_follower.agent_type, n_unfollows);
        return true;
    }
};

double preferential_weight(AnalysisState& state) {
    AnalyzerFollow analyzer(state);
    return analyzer.preferential_weight();
}

bool analyzer_handle_follow(AnalysisState& state, int id_actor, int id_target, int follow_method) {
    PERF_TIMER();
    AnalyzerFollow analyzer(state);
    return analyzer.handle_follow(id_actor, id_target, follow_method);
}

bool analyzer_handle_unfollow(AnalysisState& state, int id_unfollowed, int id_lost_follower) {
    PERF_TIMER();
    AnalyzerFollow analyzer(state);
    return analyzer.handle_unfollow(id_unfollowed, id_lost_follower);
}

bool analyzer_follow_agent(AnalysisState& state, int agent, double time_of_follow) {
    PERF_TIMER();
    AnalyzerFollow analyzer(state);
    return analyzer.follow_agent(agent, time_of_follow);
}

bool analyzer_followback(AnalysisState& state, int follower, int followed) {
    PERF_TIMER();
    AnalyzerFollow analyzer(state);
    return analyzer.followback(follower, followed);
}
