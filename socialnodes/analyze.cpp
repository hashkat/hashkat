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
    CategoryGroup tweet_ranks;
	CategoryGroup follow_ranks;
    /* Mersenne-twister random number generator */
    MTwist random_gen_state;
    /* Analysis parameters */
    int N_USERS;
    int MAX_USERS;
    int VERBOSE;
    int RANDOM_INCR;
    int P_OUT;
    int P_IN;
    int VISUALIZE;

    double T_FINAL;

    double R_TOTAL;
    double R_ADD_NORM;
    double R_FOLLOW_NORM;
    double R_TWEET_NORM;

    long int N_STEPS, N_FOLLOWS, N_TWEETS;

    UserType user_types[N_USERTYPES];

    ofstream DATA_TIME; // Output file to plot data

    map<string, string> raw_config; // Contents of INFILE
    map<string, double> config; // Contents of INFILE

    /***************************************************************************
     * Initialization functions
     ***************************************************************************/

    /* Initialization and loading of configuration.
     * Reads configuration from the given input file. */
    Analyzer(map<string, string>& raw_conf, map<string, double>& conf, int seed) :
            random_gen_state(seed) {
    	raw_config = raw_conf;
    	config = conf;
        N_USERS = config["N_USERS"];
        MAX_USERS = config["MAX_USERS"];
        VERBOSE = config["VERBOSE"];
        RANDOM_INCR = config["RANDOM_INCR"];
        P_OUT = config["P_OUT"];
        P_IN = config["P_IN"];

        VISUALIZE = config["VISUALIZE"];
        T_FINAL = config["T_FINAL"];

        N_STEPS = 0, N_FOLLOWS = 0, N_TWEETS = 0;
        set_rates();

        // The following allocates a memory chunk proportional to MAX_USERS:
        network.preallocate(MAX_USERS);
        follow_set_grower.preallocate(FOLLOW_SET_MEM_PER_USER * MAX_USERS);

        DATA_TIME.open("DATA_vs_TIME");
        set_initial_usertypes();
        set_usertype_probabilities();
        set_initial_categories();
		set_initial_follow_categories();
		set_initial_follow_categories_probabilities();
    }

    void initialize_category(CategoryGroup& group, const char* parameter) {
    	vector<double> thresholds = parse_numlist(raw_config[parameter]);
		for (int i = 0; i < thresholds.size(); i++) {
			group.categories.push_back(Category(thresholds[i]));
		}
		// Sentinel of sorts, swallows everything else:
		group.categories.push_back(Category(HUGE_VAL));
    }
    void set_initial_categories() {
    	initialize_category(tweet_ranks, "TWEET_THRESHOLDS");
    	initialize_category(follow_ranks, "FOLLOW_THRESHOLDS");
    }
	
    void set_initial_follow_categories() {
    	vector<double> follow_thresholds = parse_numlist(raw_config["FOLLOW_THRESHOLDS"]);
    	for (int i = 0; i < follow_thresholds.size(); i++) {
    		follow_ranks.categories.push_back(Category(follow_thresholds[i]));
    	}
    	// Sentinel of sorts, swallows everything else:
    	follow_ranks.categories.push_back(Category(HUGE_VAL));
    }
	
	vector<double> follow_popular_probabilities; 
    void set_initial_follow_categories_probabilities() {
    	vector<double> follow_probabilities = parse_numlist(raw_config["FOLLOW_THRESHOLDS_PROBABILITIES"]);
    	for (int i = 0; i < follow_probabilities.size(); i++) {
    		follow_popular_probabilities.push_back(follow_probabilities[i]);
    	}
    }

    void set_usertype_probabilities() {
        user_types[UT_NORMAL_INDEX].R_ADD = config["R_ADD_NORMAL"];
        user_types[UT_CELEB_INDEX].R_ADD = config["R_ADD_CELEB"];
        user_types[UT_ORG_INDEX].R_ADD = config["R_ADD_ORG"];
        user_types[UT_BOT_INDEX].R_ADD = config["R_ADD_BOT"];

        user_types[UT_NORMAL_INDEX].R_FOLLOW = config["R_FOLLOW_NORMAL"];
        user_types[UT_CELEB_INDEX].R_FOLLOW = config["R_FOLLOW_CELEB"];
        user_types[UT_ORG_INDEX].R_FOLLOW = config["R_FOLLOW_ORG"];
        user_types[UT_BOT_INDEX].R_FOLLOW = config["R_FOLLOW_BOT"];

        double add_total = 0, follow_total = 0;
        for (int i = 0; i < N_USERTYPES; i++) {
            add_total += user_types[i].R_ADD;
            follow_total += user_types[i].R_FOLLOW;
        }
        for (int i = 0; i < N_USERTYPES; i++) {
            user_types[i].R_ADD /= add_total;
            user_types[i].R_FOLLOW /= follow_total;
        }
    }

    void set_rates() {
        double R_FOLLOW = config["R_FOLLOW"];
        double R_TWEET = config["R_TWEET"];
        double R_ADD = config["R_ADD"];

        R_TOTAL = R_ADD + R_FOLLOW * N_USERS + R_TWEET * N_USERS;
        //Normalize the rates
        R_ADD_NORM = R_ADD / R_TOTAL;
        R_FOLLOW_NORM = R_FOLLOW * N_USERS / R_TOTAL;
        R_TWEET_NORM = R_TWEET * N_USERS / R_TOTAL;
    }

    void set_initial_usertypes() {
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

    // MAIN ANALYSIS ROUTINE
    /* Run the main analysis routine using this config. */
    void run_analysis() {
        double time = 0;
        while (time < T_FINAL && N_USERS < MAX_USERS) {
        	time = step_analysis(time);
        }

        // Print why program stopped
        string maximum_what =
                time < T_FINAL ? "number of users" : "amount of time steps";
        cout << "\nReached maximum " << maximum_what
                << ".\n\n- - Program terminated - -\n\n";

        if (P_OUT == 1) {
            POUT(network, MAX_USERS, N_USERS);
        } else {
            // nothing is done here
        }
        if (P_IN == 1) {
            PIN(network, MAX_USERS, N_USERS);
        } else {
            // nothing is done here
        }
        if (VISUALIZE == 1)
        {
            output_position(network, N_USERS);
        }
        else
        {
            // nothing is done here
        }
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
            output(TIME);
        }
    }

    /* Create a person at the given index.
     * The index should be an empty user slot. */
    void action_create_person(double creation_time, int index) {
		Person& p = network[index];
		p.creation_time = creation_time;
		double rand_num = rand_real_not0();
		for (int i = 0; i < N_USERTYPES; i++) {
			if (rand_num <= user_types[i].R_ADD) {
				p.type = i;
                                user_types[i].user_list.push_back(index);
				break;
			}
			rand_num -= user_types[i].R_ADD;
		}
	}

    /* decides which user to follow based on the rates in the INFILE */
	void action_follow_person(int user, int n_users) {
		Person& p = network[user];
		int user_to_follow = -1;
		
		user_to_follow = rand_int(n_users);
		/*
		double first_rand_num = rand_real_not0();
	
	
	    // if we want to follow someone based on their title
		for (int i = 0; i < N_USERTYPES; i++) {
			if (first_rand_num <= user_types[i].R_FOLLOW) {
				if (user_types[i].user_list.size() == 0) {
					return;
				}
				user_to_follow = rand_int(user_types[i].user_list.size());
				Person& p2 = network[user_to_follow];
				p2.n_followers ++;
				break;
			}
			first_rand_num -= user_types[i].R_FOLLOW;
		}*/
		DEBUG_CHECK(user_to_follow != -1, "Logic error");
		if (add_follow(p, user_to_follow)) {
			N_FOLLOWS++; // We were able to add the follow; almost always the case.
		}
		
		//if we want to follow someone based on the number of followers a user has
		
	}

	void action_tweet(int user) {
		Person& p = network[user];
		p.n_tweets++;
		tweet_ranks.categorize(user, p.n_tweets);
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
			action_follow_person(user, N_USERS);
        } else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM + R_TWEET_NORM) <= ZEROTOL) {
        	// If we find ourselves in the tweet chuck of the cumulative function:
            N_TWEETS++;
            // User to send the tweet:
            int user = rand_int(network.n_persons);
            action_tweet(user);
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

    void output(ostream& stream, double TIME) {
    	stream  << fixed << setprecision(2) << TIME << "\t\t" << N_USERS
                << "\t\t" << N_FOLLOWS << "\t\t" << N_TWEETS << "\t\n";
    }
    void output(double TIME) {
        static int n_outputs = 0;

		const char* HEADER = "\n#Time\t\tUsers\t\tFollows\t\tTweets\n\n";
        if (n_outputs % (25 * STDOUT_OUTPUT_RATE) == 0) {
        	cout << HEADER;
        }
        if (n_outputs % 500 == 0) {
            DATA_TIME << HEADER;
        }

        output(DATA_TIME, TIME);
        if (n_outputs % STDOUT_OUTPUT_RATE == 0) {
        	output(cout, TIME);
        }

        n_outputs++;
    }
};

// Run a network simulation using the given input file's parameters
void analyze_network(map<string, string>& raw_config, map<string, double>& num_config, int seed) {
    Analyzer analyzer(raw_config, num_config, seed);
    analyzer.run_analysis();
}
