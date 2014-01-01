#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <deque>

// Local includes:
#include "analyze.h"
#include "util.h"
#include "MemPoolVector.h"
#include "network.h"

#include <signal.h>

using namespace std;

static const double ZEROTOL = 1e-16; // enough precision for really really low add rate

volatile int CTRL_C_ATTEMPTS = 0;

static const int CTRL_C_ATTEMPTS_TO_ABORT = 4;
// Handler for SIGINT -- sent by Ctrl-C on command-line. Allows us to stop our program gracefully!
static void ctrl_C_handler(int __dummy) {
    CTRL_C_ATTEMPTS++;
    if (CTRL_C_ATTEMPTS > CTRL_C_ATTEMPTS_TO_ABORT) {
        panic("User demands abort!");
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

    //** These can be accessed by any of the below functions
    //** This is the principle convenience of encapsulating all the related functions
    //** In this struct -- greatly reduce tedious parameter-passing

    InfileConfig& config;

    // struct for the entity classes, see network.h for specifications
    EntityType* entity_types;

    // The network state
    Network& network;
    MemPoolVectorGrower& follow_set_grower;
	// categories for tweeting, following, retweeting, and age
    CategoryGroup& tweet_ranks;
	CategoryGroup& follow_ranks;
	CategoryGroup& retweet_ranks;

	vector <double> follow_probabilities, updating_follow_probabilities;
	vector <int> last_entity_ID;

    /* Mersenne-twister random number generator */
    MTwist random_gen_state;
    /* Analysis parameters */
    int MAX_ENTITIES;
    int VERBOSE;
    int RANDOM_INCR;
	int FOLLOW_METHOD;
	int BARABASI;

    double T_FINAL;
	// the are the rates we need to run the program
    double R_TOTAL;
    double R_ADD_NORM;
    double R_FOLLOW_NORM;
    double R_TWEET_NORM;
	double R_RETWEET_NORM;

	int n_months;
	// these values may get large, hence the long int
    long int N_STEPS, N_FOLLOWS, N_TWEETS, N_RETWEETS;

    ofstream DATA_TIME; // Output file to plot data

    // Cached config variables, for performance:
    bool config_output_summary_stats;

    /***************************************************************************
     * Initialization functions
     ***************************************************************************/

    /* Initialization and loading of configuration.
     * Reads configuration from the given input file. */
    Analyzer(AnalysisState& state, int seed) :
        //** Here, we initialize REFERENCES to the various analysis pieces.
        //** The only way to initialize references is through this construct -- called an 'initializer list'.
        //** This ensures that updates are witnessed in the caller's AnalysisState object.
            network(state.network), follow_set_grower(state.follow_set_grower),
            tweet_ranks(state.tweet_ranks), follow_ranks(state.follow_ranks),
            retweet_ranks(state.retweet_ranks), config(state.config), random_gen_state(seed) {
        // Make sure 'entity_types' also references into our AnalysisState object.
        entity_types = state.entity_types;

        network.n_entities = config["N_ENTITIES"];
        MAX_ENTITIES = config["MAX_ENTITIES"];
        VERBOSE = config["VERBOSE"];
        RANDOM_INCR = config["RANDOM_INCR"];
		FOLLOW_METHOD = config["FOLLOW_METHOD"];
		BARABASI = config["BARABASI"];

        T_FINAL = config["T_FINAL"];
        config_output_summary_stats = (config["OUTPUT_SUMMARY"] != 0);

        N_STEPS = 0, N_FOLLOWS = 0, N_TWEETS = 0, N_RETWEETS = 0, n_months = 0;
        set_rates(0.0);

        // The following allocates a memory chunk proportional to MAX_ENTITIES:
        network.preallocate(MAX_ENTITIES);
        follow_set_grower.preallocate(FOLLOW_SET_MEM_PER_USER * MAX_ENTITIES);

        DATA_TIME.open("DATA_vs_TIME");
       
        set_entity_probabilities();
        set_initial_categories();
		set_initial_entities();
		set_follow_rank_probabilities();
    }
    // make sure any initial entities are given a title based on the respective probabilities
    void set_initial_entities() {
        for (int i = 0; i < config["N_ENTITIES"]; i++) {
             action_create_entity(0.0, i);
        }
    }
	// this is a helper function for the function below, essentially it takes the thresholds
	// and sets up the categories based on the INFILE
    void initialize_category(CategoryGroup& group, const char* parameter) {
    	vector<double> thresholds = parse_numlist(config.raw_get(parameter));

		for (int i = 0; i < thresholds.size(); i++) {
			group.categories.push_back(Category(thresholds[i]));
		}
		// Sentinel of sorts, swallows everything else:
		group.categories.push_back(Category(HUGE_VAL));
    }
	
	// calls above function for the different category types
    void set_initial_categories() {
    	initialize_category(tweet_ranks, "TWEET_THRESHOLDS");
		if (BARABASI != 1) {
    		initialize_category(follow_ranks, "FOLLOW_THRESHOLDS");
		}
		initialize_category(retweet_ranks, "RETWEET_THRESHOLDS");
    }
	
	// function for follow probabilities, this is used for a preferential follow method
	void set_follow_rank_probabilities() {
		if (BARABASI == 1){
			for (int i = 1; i < MAX_ENTITIES; i ++) {
				follow_probabilities.push_back(i);
				follow_ranks.categories.push_back(Category(i-1));
			}
		}
		else {
			vector<double> set_probabilities = parse_numlist(config.raw_get("FOLLOW_THRESHOLDS_PROBABILITIES"));
			for (int i = 0; i < set_probabilities.size(); i ++) {
				Category& C = follow_ranks.categories[i];
				follow_probabilities.push_back(set_probabilities[i]);
			}
		}
	}
	
	// this gets the probabilities for the 'follow entity by class' method
    void set_entity_probabilities() {
        entity_types[ET_NORMAL_INDEX].R_ADD = config["R_ADD_NORMAL"];
        entity_types[ET_CELEB_INDEX].R_ADD = config["R_ADD_CELEB"];
        entity_types[ET_ORG_INDEX].R_ADD = config["R_ADD_ORG"];
        entity_types[ET_BOT_INDEX].R_ADD = config["R_ADD_BOT"];

        entity_types[ET_NORMAL_INDEX].R_FOLLOW = config["R_FOLLOW_NORMAL"];
        entity_types[ET_CELEB_INDEX].R_FOLLOW = config["R_FOLLOW_CELEB"];
        entity_types[ET_ORG_INDEX].R_FOLLOW = config["R_FOLLOW_ORG"];
        entity_types[ET_BOT_INDEX].R_FOLLOW = config["R_FOLLOW_BOT"];

		// normalize the different entity probabilities
        double add_total = 0, follow_total = 0;
        for (int i = 0; i < ET_AMOUNT; i++) {
            add_total += entity_types[i].R_ADD;
            follow_total += entity_types[i].R_FOLLOW;
        }
        for (int i = 0; i < ET_AMOUNT; i++) {
            entity_types[i].R_ADD /= add_total;
            entity_types[i].R_FOLLOW /= follow_total;
        }
    }

	// after every iteration, make sure the rates are updated accordingly
	vector<double> r_follow;
	vector<double> r_tweet;
	vector<double> r_retweet;
	vector<double> r_add;
	deque<int> entity_cap;
    void set_rates(double TIME) {
        double R_FOLLOW_INI = config["R_FOLLOW"];
        double R_TWEET_INI = config["R_TWEET"];
        double R_ADD_INI = config["R_ADD"];
	double R_RETWEET_INI = config["R_RETWEET"];
	int N = network.n_entities;

	/*R_TOTAL = R_ADD_INI + R_FOLLOW_INI * N + R_TWEET_INI * N + R_RETWEET_INI * N;
	R_ADD_NORM = R_ADD_INI / R_TOTAL;
	R_FOLLOW_NORM = R_FOLLOW_INI * N  / R_TOTAL;
	R_TWEET_NORM = R_TWEET_INI * N / R_TOTAL;
	R_RETWEET_NORM = R_RETWEET_NORM * N / R_TOTAL;*/

		int approx_month = 24*60*30;
		int n_months = TIME / approx_month;
		if (n_months > entity_cap.size() - 1 || TIME == 0) {
			cout << "\nNumber of Months = " << n_months << "\n\n";
			entity_cap.push_front(N);

			// CHANGE YOUR RATES ACCORDINGLY - they are constant right now
			double change_follow_rate = R_FOLLOW_INI + R_FOLLOW_INI*(n_months);
			double change_tweet_rate = R_TWEET_INI;
			double change_retweet_rate = R_RETWEET_INI;
			double change_add_rate = R_ADD_INI;
			cout << "Follow rate = " << change_follow_rate << "\n";
			DEBUG_CHECK(change_follow_rate >= 0, "The follow rate can't be < 0");
			DEBUG_CHECK(change_tweet_rate >= 0, "The tweet rate can't be < 0");
			DEBUG_CHECK(change_retweet_rate >= 0, "The retweet rate can't be < 0");

			r_follow.push_back(change_follow_rate);
			r_tweet.push_back(change_tweet_rate);
			r_retweet.push_back(change_retweet_rate);
			r_add.push_back(change_add_rate);
		}
		int new_entities;
		double overall_follow_rate, overall_tweet_rate, overall_retweet_rate;
		if (R_ADD_INI == 0) {
		    overall_follow_rate = N * r_follow[n_months];
			overall_tweet_rate = N * r_tweet[n_months];
			overall_retweet_rate = N * r_retweet[n_months];
		}
		else {
			new_entities = N - entity_cap[n_months];
			overall_follow_rate = new_entities * r_follow[0], 
			overall_tweet_rate = new_entities * r_tweet[0],
			overall_retweet_rate = new_entities * r_retweet[0];
			for (int i = 1; i <= n_months; i ++) {
				overall_follow_rate += r_follow[i] * (entity_cap[i - 1] - entity_cap[i]);
				overall_tweet_rate += r_tweet[i] * (entity_cap[i - 1] - entity_cap[i]);
				overall_retweet_rate += r_retweet[i] * (entity_cap[i - 1] - entity_cap[i]);
			}
		}
		R_TOTAL = R_ADD_INI + overall_follow_rate + overall_tweet_rate + overall_retweet_rate; 
        	//Normalize the rates
        	R_ADD_NORM = R_ADD_INI / R_TOTAL;
        
		R_FOLLOW_NORM = overall_follow_rate / R_TOTAL;
		R_TWEET_NORM = overall_tweet_rate / R_TOTAL;
		R_RETWEET_NORM = overall_retweet_rate / R_TOTAL;
    }
	

    /***************************************************************************
     * Entity mutation routines
     ***************************************************************************/
    // Return whether the follow could be added, or if we have run out of buffer room.
    bool handle_follow(int actor, int target) {
        Entity& A = network[actor];
        Entity& T = network[target];
        bool was_added = follow_set_grower.add_if_possible(A.follow_set, target);
        if (was_added) {
            was_added = follow_set_grower.add_if_possible(T.follower_set, actor);
            return true;
        }
        return false;
    }
    /***************************************************************************
     * Analysis routines
     ***************************************************************************/

    // Entry point to all analysis:
    /* Conceptually this is the true entry point to our program,
     * after the messy configuration and allocation is done. */
    double main() {
        double end_time = run_network_simulation();
        return end_time;
    }

    // ROOT ANALYSIS ROUTINE
    /* Run the main analysis routine using this config. */
    double run_network_simulation() {
        ctrl_C_handler_install();
        double time = 0;
        while ( LIKELY(time < T_FINAL && network.n_entities < MAX_ENTITIES && CTRL_C_ATTEMPTS == 0) ) {
        	time = step_analysis(time);
        }
        return time;
    }

    void step_time(double& TIME, int n_entities) {
        double prev_milestone = floor(TIME / TIME_CATEGORIZATION_FREQUENCY);
        double prev_integer = floor(TIME);
        if (RANDOM_INCR == 1) {
            // increment by random time
            TIME += -log(rand_real_not0()) / R_TOTAL;
        } else {
            TIME += 1 / R_TOTAL;
        }

        // Categorize all entities based on time, on every new time milestone.
        bool at_milestone = (floor(TIME / TIME_CATEGORIZATION_FREQUENCY) > prev_milestone);
        if (at_milestone){
        	last_entity_ID.push_back(n_entities);
        }

        if (config_output_summary_stats && (floor(TIME) > prev_integer)) {
            output_summary_stats(TIME);
        }
    }

    /* Create a entity at the given index.
     * The index should be an empty entity slot. */
    void action_create_entity(double creation_time, int index) {
		Entity& e = network[index];
		e.creation_time = creation_time;
		double rand_num = rand_real_not0();
		for (int et = 0; et < ET_AMOUNT; et++) {
			if (rand_num <= entity_types[et].R_ADD) {
				e.entity = et;
                entity_types[et].entity_list.push_back(index);
				follow_ranks.categorize(index, e.follower_set.size);
				break;
			}
			rand_num -= entity_types[et].R_ADD;
		}
		if (BARABASI == 1){
			action_follow_entity(index, index, creation_time);
		}
		if (creation_time != 0) {
		network.n_entities++;
		}
    }

    /* decides which entity to follow based on the rates in the INFILE */
	void action_follow_entity(int entity, int n_entities, double time_of_follow) {
		Entity& e1 = network[entity];
		int entity_to_follow = -1;
		double rand_num = rand_real_not0();
		// if we want to do random follows
		if (FOLLOW_METHOD == 0) {
			// find a random entity within [0:number of entities - 1]	
			entity_to_follow = rand_int(n_entities);
		}

		// if we want to use a preferential follow method
		if (FOLLOW_METHOD == 1) {
			double sum_of_weights = 0;
			updating_follow_probabilities.resize(follow_probabilities.size());
			/* search through the probabilites for each threshold and find
			   the right bin to land in */
			for (int i = 0; i < follow_probabilities.size(); i ++){
				// look at each category
				Category& C = follow_ranks.categories[i];
				updating_follow_probabilities[i] = follow_probabilities[i]*C.entities.size();
				sum_of_weights += C.entities.size()*follow_probabilities[i];				
			}
			for (int i = 0; i < follow_probabilities.size(); i ++ ){
				Category& C = follow_ranks.categories[i];
	 		   	updating_follow_probabilities[i] /= sum_of_weights;
			} 
			for (int i = 0; i < follow_probabilities.size(); i ++) {
				if (rand_num - updating_follow_probabilities[i] <= ZEROTOL) {
					// point to the category we landed in
					Category& C = follow_ranks.categories[i];
					// make sure we're not pulling a entity from an empty list
					if (C.entities.size() != 0) {
						// pull a random entity from whatever bin we landed in and break so we do not continue this loop
						entity_to_follow = C.entities[rand_int(C.entities.size())];
						break;
					}
				}
				// part of the above search
				rand_num -= updating_follow_probabilities[i];
			}
		}
		
		// if we want to follow by entity class
		if (FOLLOW_METHOD == 2) {
			/* search through the probabilities for each entity and find the right bin to land in */
			for (int i = 0; i < ET_AMOUNT; i++) {
				if (rand_num <= entity_types[i].R_FOLLOW) {
					// make sure we're not pulling from an empty list
					if (entity_types[i].entity_list.size() != 0) {
						// pull the entity from whatever bin we landed in and break so we dont continue this loop
						entity_to_follow = entity_types[i].entity_list[rand_int(entity_types[i].entity_list.size())];
						break;
					}
				}
				// part of the above search
				rand_num -= entity_types[i].R_FOLLOW;
			}					
		}
		//Retweet follow method
		if (FOLLOW_METHOD == 3) {
			if (rand_num > 0.5) {
			    Retweet retweet;
			    if (e1.retweets.check_recent(retweet)) {
			        // grab the latest retweet
					// if the retweet happened in the last 48 hours
					if (time_of_follow - retweet.time < 2880) {
						entity_to_follow = retweet.original_tweeter;
					}
				}
			}
			else {
				entity_to_follow = rand_int(n_entities);
			}
		}
		// check and make sure we are not following ourself, or we are following entity -1
		if (LIKELY(entity != entity_to_follow && entity_to_follow != -1)) {
			// point to the entity who is being followed
			if (handle_follow(entity, entity_to_follow)) {
				// based on the number of followers the followed-entity has, check to make sure we're still categorized properly
				Entity& target = network[entity_to_follow];
				follow_ranks.categorize(entity_to_follow, target.follower_set.size);
				N_FOLLOWS++; // We were able to add the follow; almost always the case.
			}
        } 
		else {
			// no follow is added, this is unlikely
            return;
        }
	}

	// function to handle the tweeting
	void action_tweet(int entity) {
		// This is the entity tweeting
		Entity& e = network[entity];
		// increase the number of tweets the entity had by one
		e.n_tweets++;
		tweet_ranks.categorize(entity, e.n_tweets);
		if (e.n_tweets > 1000) {
			action_unfollow(entity);
		}
	}
	
	void action_retweet(int entity, double time_of_retweet) {
		Entity& retweetee = network[entity];
		FollowList& f = retweetee.follow_set;
		if (f.size != 0){
			int entity_retweeted = f.location[rand_int(f.size)];
			int n_following = network.n_following(entity);
            // Loop over all the entities that witness the event:
			for (int i = 0; i < n_following; i ++){
				Entity& audience = network[network.follow_i(entity,i)];
				Retweet retweet(entity_retweeted, time_of_retweet);
				audience.retweets.add(retweet);
			}
		}
		retweetee.n_retweets ++;
		N_RETWEETS ++;
	}
	
	void action_followback(int follower, int followee) {
		// now the followee will follow the follower back
		if (handle_follow(followee, follower)) {
				Entity& target = network[follower];
				follow_ranks.categorize(follower, target.follower_set.size);
				N_FOLLOWS++; // We were able to add the follow; almost always the case.
		} 
		else {
            return;
        }
	}
	
	void action_unfollow(int entity_id) {
	    // Broken, commented out -- AD
//		Entity& e1 = network[entity_id];
//		int unfollowing_location = rand_int(network.n_followers(entity_id));
//		Entity& e2 = network[unfollowing_location];
//		FollowerList& f1 = e1.follower_set;
//		FollowList& f2 = e2.follow_set;
		// remove unfollowing location from follower_list
		// remove entity id from follow_list
		
	}

    // Performs one step of the analysis routine.
    // Takes old time, returns new time
    double step_analysis(double TIME) {
        double u_1 = rand_real_not0(); // get the first number with-in [0,1).

        int N = network.n_entities;

        // DECIDE WHAT TO DO:
        if (u_1 - (R_ADD_NORM) <= ZEROTOL) {
			// If we find ourselves in the add entity chuck of our cumulative function:
            action_create_entity(TIME, N);
        } else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM) <= ZEROTOL) {
        	// If we find ourselves in the bond node chunk of our cumulative function:
            double val = u_1 - R_ADD_NORM;
			int entity = val / (R_FOLLOW_NORM / N); // this finds the entity
			Entity& p = network[entity];
			action_follow_entity(entity, N, TIME);
        } else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM + R_TWEET_NORM) <= ZEROTOL) {
        	// If we find ourselves in the tweet chuck of the cumulative function:
            N_TWEETS++;
            // User to send the tweet:
            int entity = rand_int(network.max_entities);
            action_tweet(entity);
        } else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM + R_TWEET_NORM + R_RETWEET_NORM) <= ZEROTOL ) {
			double val = u_1 - (R_ADD_NORM + R_FOLLOW_NORM + R_TWEET_NORM);
			int entity = val / (R_RETWEET_NORM / N);
			action_retweet(entity, TIME);
        } else {
            cout << "Disaster, event out of bounds" << endl;
        }

        step_time(TIME, N);
        N_STEPS++;
        //update the rates if n_entities has changed
        set_rates(TIME);

#ifdef SLOW_DEBUG_CHECKS
        static int i = 0;
        if ((i%1000)==0) {
        network.sanity_check();
        }
        i++;
#endif

        return TIME;
    }

    /***************************************************************************
     * Helper functions
     ***************************************************************************/

    /* Using Mersenne-twister, grab an integer from 0 to max, non-inclusive (ie appropriate for array lengths). */
    int rand_int(int max) {
        return random_gen_state.genrand_int32() % max;
    }
    /* Using Mersenne-twister, grab a real number within [0,1) with 53-bit resolution */
    double rand_real_not1() {
        return random_gen_state.genrand_res53();
    }
    /* Using Mersenne-twister, grab a real number within (0,1] with 53-bit resolution */
    double rand_real_not0() {
        return 1.0 - rand_real_not1();
    }
    /* Using Mersenne-twister, grab a real number within [0,1] */
    double rand_real_with01() {
        return random_gen_state.genrand_real1();
    }

    void output_summary_stats(ostream& stream, double TIME) {
        stream << fixed << setprecision(2)
                << TIME << "\t\t"
                << network.n_entities << "\t\t"
                << N_FOLLOWS << "\t\t"
                << N_TWEETS << "\t\t"
                << N_RETWEETS << "\t\n";
    }

    void output_summary_stats(double TIME) {
        static int n_outputs = 0;

		const char* HEADER = "\n#Time\t\tUsers\t\tFollows\t\tTweets\t\tRetweets\n\n";
        if (n_outputs % (25 * STDOUT_OUTPUT_RATE) == 0) {
        	cout << HEADER;
        }
        if (n_outputs % 500 == 0) {
            DATA_TIME << HEADER;
        }

        output_summary_stats(DATA_TIME, TIME);
        if (n_outputs % STDOUT_OUTPUT_RATE == 0) {
        	output_summary_stats(cout, TIME);
        	/*// In standard output only, print the follow lists of the top 10 entities (by follows):
        	int capped_len = std::min(10, network.n_entitys);
        	for (int i = 0; i < capped_len; i++) {
        		MemPoolVector& follow_set = network[i].follow_set;
        		cout << (i+1) << ": ";
        		for (int i = 0; i < follow_set.n_following; i++) {
        			cout << ' ' << follow_set[i];
        		}
        		cout << '\n';
        	}*/
        }

        n_outputs++;
    }
};

// Run a network simulation using the given input file's parameters
void simulate_network(AnalysisState& analysis_state, int seed) {
    Analyzer analyzer(analysis_state, seed);
    analyzer.main();
}
