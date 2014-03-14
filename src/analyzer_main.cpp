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
#include "entity.h"
#include "io.h"
#include "tweets.h"

#include "config_static.h"

#include "dependencies/lcommon/Timer.h"
#include "dependencies/lcommon/perf_timer.h"

#include "interrupt_menu.h"

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

/* The Analyze struct encapsulates the many-parameter analyze function, and its state. */
struct Analyzer {

    /***************************************************************************
     * Configuration and state variables of the analysis routines.
     ***************************************************************************/

    AnalysisState& state;
    ParsedConfig& config;
    AnalysisStats& stats;
    
    // struct for the entity classes, see network.h for specifications
    EntityTypeVector& entity_types;

    // The network state
    Network& network;

	// Categories for tweeting, following, retweeting
    CategoryGrouper& tweet_ranks;
	CategoryGrouper& follow_ranks;
	CategoryGrouper& retweet_ranks;
    Add_Rates& add_rates;
    TweetBank& tweet_bank;
    MostPopularTweet& most_pop_tweet;

    /* Mersenne-twister random number generator */
    MTwist& rng;

    // Times the interval between prints to the console
    Timer stdout_milestone_timer;

    ofstream DATA_TIME; // Output file to plot data
    ofstream add_data;
    ofstream following_data;
    ofstream followers_data;
    ofstream tweet_data;
    ofstream retweet_data;
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
            entity_types(state.entity_types), network(state.network),
            tweet_ranks(state.tweet_ranks), follow_ranks(state.follow_ranks), retweet_ranks(state.retweet_ranks),
            rng(state.rng), time(state.time), add_rates(state.add_rates), tweet_bank(state.tweet_bank),
            most_pop_tweet(state.most_pop_tweet) {

        // The following allocates a memory chunk proportional to max_entities:
        network.allocate(config.max_entities);

        DATA_TIME.open("DATA_vs_TIME");
        add_data.open("output/entity_populations.dat");
        following_data.open("output/entity_following.dat");
        followers_data.open("output/entity_followers.dat");
        tweet_data.open("output/entity_tweets.dat");
        retweet_data.open("output/entity_retweets.dat");
		
        set_initial_entities();
        set_future_add_rates(add_rates);
        call_future_rates();
		analyzer_rate_update(state);
    }

    void set_future_add_rates(Add_Rates& add_rates) {
        int projected_months = config.max_sim_time / APPROX_MONTH;
        if (add_rates.RF.function_type == "constant" ) {
            for (int i = 0; i <= projected_months; i ++) {
                add_rates.RF.monthly_rates.push_back(add_rates.RF.const_val);
            }
        } else if (add_rates.RF.function_type == "linear") {
            for (int i = 0; i <= projected_months; i ++) {
                add_rates.RF.monthly_rates.push_back(add_rates.RF.y_intercept + i*add_rates.RF.slope);
            }
        }
    }
    
	void call_future_rates() {
		for (int i = 0; i < entity_types.size(); i ++) {
			for (int j = 0; j < number_of_diff_events; j ++) {
				set_future_rates(entity_types[i], j);
			}
		}
	}
	void set_future_rates(EntityType& et, int event) {
        int projected_months = config.max_sim_time / APPROX_MONTH;
        if (et.RF[event].function_type == "not specified" && event == 0) {
            error_exit("FATAL: Follow rate function for entity \""+ et.name + "\" was not specified.");
        } else if (et.RF[event].function_type == "not specified" && event == 1) {
            error_exit("FATAL: Follow rate function for entity \""+ et.name + "\" was not specified.");
        } else if (et.RF[event].function_type == "constant") {
			ASSERT(et.RF[event].const_val >= 0, "Check your rates, one of them is < 0");
			for (int i = 0; i <= projected_months; i ++) {
                et.RF[event].monthly_rates.push_back(et.RF[event].const_val);
			}
		} else if (et.RF[event].function_type == "linear") {
			for (int i = 0; i <= projected_months; i ++) {
				if (et.RF[event].y_intercept + i * et.RF[event].slope >= 0) {
					et.RF[event].monthly_rates.push_back(et.RF[event].y_intercept + i * et.RF[event].slope);
				} else {
					et.RF[event].monthly_rates.push_back(0);
				}
			}
		} else if (et.RF[event].function_type == "exponential") {
			for (int i = 0; i <= projected_months; i ++) {
				if (et.RF[event].amplitude*exp(et.RF[event].exp_factor*i) >= 0) {
					et.RF[event].monthly_rates.push_back(et.RF[event].amplitude*exp(et.RF[event].exp_factor*i));
				} else {
					et.RF[event].monthly_rates.push_back(0);
				}
			}
		}
	}
    void set_initial_entities() {
        for (int i = 0; i < config.initial_entities; i++) {
             action_create_entity(0.0, i);
        }
    }

    /***************************************************************************
     * Analysis routines
     ***************************************************************************/

    // Entry point to all analysis:
    /* Conceptually this is the true entry point to our program,
     * after the messy configuration and allocation is done.
     * Returns end-time. */
    double main() {
        if (file_exists(config.save_file)) {
            load_network_state();
        }
        run_network_simulation();
        find_most_popular_tweet();
        return time;
    }

    bool sim_time_check() {
        return (time < config.max_sim_time);
    }
    bool real_time_check() {
        return (max_sim_timer.get_microseconds() < (config.max_real_time * 1000));
    }
    void interrupt_reset() {
        SIGNAL_ATTEMPTS = 0;
    }
    bool interrupt_check() {
        return (SIGNAL_ATTEMPTS == 0);
    }

    void load_network_state() {
        DataReader reader(state, config.save_file.c_str());
        string previous_config_file;
        reader << previous_config_file;
        if (previous_config_file != config.entire_config_file) {
            error_exit("Error, config file does not match the one being loaded from! Exiting...");
        }
        state.visit(reader);
    }

    void save_network_state() {
        const char* fname = config.save_file.c_str();
        printf("SAVING NETWORK STATE TO %s\n", fname);
        DataWriter writer(state, fname);
        writer << config.entire_config_file;
        state.visit(writer);
    }

    // ROOT ANALYSIS ROUTINE
    /* Run the main analysis routine using this config. */
    void run_network_simulation() {
        while ( LIKELY(sim_time_check() && real_time_check()) ) {
            if (!interrupt_check()) {
                interrupt_reset();
                if (!config.enable_interactive_mode) {
                    break;
                }
                // Has the user requested an exit?
                if (!show_interrupt_menu(state)) {
                    break;
                }
            }
            perf_timer_begin("analyzer.step_analysis");
        	step_analysis();
        	perf_timer_end("analyzer.step_analysis");
        }
        save_network_state();
    }

    Language entity_pick_language() {
        return config.lang_probs.kmc_select(rng);
    }

    /* Create a entity at the given index.
     * The index should be an empty entity slot. */
    bool action_create_entity(double creation_time, int index) {
		Entity& e = network[index];
		e.creation_time = creation_time;
		e.language = entity_pick_language();
		e.humour_bin = 0; // For now, no humour
//		e.humour_bin = rng.rand_int(N_BIN_HUMOUR); // Uniform
		e.preference_class = rng.rand_int(config.pref_classes.size());
		// Place the entity in a random location
		// TODO: Location choosing scheme
		e.location.x = rng.rand_real_with01();
		e.location.y = rng.rand_real_with01();
		double rand_num = rng.rand_real_not0();
		for (int et = 0; et < entity_types.size(); et++) {
		    EntityType& type = entity_types[et];
			if (rand_num <= type.prob_add) {
				e.entity_type = et;
                type.entity_list.push_back(index);
				follow_ranks.categorize(index, e.follower_set.size());
                type.follow_ranks.categorize(index, e.follower_set.size());
				break;
			}
			rand_num -= entity_types[et].prob_add;
		}
		if (config.use_barabasi){
			analyzer_follow_entity(state, index, index, creation_time);
		}
        network.n_entities++;
        return true; // Always succeeds, for now.
    }

    smartptr<TweetContent> generate_tweet_content(int id_original_author) {
        Entity& e_original_author = network[id_original_author];

        smartptr<TweetContent> ti(new TweetContent);
        ti->id_original_author = id_original_author;
        ti->time_of_tweet = time;
        // TODO: Pick
        ti->language = e_original_author.language; // TODO: AD -- Bilingual tweets make no sense
        return ti;
    }

    Tweet generate_tweet(int id_tweeter, int id_link, int generation, const smartptr<TweetContent>& content) {
        Entity& e_tweeter = network[id_tweeter];
        Tweet tweet;
        tweet.content = content;
        tweet.creation_time = time;
        tweet.id_tweeter = id_tweeter;
        tweet.id_link = id_link;
        tweet.generation = generation;
        if (network.n_followers(id_tweeter) != 0) {
            state.tweet_bank.add(tweet);
        }
        return tweet;
    }

	// function to handle the tweeting
	bool action_tweet(int id_tweeter) {
        // This is the entity tweeting
		Entity& e = network[id_tweeter];
        entity_types[e.entity_type].n_tweets++;
        tweet_ranks.categorize(id_tweeter, e.n_tweets);
        stats.n_tweets ++;
        if (network.n_followers(id_tweeter) > 0) {
            generate_tweet(id_tweeter, id_tweeter, 0, generate_tweet_content(id_tweeter));
            // Generate the tweet content:
            // increase the number of tweets the entity had by one
            e.n_tweets++;
            if (e.n_tweets / (time - e.creation_time) >= config.unfollow_tweet_rate) {
                action_unfollow(id_tweeter);
            }
            entity_types[e.entity_type].n_tweets++;
            tweet_ranks.categorize(id_tweeter, e.n_tweets);
            stats.n_tweets ++;
        }
        // Else, no followers -- no need to create a tweet
        //** AD: Still consider a success for now, re-evaluate later
        //** AD: Maybe we may want to discount the possibility of such tweets and just restart
		return true; // Always succeeds
	}

	bool action_retweet(RetweetChoice choice, double time_of_retweet) {
		Entity& e_observer = network[choice.id_observer];
		Entity& e_author = network[choice.id_author];

		Tweet tweet = generate_tweet(choice.id_observer, choice.id_link, choice.generation, *choice.content);
        entity_types[e_observer.entity_type].n_retweets ++;
        e_observer.n_retweets ++;
        stats.n_retweets ++;

        return true; // Always succeeds
	}

	// Causes 'id_unfollowed' to lose a follower
	// Returns true if a follower was removed, false if there was no follower to remove
	bool action_unfollow(int id_unfollowed) {
	    perf_timer_begin("action_unfollow");
		FollowerSet& candidate_followers = network.follower_set(id_unfollowed);

		int id_lost_follower = -1; // The entity to unfollow us
		if (!candidate_followers.pick_random_uniform(rng, id_lost_follower)) {
		    perf_timer_end("action_unfollow");
		    return false; // Empty
		}

		DEBUG_CHECK(id_lost_follower != -1, "Should not be -1 after choice!");

		// Necessary for use with follower set:
        FollowerSet::Context context(state, id_unfollowed);
        // Remove our target from our actor's follows:
        bool had_follower = candidate_followers.remove(context, id_lost_follower);
		DEBUG_CHECK(had_follower, "unfollow: Did not exist in follower list");

        // Remove our unfollowed person from our target's followers:
		Entity& e_lost_follower = network[id_lost_follower];
		bool had_follow = e_lost_follower.following_set.remove(state, id_unfollowed);
		DEBUG_CHECK(had_follow, "unfollow: Did not exist in follow list");
		perf_timer_end("action_unfollow");
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
    void step_analysis() {
        /*
         * Our step is wrapped in a loop to enabling restarting of events.
         * Often, there will be no way to resolve an event failure at the moment it occurs.
         * For example, when we land on a follow selection, and only have follows that we already have made.
         * At this point, the only logical course is to retry the whole KMC event.
         *
         * Luckily, this should be (relatively) rare.
         */
        bool complete = false;
        while (!complete) {
            double r = rng.rand_real_not0(); // get the first number with-in [0,1).
            int N = network.n_entities;

            // DECIDE WHAT TO DO:
            if (subtract_var(r, stats.prob_add) <= ZEROTOL) {
                // If we find ourselves in the add entity chuck of our cumulative function:
                perf_timer_begin("action_create_entity");
                complete = action_create_entity(time, N);
                perf_timer_end("action_create_entity");
            } else if (subtract_var(r, stats.prob_follow) <= ZEROTOL) {
                int entity = analyzer_select_entity(state, FOLLOW_SELECT);
                perf_timer_begin("action_follow_entity");
                complete = analyzer_follow_entity(state, entity, N, time);
                perf_timer_end("action_follow_entity");
            } else if (subtract_var(r, stats.prob_tweet) <= ZEROTOL) {
                // The tweet event
                int entity = analyzer_select_entity(state, TWEET_SELECT);
                perf_timer_begin("action_tweet");
                complete = action_tweet(entity);
                perf_timer_end("action_tweet");
            } else if (subtract_var(r, stats.prob_retweet) <= ZEROTOL ) {
                RetweetChoice choice = analyzer_select_tweet_to_retweet(state, RETWEET_SELECT);
                if (choice.id_author != -1) {
                    perf_timer_begin("action_retweet");
                    complete = action_retweet(choice, time);
                    perf_timer_end("action_retweet");
                }
            } else {
                error_exit("step_analysis: event out of bounds");
            }
        }

        step_time();
        stats.n_steps++;
        //update the rates if n_entities has changed
        analyzer_rate_update(state);
    }

    /* Step our KMC simulation proportionally to the global event rate. */
    void step_time() {
        static double STATIC_TIME = 0.0;

        double prev_time = time;
        double prev_integer = floor(time);
        double increment = -log(rng.rand_real_not0()) / stats.event_rate;
        if (config.use_random_time_increment) {
            // increment by random time
            time += increment;
        } else {
            time += 1.0 / stats.event_rate;
        }

        ASSERT(STATIC_TIME < time, "Fail");
        STATIC_TIME = time;
        if (config.output_stdout_summary && (floor(time) > prev_integer || config.output_verbose)) {
          output_summary_stats();
        } 
        /*if (stats.n_outputs % (STDOUT_OUTPUT_RATE*50 + 1)  == 0 && stats.n_outputs != 0) {
            cout << "\n\nPerforming mid-simulation calculations...\n\n";
            output_network_statistics(state);
            cout << "Done.\n";
        }*/
    }

    /***************************************************************************
     * Helper functions
     ***************************************************************************/

    void find_most_popular_tweet() {
        int local_max = -1;
        Tweet local_tweet;
        vector<Tweet> active_tweet_list = tweet_bank.as_vector();
        for (auto& tweet : active_tweet_list) {
            UsedEntities& ue = tweet.content->used_entities;
            if (ue.size() > local_max) {
                local_max = ue.size();
                local_tweet = tweet;
            }
        }
        if (local_max > most_pop_tweet.global_max) {
            most_pop_tweet.global_max = local_max;
            most_pop_tweet.most_popular_tweet = local_tweet;
        }

//        if (!active_tweet_list.empty()) {
//            ASSERT(!most_pop_tweet.most_popular_tweet.content.empty(), "Should have a most popular tweet!");
//        }
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

    void output_summary_stats(ostream& stream, double time_spent = /*Don't print*/-1) {
        stream << fixed << setprecision(2)
                << time << "\t\t"
                << network.n_entities << "\t\t"
                << stats.n_follows << "\t\t"
                << stats.n_tweets << "\t\t"
                << stats.n_retweets << "(" << state.tweet_bank.n_active_tweets() << ")\t"
                << stats.event_rate << "\t\t";
        if (time_spent != -1) {
            stream << time_spent << "ms\t";
        }
        stream << "\n";
    }
    void output_summary_stats() {

		const char* HEADER = "\n#Time\t\tUsers\t\tFollows\t\tTweets\t\tRetweets\tR\tReal Time Spent\n\n";
        if (stats.n_outputs % (25 * STDOUT_OUTPUT_RATE) == 0) {
        	cout << HEADER;
        }
        if (stats.n_outputs % 500 == 0) {
            DATA_TIME << HEADER;
            following_data << "\n#Time\t";
            followers_data << "\n#Time\t";
            tweet_data << "\n#Time\t";
            retweet_data << "\n#Time\t";
            add_data << "\n#Time\t";
            for (auto& type : entity_types) {
                following_data << type.name << "\t";
                followers_data << type.name << "\t";
                tweet_data << type.name << "\t";
                retweet_data << type.name << "\t";
                add_data << type.name << "\t";
            }
            following_data << "\n\n";
            followers_data << "\n\n";
            tweet_data << "\n\n";
            retweet_data << "\n\n";
            add_data << "\n\n";
            }
            following_data << time << "\t";
            followers_data << time << "\t";
            tweet_data << time << "\t";
            retweet_data << time << "\t";
            add_data << time << "\t";
            for (auto& type : entity_types) {
                following_data << type.n_follows << "\t";
                followers_data << type.n_followers << "\t";
                tweet_data << type.n_tweets << "\t";
                retweet_data << type.n_retweets << "\t";
                add_data << type.entity_list.size() << "\t";
            }
        following_data << "\n";
        followers_data << "\n";
        tweet_data << "\n";
        retweet_data << "\n";
        add_data << "\n";
        output_summary_stats(DATA_TIME);
        if (stats.n_outputs % STDOUT_OUTPUT_RATE == 0) {
        	output_summary_stats(cout, stdout_milestone_timer.get_microseconds() / 1000.0);
            find_most_popular_tweet();
            stdout_milestone_timer.start(); // Restart the timer
        }

        stats.n_outputs++;
    }
};

// Run a network simulation using the given input file's parameters
void analyzer_main(AnalysisState& state) {
    Timer timer;
    Analyzer analyzer(state);
    if (state.config.handle_ctrlc) {
        signal_handlers_install(state);
    }
    analyzer.main();
    signal_handlers_uninstall(state);
    printf("'analyzer_main' took %.2f milliseconds.\n", timer.get_microseconds() / 1000.0);
}
