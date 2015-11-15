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

#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <deque>

// Local includes:
#include "analyzer.h"
#include "util.h"
#include "network.h"
#include "agent.h"
#include "io.h"
#include "tweets.h"

#include "config_static.h"

#include "dependencies/lcommon/Timer.h"
#include "dependencies/lcommon/perf_timer.h"

#include "interactive_mode.h"

#include <signal.h>

using namespace std;

volatile int SIGNAL_ATTEMPTS = 0;

static const int SIGNAL_ATTEMPTS_TO_ABORT = 3;
// Handler for singals -- sent by eg Ctrl-C on command-line. Allows us to stop our program gracefully!
static void signal_handler(int __dummy) {
    SIGNAL_ATTEMPTS++;
    if (SIGNAL_ATTEMPTS >= SIGNAL_ATTEMPTS_TO_ABORT) {
        error_exit("User demands abort!");
        signal(SIGINT, SIG_DFL);
    }
}

static void signal_handlers_uninstall(AnalysisState& state) {
   signal(SIGINT, SIG_DFL);
   signal(SIGUSR1 , SIG_DFL); // For custom interaction
}

static void signal_handlers_install(AnalysisState& state) {
   if (state.config.handle_ctrlc) {
       signal(SIGINT, signal_handler);
   }
   signal(SIGUSR1, signal_handler); // For custom interaction
}

/* The Analyzer struct encapsulates the many-parameter analyze function, and its state. */
struct Analyzer {

    /***************************************************************************
     * Configuration and state variables of the analysis routines.
     ***************************************************************************/

    AnalysisState& state;
    ParsedConfig& config;
    NetworkStats& stats;
    
    // struct for the agent classes, see network.h for specifications
    AgentTypeVector& agent_types;

    // The network state
    Network& network;

	// Categories for tweeting, following, retweeting
    CategoryGrouper& tweet_ranks;
    CategoryGrouper& follow_ranks;
    CategoryGrouper& retweet_ranks;
    Add_Rates& add_rates;
    TweetBank& tweet_bank;
    MostPopularTweet& most_pop_tweet;
    HashTags& hashtags;

    /* Mersenne-twister random number generator */
    MTwist& rng;

    // Times the interval between prints to the console
    Timer stdout_milestone_timer;
    RealOrSimulatedTimePeriodChecker output_time_checker;
    static RealOrSimulatedTimePeriodChecker output_time_checker_from_config(ParsedConfig& config) {
        if (config.summary_output_rate_real_minutes) {
            return {RealTimePeriodChecker(config.summary_output_rate)};
        } else {
            // Simulated time period checker
            return {TimePeriodChecker(config.summary_output_rate)};
        }
    }

    ofstream DATA_TIME; // Output file to plot data

    double& time;

    Timer max_sim_timer;

    /***************************************************************************
     * Initialization functions
     ***************************************************************************/

    /* Initialization and loading of configuration.
     * Reads configuration from the given input file. */
    Analyzer(AnalysisState& state) :
        //** Here, we initialize REFERENCES to the various analysis pieces.
        //** The only way to initialize references is through this construct -- called an 'initializer list'.
        //** This ensures that updates are witnessed in the caller's AnalysisState object.
            state(state), stats(state.stats), config(state.config),
            agent_types(state.agent_types), network(state.network),
            tweet_ranks(state.tweet_ranks), follow_ranks(state.follow_ranks), retweet_ranks(state.retweet_ranks),
            rng(state.rng), time(state.time), add_rates(state.config.add_rates), tweet_bank(state.tweet_bank),
            most_pop_tweet(state.most_pop_tweet), hashtags(state.hashtags), output_time_checker(output_time_checker_from_config(state.config)) {

        // The following allocates a memory chunk proportional to max_agents:
        network.allocate(config.max_agents);

        DATA_TIME.open("DATA_vs_TIME");
        
        set_initial_agents();
        analyzer_rate_update(state);
        init_referral_rate_function(config);
    }
    void init_referral_rate_function(ParsedConfig& config) {
        // for now, this function decreases over time by 1 / t
        int projected_months = config.max_sim_time / APPROX_MONTH;
        for (int i = 0; i <= projected_months; i ++) {
            config.referral_rate_function.monthly_rates.push_back(1.0 / (double) (1+i));
        }
    }
    void set_initial_agents() {
        for (int i = 0; i < config.initial_agents; i++) {
             action_create_agent();
        }
    }

    /***************************************************************************
     * Analysis routines
     ***************************************************************************/

    // Entry point to all analysis:
    /* Conceptually this is the true entry point to our program,
     * after the messy configuration and allocation is done.
     * Returns end-time. */
    double main(Timer& timer) {
        if (config.load_network_on_startup && file_exists(config.save_file)) {
            load_network_state(config.save_file.c_str());
        } else {
            lua_hook_new_network(state);
        }
        run_network_simulation(timer);
        if (config.most_popular_tweet_content) {
            find_most_popular_tweet();
        }
        return time;
    }

    bool sim_time_check() {
        return (time < config.max_sim_time);
    }

    bool real_time_check() {
        const int MINUTE_TO_MICROSECOND = 60*1000000LL;
        return (max_sim_timer.get_microseconds() < (config.max_real_time * MINUTE_TO_MICROSECOND));
    }
    void interrupt_reset() {
        SIGNAL_ATTEMPTS = 0;
    }
    bool interrupt_check() {
        return (SIGNAL_ATTEMPTS == 0);
    }

    void fix_agents_upon_resubmission(AnalysisState& state) {
        Network& n = state.network;
        auto& R = state.config.regions;
        cout << "Fixing agents who have changed attributes...\n";
        for (int i = 0; i < n.size(); i ++) {
            Agent& a = n[i];
            auto& region = R.regions[a.region_bin];
            a.language = (Language) rng.kmc_select(region.language_probs);
            auto temp = network.follower_set(a.id).as_vector();
            for (int id : temp) {
                analyzer_handle_unfollow(state, a.id, id);
                analyzer_handle_follow(state, id, a.id, 0);
            }
        }
    }

    void load_network_state(const char* fname) {
        printf("LOADING NETWORK STATE FROM %s\n", fname);
        DataReader reader(state, fname);
        string previous_config_file;
        reader << previous_config_file;
        if (!config.ignore_load_config_check && previous_config_file != config.entire_config_file) {
            error_exit("Error, config file does not exactly match the one being loaded from!\n"
                    "If you do not care, please set output.ignore_load_config_check to true.\nExiting...");
        }
        state.visit(reader);

        /* Synchronize rates from the loaded configuration.
         * This is done because, although we can load a new configuration,
         * some rates remain duplicated in our state object. */
        state.sync_rates();

        lua_hook_load_network(state);
        fix_agents_upon_resubmission(state);
    }

    void save_network_state(const char* fname) {
        lua_hook_save_network(state);
        printf("\n\nSAVING NETWORK STATE TO %s\n", fname);
        DataWriter writer(state, fname);
        writer << config.entire_config_file;
        state.visit(writer);
    }

    // ROOT ANALYSIS ROUTINE
    /* Run the main analysis routine using this config. */
    void run_network_simulation(Timer& timer) {
        if (config.output_stdout_summary)
            cout << setw(25)
            << "Simulation Time (min)" << setw(25)
            << "Agents" << setw(25)
            << "Follows" << setw(25)
            << "Tweets" << setw(25)
            << "Active Tweets" << setw(25)
            << "Retweets" << setw(25)
            << "Unfollows" << setw(25)
            << "Cumulative-Rate" << setw(25)
            << "Real Time (s)" << "\n\n";
        while (sim_time_check() && real_time_check()) {
            if (!interrupt_check()) {
                interrupt_reset();
                if (!config.enable_interactive_mode) {
                    stats.user_did_exit = true;
                    break;
                }
                // Has the user requested an exit?
                if (!start_interactive_mode(state)) {
                    stats.user_did_exit = true;
                    break;
                }
            }
        	if (!step_analysis(timer)) {
        	    break;
        	}
        }
        if (config.save_network_on_timeout) {
            save_network_state(config.save_file.c_str());
        }
    }

    /* Create a new agent at the next index. */
    bool action_create_agent() {
        PERF_TIMER();

        if (network.size() == network.max_size()) {
            // Make sure we do not try to add!
            return false;
        }

        int id = network.size();
        network.grow();

        double creation_time = time;
        Agent& e = network[id];

        e.id = id;

        // Determine abstract location:
        auto& R = state.config.regions;
        DEBUG_CHECK(R.regions.size() > N_BIN_REGIONS, "Too many regions!");
        int region_bin = rng.kmc_select(R.add_probs);
        auto& region = R.regions[region_bin];

        e.region_bin = region_bin;
        e.ideology_bin = rng.kmc_select(region.ideology_probs);
        e.creation_time = creation_time;
        e.language = (Language) rng.kmc_select(region.language_probs);
        // For now, either always mark ideology, or never
        e.ideology_tweet_percent = rng.random_chance(0.5) ? 1.0 : 0.0;
        e.preference_class = rng.kmc_select(region.preference_class_probs);

        double rand_num = rng.rand_real_not0();
        for (int et = 0; et < agent_types.size(); et++) {
            AgentType& type = agent_types[et];
            if (rand_num <= type.prob_add) {
                e.agent_type = et;
                type.agent_list.push_back(id);
                follow_ranks.categorize(id, e.follower_set.size());
                type.follow_ranks.categorize(id, e.follower_set.size());
                break;
            }
            rand_num -= agent_types[et].prob_add;
        }

        lua_hook_add(state, id);

        if (config.use_barabasi){
            // follow so many times depending on setting
            for (int i = 0; i < config.barabasi_connections; i ++) {
                analyzer_follow_agent(state, id, creation_time);
            }
        }
        return true;
    }

    smartptr<TweetContent> generate_tweet_content(int id_original_author) {
        Agent& e_original_author = network[id_original_author];
        int agent = e_original_author.agent_type;
        AgentType& agent_type = agent_types[agent];

        smartptr<TweetContent> ti(new TweetContent);
        ti->id_original_author = id_original_author;
        ti->time_of_tweet = time;
//        ti->type = agent_type;
        ti->ideology_bin = e_original_author.ideology_bin;
        ti->type = (TweetType)rng.kmc_select(agent_type.tweet_type_probs, N_TWEET_TYPES);
        // TODO: Pick
        Language lang = e_original_author.language;

        // For bilingual tweeters, tweet both languages with equal probability:
        if (lang == LANG_FRENCH_AND_ENGLISH) {
            lang = rng.random_chance(0.5) ? LANG_ENGLISH : LANG_FRENCH;
        }
        ti->language = lang;
        return ti;
    }
    
    bool include_hashtag() {
        return rng.random_chance(config.hashtag_prob);
    }

    Tweet generate_tweet(int id_tweeter, int id_link, int generation, const smartptr<TweetContent>& content) {
        PERF_TIMER();
        Agent& e_tweeter = network[id_tweeter];
        Agent& e_author = network[content->id_original_author];

        Tweet tweet;
        tweet.id_tweet = stats.global_stats.n_tweets;
        tweet.content = content;
        tweet.creation_time = time;
        tweet.id_tweeter = id_tweeter;
        tweet.id_link = id_link;
        tweet.generation = generation;
        // if this is a hashtag and not a retweet, we have to add the agent id into 
        // the circular buffer. 
        if (include_hashtag() && generation == 0) {
            RECORD_STAT(state, e_tweeter.agent_type, n_hashtags);
            tweet.hashtag = true;
            hashtags.hashtag_groups[e_author.ideology_bin][e_author.region_bin].circ_buffer.add(e_author.id);
        }

        // Always start in the first retweet time bin:
        tweet.retweet_time_bin = 0;
        tweet.retweet_next_rebin_time = time + config.tweet_obs.initial_resolution;

        /* Determines reaction weights for the tweet: */
        double total_weight = e_tweeter.follower_set.determine_tweet_weights(e_author, *content, config.tweet_react_rates, tweet.react_weights);

        /* Only consider tweets that can actually be retweeted. */
        if (total_weight != 0) {
            state.tweet_bank.add(tweet);
        }

        return tweet;
    }

	// function to handle the tweeting
	bool action_tweet(int id_tweeter) {
	    PERF_TIMER();

            // This is the agent tweeting
                    Agent& e = network[id_tweeter];
            tweet_ranks.categorize(id_tweeter, e.n_tweets);
            e.n_tweets++;
            generate_tweet(id_tweeter, id_tweeter, 0, generate_tweet_content(id_tweeter));
            lua_hook_tweet(state, id_tweeter, e.n_tweets);
            // Generate the tweet content:
            // increase the number of tweets the agent had by one
            if (e.n_tweets / (time - e.creation_time) >= config.unfollow_tweet_rate) {
                action_unfollow(id_tweeter);
            }

            RECORD_STAT(state, e.agent_type, n_tweets);

            return true; // Always succeeds
	}

	// Despite being called action_retweet, may result in follow
	// depending on probability encoded in PreferenceClass, if not first-generation tweet.
	bool action_retweet(RetweetChoice choice, double time_of_retweet) {
	    PERF_TIMER();
		Agent& e_observer = network[choice.id_observer];
		Agent& e_author = network[choice.id_author];

		PreferenceClass& obs_pref_class = config.pref_classes[e_observer.preference_class];

        if (config.use_follow_via_retweets) {
    		if (choice.generation > 0) {
    		    // Not first-generation tweet? Attempt follow.
    		    // Note this is not attempted for first-generation tweets because the observer
    		    // would necessarily be in the authors follower set already.
                double val = preferential_weight(state);
                if (rng.random_chance(val)) {
                    // Return success of follow:
                    RECORD_STAT(state, e_observer.agent_type, n_retweet_follows);
                    return analyzer_handle_follow(state, choice.id_observer, choice.id_author, N_FOLLOW_MODELS + 2);
                }
    		}
        }

		generate_tweet(choice.id_observer, choice.id_link, choice.generation, *choice.content);

        RECORD_STAT(state, e_observer.agent_type, n_retweets);
        e_observer.n_retweets ++;

        return true;
	}

	// Causes 'id_unfollowed' to lose a follower
	// Returns true if a follower was removed, false if there was no follower to remove
	bool action_unfollow(int id_unfollowed) {
	    PERF_TIMER();

		FollowerSet& candidate_followers = network.follower_set(id_unfollowed);

		int id_lost_follower = -1; // The agent to unfollow us
		if (!candidate_followers.pick_random_uniform(rng, id_lost_follower)) {
		    perf_timer_end("action_unfollow");
		    return false; // Empty
		}

		DEBUG_CHECK(id_lost_follower != -1, "Should not be -1 after choice!");

        // Remove our target from our actor's follows:
        bool had_follower = candidate_followers.remove(network[id_lost_follower]);
		DEBUG_CHECK(had_follower, "unfollow: Did not exist in follower list");

        // Remove our unfollowed person from our target's followers:
		Agent& e_lost_follower = network[id_lost_follower];
		bool had_follow = e_lost_follower.following_set.remove(state, id_unfollowed);
		DEBUG_CHECK(had_follow, "unfollow: Did not exist in follow list");

		lua_hook_unfollow(state, id_lost_follower, id_unfollowed);
		RECORD_STAT(state, e_lost_follower.agent_type, n_unfollows);

		return true;
	}

	// Helper for step_analysis.
	// Subtracts from a 'double' variable, 'var' by 'increment', and returns the new value.
	static double subtract_var(double& var, double increment) {
	    var -= increment;
	    return var;
	}

    // Performs one step of the analysis routine.
    // Takes old time, returns new time
    bool step_analysis(Timer& timer) {
        PERF_TIMER();

        // Exit case 1:
        // We opt to inform the user of a stagnant network rather than trying continuously:
        if (stats.event_rate == 0) {
            cout << "ANAMOLY: Stagnant network! Nothing to do. Exiting." << endl;
            return false;
        }

        // Exit case 2:
        // Check if we have reached desired analysis steps:
        if (stats.n_steps >= config.max_analysis_steps) {
            return false;
        }

        /*
         * Fix for Github issue #3:
         *
         * Our step used to be wrapped in a loop to enabling restarting of events. This was incorrect.
         * Often, there will be no way to resolve an event failure at the moment it occurs.
         *
         * The comment use to incorrectly state "At this point, the only logical course is to retry the whole KMC event."
         *
         * Only, this isn't the case. The correct thing to do is to continue, because KMC is tolerant to 
         * arbitrarily large 'do-nothing' rate blocks. 
         *
         * Retrying as we did before (ie, not moving time forward) caused some underestimation in the time of events.
         */

        bool network_has_changed = false;

        lua_hook_step_analysis(state);

        // Get a random number within [0,1).
        double r = rng.rand_real_not0(); // 
        // Decide what action corresponds to our random number.
        if (subtract_var(r, stats.prob_add) <= ZEROTOL) {
            // The agent creation event
            network_has_changed = action_create_agent();
        } else if (subtract_var(r, stats.prob_follow) <= ZEROTOL) {
            // The follow event
            int agent = analyzer_select_agent(state, FOLLOW_SELECT);
            if (agent != -1) {
                network_has_changed = analyzer_follow_agent(state, agent, time);
            }
        } else if (subtract_var(r, stats.prob_tweet) <= ZEROTOL) {
            // The tweet event
            int agent = analyzer_select_agent(state, TWEET_SELECT);
            if (agent != -1) {
                network_has_changed = action_tweet(agent);
            }
        } else if (subtract_var(r, stats.prob_retweet) <= ZEROTOL ) {
            // The retweet event
            RetweetChoice choice = analyzer_select_tweet_to_retweet(state, RETWEET_SELECT);
            if (choice.id_author != -1) {
                network_has_changed = action_retweet(choice, time);
            }
        } else {
            error_exit("step_analysis: event out of bounds");
        }

        step_time(timer);
        stats.n_steps++;

        //update the rates if n_agents has changed
        analyzer_rate_update(state);

        return true;
    }

    /* Step our KMC simulation proportionally to the global event rate. */
    void step_time(Timer& timer) {
        if (config.use_random_time_increment) {
            // increment by random time
            double increment = -log(rng.rand_real_not0()) / stats.event_rate;
            time += increment;
        } else {
            time += 1.0 / stats.event_rate;
        }

        if (config.output_stdout_summary && output_time_checker.has_past(time)) {
            output_summary_stats(timer);
        } 
    }

    /***************************************************************************
     * Helper functions`
     ***************************************************************************/
    
    bool is_following(int& follower, int& followee) {
        FollowingSet& fs = network.following_set(follower);
        return fs.contains(followee);
    }
    
    bool is_path(Tweet& tweet, vector<Tweet>& atl) {
        int gen = tweet.generation;
        vector<Tweet> tweet_list;
        for (auto& t : atl) {
            if (t.content == tweet.content && t.id_tweeter != tweet.id_tweeter && t.generation == gen - 1) {
                tweet_list.push_back(t);
            }
        }
        // check and see if we are connected to the previous generation
        for (auto& t : tweet_list) {
            if (is_following(tweet.id_tweeter, t.id_tweeter)) {
                return true;
            }
        }
        return false;
    }
    
    bool retweet_checks() {
        // atl --- active tweet list
        vector<Tweet> atl = tweet_bank.as_vector();
        for (auto& tweet : atl) {
            // if it is a retweet
            if (tweet.creation_time != tweet.content->time_of_tweet) {
                if (is_path(tweet, atl)) {
                    return true;
                } 
            } else if (tweet.creation_time == tweet.content->time_of_tweet) {
                return true;
            }
        }
        return false;
    }

    void find_most_popular_tweet() {
        int local_max = 0;
        Tweet local_tweet;
        vector<Tweet> active_tweet_list = tweet_bank.as_vector();
        for (auto& tweet : active_tweet_list) {
            UsedAgents& ue = tweet.content->used_agents;
            if (ue.size() > local_max) {
                if (tweet.creation_time == tweet.content->time_of_tweet)
                local_max = ue.size();
                local_tweet = tweet;
            }
        }
        if (local_max > most_pop_tweet.global_max) {
            most_pop_tweet.global_max = local_max;
            most_pop_tweet.most_popular_tweet = local_tweet;
        }
    }
    
    void output_tweets() {
        vector<Tweet> atl = tweet_bank.as_vector();
        ofstream output;
        output.open("output/tweets.dat");
        output << "\nID\t\torigID\t\ttime\t\torigtime\n\n";
        for (auto& t : atl) {
            output << t.id_tweeter << "\t\t" << t.content->id_original_author << "\t\t" << t.creation_time << "\t\t" << t.content->time_of_tweet << "\n";
        }
    }

    void output_summary_stats(ostream& stream, bool newline, Timer& timer) {
        if (newline) {
            stream << scientific << setprecision(2) << setw(25)
            << time << setw(25)
            << network.size() << setw(25)
            << stats.global_stats.n_follows << setw(25)
            << stats.global_stats.n_tweets << setw(25)
            << state.tweet_bank.n_active_tweets() << setw(25)
            << stats.global_stats.n_retweets << setw(25)
            << stats.global_stats.n_unfollows << setw(25)
            << stats.event_rate << setw(25)
            << timer.get_microseconds()*1e-6 << "\n";
            flush(stream);
        } else {
            stream << setprecision(2) << scientific << setw(25)
            << time << setw(25)
            << (double) network.size() << setw(25)
            << (double) stats.global_stats.n_follows << setw(25)
            << (double) stats.global_stats.n_tweets << setw(25)
            << (double) state.tweet_bank.n_active_tweets() << setw(25)
            << (double) stats.global_stats.n_retweets << setw(25)
            << (double) stats.global_stats.n_unfollows << setw(25)
            << stats.event_rate << setw(25)
            << timer.get_microseconds()*1e-6 << "\r";
            flush(stream);
        }
    }
    void output_summary_stats(Timer& timer) {

        if (stats.n_outputs == 0) {
            DATA_TIME << "#" << setw(25)
            << "Simulation Time (min)" << setw(25)
            << "Agents" << setw(25)
            << "Follows" << setw(25)
            << "Tweets" << setw(25)
            << "Active Tweets" << setw(25)
            << "Retweets" << setw(25)
            << "Unfollows" << setw(25)
            << "Cumulative-Rate" << setw(25)
            << "Real Time (s)" << "\n";
        }

        if (stats.n_outputs % STDOUT_OUTPUT_RATE == 0) {
            output_summary_stats(DATA_TIME, true, timer);
            output_summary_stats(cout, false, timer);
        }

        stats.n_outputs++;
    }
};

bool analyzer_create_agent(AnalysisState& state) {
    ASSERT(!state.analyzer.empty(), "Analysis is not active!");
    return state.analyzer->action_create_agent();
}

bool analyzer_sim_time_check(AnalysisState& state) {
    ASSERT(!state.analyzer.empty(), "Analysis is not active!");
    return state.analyzer->sim_time_check();
}

void analyzer_save_network_state(AnalysisState& state, const char* fname) {
    ASSERT(!state.analyzer.empty(), "Analysis is not active!");
    state.analyzer->save_network_state(fname);
}

void analyzer_load_network_state(AnalysisState& state, const char* fname) {
    ASSERT(!state.analyzer.empty(), "Analysis is not active!");
    state.analyzer->load_network_state(fname);
}

bool analyzer_real_time_check(AnalysisState& state) {
    ASSERT(!state.analyzer.empty(), "Analysis is not active!");
    return state.analyzer->real_time_check();
}

// Run a network simulation using the given input file's parameters
void analyzer_main(AnalysisState& state) {
    // We maintain a back-pointer to Analyzer so that we can access its methods
    // from other files, during analysis.

    Timer timer;
    state.analyzer.set(new Analyzer(state)); // Install back-pointer

    if (state.config.handle_ctrlc) {
        signal_handlers_install(state);
    }

    // >> The main analysis function:
    state.analyzer->main(timer);

    signal_handlers_uninstall(state);
    lua_hook_exit(state);

    // Print summary time:
    printf("'analyzer_main' took %.2f milliseconds.\n", timer.get_microseconds() / 1000.0);
}
