#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>

// Local includes:
#include "analyze.h"
#include "util.h"
#include "follow_set.h"
#include "io.h"
#include "network.h"

using namespace std;

static const double ZEROTOL = 1e-16; // enough precision for really really low add rate

/* The Analyze struct encapsulates the many-parameter analyze function, and its state. */
struct Analyzer {

    /***************************************************************************
     * Configuration and state variables of the analysis routines.
     ***************************************************************************/

    //** These can be accessed by any of the below functions
    //** This is the principle convenience of encapsulating all the related functions
    //** In this struct -- greatly reduce tedious parameter-passing
    // The network state
    Network network;
    FollowSetGrower follow_set_grower;
	// categories for tweeting, following and retweeting
    CategoryGroup tweet_ranks;
	CategoryGroup follow_ranks;
	CategoryGroup retweet_ranks;
	
    /* Mersenne-twister random number generator */
    MTwist random_gen_state;
    /* Analysis parameters */
    int N_USERS;
    int MAX_USERS;
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

	// these values may get large, hence the long int
    long int N_STEPS, N_FOLLOWS, N_TWEETS, N_RETWEETS;

	// struct for the user classes, see network.h for specifications
    UserType user_entities[N_ENTITIES];

    ofstream DATA_TIME; // Output file to plot data

    map<string, string> raw_config; // Contents of INFILE
    map<string, double> config; // Contents of INFILE
    // Cached config variables, for performance:
    bool config_output_summary_stats;

    /***************************************************************************
     * Initialization functions
     ***************************************************************************/

    /* Initialization and loading of configuration.
     * Reads configuration from the given input file. */
    Analyzer(map<string, string>& conf , int seed) :
            random_gen_state(seed) {
    	raw_config = conf;
    	// For convenience, have all numbers preparsed as if they were numbers (errors result in 0):
    	for (map<string, string>::iterator it = conf.begin(); it != conf.end(); ++it) {
    	    config[it->first] = parse_num(it->second);
    	}
        N_USERS = config["N_USERS"];
        MAX_USERS = config["MAX_USERS"];
        VERBOSE = config["VERBOSE"];
        RANDOM_INCR = config["RANDOM_INCR"];
		FOLLOW_METHOD = config["FOLLOW_METHOD"];
		BARABASI = config["BARABASI"];

        T_FINAL = config["T_FINAL"];
        config_output_summary_stats = (config["OUTPUT_SUMMARY"] != 0);

        N_STEPS = 0, N_FOLLOWS = 0, N_TWEETS = 0, N_RETWEETS = 0;
        set_rates();

        // The following allocates a memory chunk proportional to MAX_USERS:
        network.preallocate(MAX_USERS);
        follow_set_grower.preallocate(FOLLOW_SET_MEM_PER_USER * MAX_USERS);

        DATA_TIME.open("DATA_vs_TIME");
       
        set_entity_probabilities();
		set_initial_entities();
        set_initial_categories();
		set_follow_rank_probabilities();
		set_initial_follow_categories();
    }
	// this is a helper function for the function below, essentially it takes the thresholds
	// and sets up the categories based on the INFILE
    void initialize_category(CategoryGroup& group, const char* parameter) {
    	vector<double> thresholds = parse_numlist(raw_config[parameter]);
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
	vector <double> follow_probabilities;
	void set_follow_rank_probabilities() {
		if (BARABASI == 1){
			for (int i = 1; i < MAX_USERS; i ++) {
				follow_probabilities.push_back(i);
				follow_ranks.categories.push_back(Category(i-1));
			}
		}
		else {
			vector<double> set_probabilities = parse_numlist(raw_config["FOLLOW_THRESHOLDS_PROBABILITIES"]);
			for (int i = 0; i < set_probabilities.size(); i ++) {
				Category& C = follow_ranks.categories[i];
				follow_probabilities.push_back(set_probabilities[i]);
			}
		}
	}
	
	// this makes sure that any initial users are categorized properly before simulation is even run
	// ???????? does this need to be done for tweeting and retweeting ???????????
	void set_initial_follow_categories() {
		for (int i = 0; i < N_USERS; i ++) {
			Person& p = network[i];
			follow_ranks.categorize(i, p.n_followers);
			
		}
	}
	
	// this gets the probabilities for the 'follow user by class' method
    void set_entity_probabilities() {
        user_entities[UT_NORMAL_INDEX].R_ADD = config["R_ADD_NORMAL"];
        user_entities[UT_CELEB_INDEX].R_ADD = config["R_ADD_CELEB"];
        user_entities[UT_ORG_INDEX].R_ADD = config["R_ADD_ORG"];
        user_entities[UT_BOT_INDEX].R_ADD = config["R_ADD_BOT"];

        user_entities[UT_NORMAL_INDEX].R_FOLLOW = config["R_FOLLOW_NORMAL"];
        user_entities[UT_CELEB_INDEX].R_FOLLOW = config["R_FOLLOW_CELEB"];
        user_entities[UT_ORG_INDEX].R_FOLLOW = config["R_FOLLOW_ORG"];
        user_entities[UT_BOT_INDEX].R_FOLLOW = config["R_FOLLOW_BOT"];

		// normalize the different user probabilities
        double add_total = 0, follow_total = 0;
        for (int i = 0; i < N_ENTITIES; i++) {
            add_total += user_entities[i].R_ADD;
            follow_total += user_entities[i].R_FOLLOW;
        }
        for (int i = 0; i < N_ENTITIES; i++) {
            user_entities[i].R_ADD /= add_total;
            user_entities[i].R_FOLLOW /= follow_total;
        }
    }

	// after every iteration, make sure the rates are updated accordingly
    void set_rates() {
        double R_FOLLOW = config["R_FOLLOW"];
        double R_TWEET = config["R_TWEET"];
        double R_ADD = config["R_ADD"];
		double R_RETWEET = config["R_RETWEET"];

        R_TOTAL = R_ADD + R_FOLLOW * N_USERS + R_TWEET * N_USERS + R_RETWEET * N_USERS;
        //Normalize the rates
        R_ADD_NORM = R_ADD / R_TOTAL;
        R_FOLLOW_NORM = R_FOLLOW * N_USERS / R_TOTAL;
        R_TWEET_NORM = R_TWEET * N_USERS / R_TOTAL;
		R_RETWEET_NORM = R_RETWEET * N_USERS / R_TOTAL;
    }
	
	// make sure any initial users are given a title based on the respective probabilities
    void set_initial_entities() {
        for (int i = 0; i < N_USERS; i ++) {
             action_create_person(0.0, i);
        }
    }

    /***************************************************************************
     * Person mutation routines
     ***************************************************************************/
    // Return whether the follow could be added, or if we have run out of buffer room.
    bool add_follow(Person& person, int follow) {
        return follow_set_grower.add_follow(person.follow_set, follow);
    }

    /***************************************************************************
     * Analysis routines
     ***************************************************************************/

    // Entry point to all analysis:
    /* Conceptually this is the true entry point to our program,
     * after the messy configuration and allocation is done. */
    void main() {
        double end_time = run_network_simulation();
        output_network_statistics(end_time);
    }

    // ROOT ANALYSIS ROUTINE
    /* Run the main analysis routine using this config. */
    double run_network_simulation() {
        double time = 0;
        while (time < T_FINAL && N_USERS < MAX_USERS) {
        	time = step_analysis(time);
        }
        return time;

    }
    // ROOT OUTPUT ROUTINE
    /* After 'analyze', print the results of the computations. */
    void output_network_statistics(double end_time) {
        // Print why program stopped
        if (!config["SILENT"]) {
            string maximum_what =
                    end_time < T_FINAL ? "number of users" : "amount of time steps";
            cout << "\nReached maximum " << maximum_what
                    << ".\n\n- - Program terminated - -\n\n";
        }

        // Depending on our INFILE/configuration, we may output various analysis
        if (config["P_OUT"]) {
            POUT(network, MAX_USERS, N_USERS, N_FOLLOWS);
        }
        if (config["P_IN"]) {
            PIN(network, MAX_USERS, N_USERS, R_FOLLOW_NORM);
        }		
        if (config["VISUALIZE"]) {
            output_position(network, N_USERS);
        }
		/* ADD FUNCTIONS THAT RUN AFTER NETWORK IS BUILT HERE */
		if (BARABASI != 1) {
		Categories_Check(tweet_ranks, follow_ranks, retweet_ranks);
		}
		Cumulative_Distro(network, MAX_USERS, N_USERS, N_FOLLOWS);
		entity_statistics(network, N_FOLLOWS, N_USERS, N_ENTITIES, user_entities);
		
        DATA_TIME.close();
    }

    void step_time(double& TIME) {
        double prev_floor = floor(TIME / FILE_OUTPUT_RATE);
        if (RANDOM_INCR == 1) {
            // increment by random time
            TIME += -log(rand_real_not0()) / R_TOTAL;
        } else {
            TIME += 1 / R_TOTAL;
        }
        // Output on every new integral time milestone:
        if (floor(TIME / FILE_OUTPUT_RATE) > prev_floor) {
            if (config_output_summary_stats) {
                output_summary_stats(TIME);
            }
        }
    }

    /* Create a person at the given index.
     * The index should be an empty user slot. */
    void action_create_person(double creation_time, int index) {
		Person& p = network[index];
		p.creation_time = creation_time;
		double rand_num = rand_real_not0();
		for (int i = 0; i < N_ENTITIES; i++) {
			if (rand_num <= user_entities[i].R_ADD) {
				p.entity = i;
                user_entities[i].user_list.push_back(index);
				follow_ranks.categorize(index, p.n_followers);
				break;
			}
			rand_num -= user_entities[i].R_ADD;
		}
		if (BARABASI == 1){
			action_follow_person(index, index, creation_time);
		}
	}

    /* decides which user to follow based on the rates in the INFILE */
	void action_follow_person(int user, int n_users, double time_of_follow) {
		Person& p1 = network[user];
		int user_to_follow = -1;
		double rand_num = rand_real_not0();
				
		// if we want to do random follows
		if (FOLLOW_METHOD == 0) {
			// find a random user within [0:number of users - 1]	
			user_to_follow = rand_int(n_users);
		}
		
		// if we want to use a preferential follow method
		if (FOLLOW_METHOD == 1) {
			double sum_of_weights = 0;
			double updating_follow_probabilities[follow_probabilities.size()];
			/* search through the probabilites for each threshold and find
			   the right bin to land in */
			for (int i = 0; i < follow_probabilities.size(); i ++){
				// look at each category
				Category& C = follow_ranks.categories[i];
				updating_follow_probabilities[i] = follow_probabilities[i]*C.users.size();
				sum_of_weights += C.users.size()*follow_probabilities[i];				
			}
			for (int i = 0; i < follow_probabilities.size(); i ++ ){
				Category& C = follow_ranks.categories[i];
	 		   	updating_follow_probabilities[i] /= sum_of_weights;
			} 
			for (int i = 0; i < follow_probabilities.size(); i ++) {
				if (rand_num - updating_follow_probabilities[i] <= ZEROTOL) {
					// point to the category we landed in
					Category& C = follow_ranks.categories[i];
					// make sure we're not pulling a user from an empty list
					if (C.users.size() != 0) {
						// pull a random user from whatever bin we landed in and break so we do not continue this loop
						user_to_follow = C.users[rand_int(C.users.size())];
						break;
					}
				}
				// part of the above search
				rand_num -= updating_follow_probabilities[i];
			}
		}
		
		// if we want to follow by user class
		if (FOLLOW_METHOD == 2) {
			/* search through the probabilities for each entity and find the right bin to land in */
			for (int i = 0; i < N_ENTITIES; i++) {
				if (rand_num <= user_entities[i].R_FOLLOW) {
					// make sure we're not pulling from an empty list
					if (user_entities[i].user_list.size() != 0) {
						// pull the user from whatever bin we landed in and break so we dont continue this loop
						user_to_follow = user_entities[i].user_list[rand_int(user_entities[i].user_list.size())];
						break;
					}
				}
				// part of the above search
				rand_num -= user_entities[i].R_FOLLOW;
			}					
		}
		//Retweet follow method
		if (FOLLOW_METHOD == 3) {
			if (rand_num > 0.5) {
				// grab the latest user
				if (p1.retweet_userlist.size() != 0) {
					// get the last user in the list and see when the retweet happened
					double last_time = p1.retweet_userlist_time[p1.retweet_userlist_time.size() - 1];
					int last_user = p1.retweet_userlist[p1.retweet_userlist.size() - 1];
					// if the retweet happened in the last 48 hours
					if (time_of_follow - last_time < 2880) {
						user_to_follow = p1.retweet_userlist[p1.retweet_userlist.size() - 1];
					}
				}
			}
			else {
				user_to_follow = rand_int(n_users);
			}
		}
		// check and make sure we are not following ourself, or we are following user -1
		if (LIKELY(user != user_to_follow && user_to_follow != -1)) {
			DEBUG_CHECK(user_to_follow != -1, "Logic error");
			// point to the person who is being followed
			Person& p2 = network[user_to_follow];
			if (add_follow(p1, user_to_follow) /*&& add_follow(p2, user)*/) {
				// based on the number of followers p2 has, check to make sure we're still categorized properly
				p2.n_followers ++;
				follow_ranks.categorize(user_to_follow, p2.n_followers);
				N_FOLLOWS++; // We were able to add the follow; almost always the case.
			}
        } 
		else {
			// no follow is added, this is unlikely
            return;
        }
	}

	// function to handle the tweeting
	void action_tweet(int user) {
		// This is the person tweeting
		Person& p = network[user];
		// increase the number of tweets the user had by one
		p.n_tweets++;
		tweet_ranks.categorize(user, p.n_tweets);
	}
	
	void action_retweet(int user, double time_of_retweet) {
		Person& retweetee = network[user];
		FollowSet& f = retweetee.follow_set;
		if (f.n_following != 0){
			int user_retweeted = f.location[rand_int(f.n_following)];
			for (int i = 0; i < network.n_following(user); i ++){
				Person& reciever = network[network.follow_i(user,i)];
				reciever.retweet_userlist.push_back(user_retweeted);
				reciever.retweet_userlist_time.push_back(time_of_retweet);
				/* if the retweet list has more than 5 user ID's, lets make sure they only have 5*/
				if (reciever.retweet_userlist.size() > 5) {
					reciever.retweet_userlist.erase(reciever.retweet_userlist.begin());
					reciever.retweet_userlist_time.erase(reciever.retweet_userlist_time.begin());
				}
			}
		}
		N_RETWEETS ++;
	}

    // Performs one step of the analysis routine.
    // Takes old time, returns new time
    double step_analysis(double TIME) {
        double u_1 = rand_real_not1(); // get the first number with-in [0,1).

        // DECIDE WHAT TO DO:
        if (u_1 - (R_ADD_NORM) <= ZEROTOL) {
			// If we find ourselves in the add user chuck of our cumulative function:
            action_create_person(TIME, N_USERS);
			N_USERS ++;
            //TODO: call to function to decide which user to add
        } else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM) <= ZEROTOL) {
        	// If we find ourselves in the bond node chunk of our cumulative function:
            double val = u_1 - R_ADD_NORM;
			int user = val / (R_FOLLOW_NORM / N_USERS); // this finds the user
			Person& p = network[user];
			action_follow_person(user, N_USERS, TIME);
        } else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM + R_TWEET_NORM) <= ZEROTOL) {
        	// If we find ourselves in the tweet chuck of the cumulative function:
            N_TWEETS++;
            // User to send the tweet:
            int user = rand_int(network.n_persons);
            action_tweet(user);
        } else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM + R_TWEET_NORM + R_RETWEET_NORM) <= ZEROTOL ) {
			double val = u_1 - (R_ADD_NORM + R_FOLLOW_NORM + R_TWEET_NORM);
			int user = val / (R_RETWEET_NORM / N_USERS);
			action_retweet(user, TIME);
        } else {
            cout << "Disaster, event out of bounds" << endl;
        }

        step_time(TIME);
        N_STEPS++;
        //update the rates if n_users has changed
        set_rates();

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
    	stream  << fixed << setprecision(2) << TIME << "\t\t" << N_USERS
                << "\t\t" << N_FOLLOWS << "\t\t" << N_TWEETS << "\t\t" << N_RETWEETS << "\t\n";
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
        	/*// In standard output only, print the follow lists of the top 10 users (by follows):
        	int capped_len = std::min(10, network.n_persons);
        	for (int i = 0; i < capped_len; i++) {
        		FollowSet& follow_set = network[i].follow_set;
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
void analyze_network(map<string, string>& config, int seed) {
    Analyzer analyzer(config, seed);
    analyzer.main();
}
