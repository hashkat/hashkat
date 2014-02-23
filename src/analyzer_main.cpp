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

#include "config_static.h"

#include "dependencies/lcommon/Timer.h"

#include <signal.h>

using namespace std;

volatile int CTRL_C_ATTEMPTS = 0;

static const int CTRL_C_ATTEMPTS_TO_ABORT = 3;
// Handler for SIGINT -- sent by Ctrl-C on command-line. Allows us to stop our program gracefully!
static void ctrl_C_handler(int __dummy) {
    CTRL_C_ATTEMPTS++;
    if (CTRL_C_ATTEMPTS >= CTRL_C_ATTEMPTS_TO_ABORT) {
        error_exit("User demands abort!");
        signal(SIGINT, SIG_DFL);
    }
}
static void ctrl_C_handler_install() {
   signal(SIGINT, ctrl_C_handler);
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
            rng(state.rng), time(state.time), add_rates(state.add_rates) {

        // The following allocates a memory chunk proportional to max_entities:
        network.allocate(config.max_entities);

        DATA_TIME.open("DATA_vs_TIME");
        add_data.open("entity_populations.dat");
        following_data.open("entity_following.dat");
        followers_data.open("entity_followers.dat");
        tweet_data.open("entity_tweets.dat");
        retweet_data.open("entity_retweets.dat");
		
        set_initial_entities();
        set_future_add_rates(add_rates);
        call_future_rates();
		analyzer_rate_update(state);
    }

    void set_future_add_rates(Add_Rates& add_rates) {
        int projected_months = config.max_time / APPROX_MONTH;
        cout << add_rates.RF.function_type << "\n";
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
			for (int j = 1; j < number_of_diff_events; j ++) {
				set_future_rates(entity_types[i], j);
			}
		}
	}
	void set_future_rates(EntityType& et, int event) {
        int projected_months = config.max_time / APPROX_MONTH;
        if (et.RF[event].function_type == "not specified" && event == 1) {
            error_exit("FATAL: Follow rate function for entity \""+ et.name + "\" was not specified.");
        } else if (et.RF[event].function_type == "not specified" && event == 2) {
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
        run_network_simulation();
        return time;
    }

    // ROOT ANALYSIS ROUTINE
    /* Run the main analysis routine using this config. */
    void run_network_simulation() {
        while ( LIKELY(time < config.max_time && network.n_entities < config.max_entities && CTRL_C_ATTEMPTS == 0) ) {
        	step_analysis();
        }
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
		e.preference_class = rng.rand_int(config.pref_classes.size());
		// Place the entity in a random location
		// TODO: Location choosing scheme
		e.location.x = rng.rand_real_with01();
		e.location.y = rng.rand_real_with01();
		double rand_num = rng.rand_real_not0();
		for (int et = 0; et < entity_types.size(); et++) {
			if (rand_num <= entity_types[et].prob_add) {
				e.entity_type = et;
                entity_types[et].entity_list.push_back(index);
				follow_ranks.categorize(index, e.follower_set.size());
                entity_types[et].follow_ranks.categorize(index, e.follower_set.size());
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

    // will use the categories here to decide who will retweet
    int users_to_retweet(Entity& e) {
        // returns full list for now
        FollowerSet& fs = e.follower_set;

        // AD -- TEMPORARY NOTE: Refactoring occurred here
        if (e.language == LANG_ENGLISH) {
            // return all of the english speakers and bilingual speakers 
            return language_entity_amount(fs, LANG_ENGLISH) + language_entity_amount(fs, LANG_FRENCH_AND_ENGLISH);
        } else if (e.language == LANG_FRENCH_AND_ENGLISH) {
            // return the whole list
            return e.follower_set.size(); // TODO: AD -- Bilingual tweets make no sense
        } else if (e.language == LANG_FRENCH){
            return language_entity_amount(fs, LANG_FRENCH) + language_entity_amount(fs, LANG_FRENCH_AND_ENGLISH);
        }
        return 0;
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

    Tweet generate_tweet(int id_tweeter, const smartptr<TweetContent>& content) {
        Entity& e_tweeter = network[id_tweeter];
        Tweet tweet;
        tweet.content = content;
        tweet.creation_time = time;
        tweet.id_tweeter = id_tweeter;
        if (network.n_followers(id_tweeter) != 0) {
            RateVec<1> rate_vec(0.01 * network.n_followers(id_tweeter));
            state.tweet_bank.tree.add(tweet, rate_vec);
        }
        return tweet;
    }

	// function to handle the tweeting
	bool action_tweet(int id_tweeter) {
        if (network.n_followers(id_tweeter) == 0) {
            // No followers -- no need to even store the tweet.
            return true; //** AD: Still consider a success for now, re-evaluate later
            //** AD: Maybe we may want to discount the possibility of such tweets and just restart
        }
		// This is the entity tweeting
		Entity& e = network[id_tweeter];
        // add info to TweetInfo struct
        e.last_tweet = generate_tweet(id_tweeter, generate_tweet_content(id_tweeter));
		// increase the number of tweets the entity had by one
		e.n_tweets++;
        double ridic_tweet_rate = 0.00694; // 10 tweets/day
            if (e.n_tweets / (time - e.creation_time) >= ridic_tweet_rate) {
                action_unfollow(id_tweeter);
            }
        entity_types[e.entity_type].n_tweets++;
		tweet_ranks.categorize(id_tweeter, e.n_tweets);
        stats.n_tweets ++;
		return true; // Always succeeds
	}

	bool action_retweet(RetweetChoice choice, double time_of_retweet) {
		Entity& e_observer = network[choice.id_observer];
		Entity& e_author = network[choice.id_author];

		DEBUG_CHECK(!e_author.last_tweet.content.empty(), "Retweeting empty tweet!");
		e_observer.last_tweet = generate_tweet(choice.id_observer, e_author.last_tweet.content);
        entity_types[e_observer.entity_type].n_retweets ++;
        stats.n_retweets ++;

        return true; // Always succeeds
	}

	bool action_unfollow(int id_actor) {
		Entity& e_actor = network[id_actor]; // The entity which unfollows
		FollowSet& actor_follows = e_actor.follow_set;

		int id_target = -1; // The entity to unfollow
		if (!actor_follows.pick_random_uniform(rng, id_target)) {
		    return false; // Empty
		}

        // Remove our target from our actor's follows:
        bool actor_had_target = actor_follows.remove(state, 1.0, id_target);
		DEBUG_CHECK(actor_had_target, "unfollow: Did not exist in follow list");

        // Remove our actor from our target's followers:
		Entity& e_target = network[id_target];
		FollowerSet& target_followers = e_target.follower_set;

		// Necessary for use with follower set:
        follower_set::Context context(state, id_target);
		bool target_had_actor = target_followers.remove(context, config.follower_rates, id_actor);
		DEBUG_CHECK(target_had_actor, "unfollow: Did not exist in follower list");

		return true;
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
         * Luckily, this should be rare.
         */
        bool complete = false;
        while (!complete) {
            double u_1 = rng.rand_real_not0(); // get the first number with-in [0,1).
            int N = network.n_entities;

            // DECIDE WHAT TO DO:
            if (u_1 - (stats.prob_add) <= ZEROTOL) {
                // If we find ourselves in the add entity chuck of our cumulative function:
                complete = action_create_entity(time, N);
            } else if (u_1 - (stats.prob_add + stats.prob_follow) <= ZEROTOL) {
                int entity = analyzer_select_entity(state, FOLLOW_SELECT);
                complete = analyzer_follow_entity(state, entity, N, time);
            } else if (u_1 - (stats.prob_add + stats.prob_follow + stats.prob_tweet) <= ZEROTOL) {
                // The tweet event
                int entity = analyzer_select_entity(state, TWEET_SELECT);
                complete = action_tweet(entity);
            } else if (u_1 - (stats.prob_add + stats.prob_follow + stats.prob_tweet + stats.prob_retweet) <= ZEROTOL ) {
                RetweetChoice choice = analyzer_select_entity_retweet(state, RETWEET_SELECT);
                if (choice.id_author != -1) {
                    complete = action_retweet(choice, time);
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
        bool small_network = (network.n_entities < 40);
        if (config.output_stdout_summary && (floor(time) > prev_integer || small_network)) {
          output_summary_stats();
        }
    }

    /***************************************************************************
     * Helper functions
     ***************************************************************************/

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
            for (int i = 0; i < entity_types.size(); i ++) {
                following_data << entity_types[i].name << "\t";
                followers_data << entity_types[i].name << "\t";
                tweet_data << entity_types[i].name << "\t";
                retweet_data << entity_types[i].name << "\t";
                add_data << entity_types[i].name << "\t";
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
            for (int i = 0; i < entity_types.size(); i ++) {
                following_data << entity_types[i].n_follows << "\t";
                followers_data << entity_types[i].n_followers << "\t";
                tweet_data << entity_types[i].n_tweets << "\t";
                retweet_data << entity_types[i].n_retweets << "\t";
                add_data << entity_types[i].entity_list.size() << "\t";
            }
        following_data << "\n";
        followers_data << "\n";
        tweet_data << "\n";
        retweet_data << "\n";
        add_data << "\n";
        output_summary_stats(DATA_TIME);
        if (stats.n_outputs % STDOUT_OUTPUT_RATE == 0) {
        	output_summary_stats(cout, stdout_milestone_timer.get_microseconds() / 1000.0);
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
        ctrl_C_handler_install();
    }
    analyzer.main();
    signal(SIGINT, SIG_DFL);
    printf("'analyzer_main' took %.2f milliseconds.\n", timer.get_microseconds() / 1000.0);
}
