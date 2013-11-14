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
#include "io.h"

using namespace std;

static const double ZEROTOL = 1e-16; // enough precision for really really low add rate

/* Check that nothing has gone horribly wrong. Expensive. */
static void full_sanity_check(Network& network, int MAX_USERS) {
    for (int i = 0; i < MAX_USERS; i++) {
        DEBUG_CHECK(network[i].n_following <= MAX_FOLLOWING,
                "Cannot have more followers than MAX_FOLLOWING!");
    }
}

/* The Analyze struct encapsulates the many-parameter analyze function, and it's state. */
struct Analyzer {

    /***************************************************************************
     * Configuration and state variables of the analysis routines.
     ***************************************************************************/

    //** These can be accessed by any of the below functions
    //** This is the principle convenience of encapsulating all the related functions
    //** In this struct -- greatly reduce tedious parameter-passing
    // The network state
    Network network;
    /* Mersenne-twister random number generator */
    MTwist random_gen_state;
    /* Analysis parameters */
    int N_USERS;
    int MAX_USERS;
    int VERBOSE;
    int RANDOM_INCR;
    int P_OUT;
    int P_IN;

    double T_FINAL;

    double R_FOLLOW;
    double R_TWEET;
    double R_ADD;

    double R_TOTAL;
    double R_ADD_NORM;
    double R_FOLLOW_NORM;
    double R_TWEET_NORM;

    long int  N_STEPS, N_FOLLOWS, N_TWEETS;

    ofstream DATA_TIME; // Output file to plot data

    /***************************************************************************
     * Initialization functions
     ***************************************************************************/

    /* Initialization and loading of configuration.
     * Reads configuration from the given input file. */
    Analyzer(string input_file, int seed) :
            random_gen_state(seed) {
        N_USERS = load_config_var(input_file, "N_USERS");
        MAX_USERS = load_config_var(input_file, "MAX_USERS");
        VERBOSE = load_config_var(input_file, "VERBOSE");
        RANDOM_INCR = load_config_var(input_file, "RANDOM_INCR");
        P_OUT = load_config_var(input_file, "P_OUT");
        P_IN = load_config_var(input_file, "P_IN");

        T_FINAL = load_config_var(input_file, "T_FINAL");

        R_FOLLOW = load_config_var(input_file, "R_FOLLOW");
        R_TWEET = load_config_var(input_file, "R_TWEET");
        R_ADD = load_config_var(input_file, "R_ADD");

        N_STEPS = 0, N_FOLLOWS = 0, N_TWEETS = 0;
        set_rates();

        // The following allocates the (large) memory chunk proportional to MAX_USERS:
        network.preallocate(MAX_USERS);
        // Open our output file
        DATA_TIME.open("DATA_vs_TIME");
    }

    void set_rates() {
        R_TOTAL = R_ADD + R_FOLLOW * N_USERS + R_TWEET * N_USERS;

        //Normalize the rates
        R_ADD_NORM = R_ADD / R_TOTAL;
        R_FOLLOW_NORM = R_FOLLOW * N_USERS / R_TOTAL;
        R_TWEET_NORM = R_TWEET * N_USERS / R_TOTAL;
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

        full_sanity_check(network, MAX_USERS);

        // Print why program stopped
        string maximum_what =
                time < T_FINAL ? "number of users" : "amount of time steps";
        cout << "\nReached maximum " << maximum_what
                << ".\n\n- - Program terminated - -\n\n";

        if (P_OUT == 1) {
            POUT(network, MAX_USERS);
        } else {
            // nothing is done here
        }

        DATA_TIME.close();
    }

    void step_time(double& TIME) {
        double prev_floor = floor(TIME);
        if (RANDOM_INCR == 1) {
            // increment by random time
            TIME += -log(rand_real()) / R_TOTAL;
        } else {
            TIME += 1 / R_TOTAL;
        }
        // Output on every new integral time milestone:
        if (floor(TIME) > prev_floor) {
            output(TIME);
        }
    }

    // Performs one step of the analysis routine.
    // Takes old time, returns new time
    double step_analysis(double TIME) {
        double u_1 = random_gen_state.genrand_real2(); // get the first number with-in [0,1).

        // DECIDE WHAT TO DO
        //##############################################################################
        // If we find ourselves in the add user chuck of our cumulative function
        if (u_1 - (R_ADD_NORM) <= ZEROTOL) {
            N_USERS++;
            //call to function to decide which user to add
        }

        // If we find ourselves in the bond node chunk of our cumulative function

        else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM) <= ZEROTOL) {
            double val = u_1 - R_ADD_NORM;
            int user = val / (R_FOLLOW_NORM / N_USERS); // this finds the user
            Person& p = network[user];
            if (p.n_following < MAX_FOLLOWING) {
                N_FOLLOWS++;
                p.follows[p.n_following] = rand_int(N_USERS);
                p.n_following++;
            }
        }

        // if we find ourselves in the tweet chuck of the cumulative function
        else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM + R_TWEET_NORM) <= ZEROTOL) {
            N_TWEETS++;
            double val = u_1 - R_ADD_NORM - R_FOLLOW_NORM;
            int user = val / (R_TWEET_NORM / N_USERS); // this finds the user
        }

        else {
            cout << "Disaster, event out of bounds" << endl;
        }

        step_time(TIME);
        N_STEPS++;
        //update the rates if n_users has changed
        set_rates();

        return TIME;
    }

    /***************************************************************************
     * Helper functions
     ***************************************************************************/

    /* Using Mersenne-twister, grab an integer from 0 to max, non-inclusive (ie appropriate for array lengths). */
    int rand_int(int max) {
        return random_gen_state.genrand_int32() % max;
    }
    /* Using Mersenne-twister, grab a real number within [0,1) */
    double rand_real() {
        return random_gen_state.genrand_real2();
    }

    void output(double TIME) {
        static int n_outputs = 0;


        double DYNAMIC_ADD_RATE = N_USERS / TIME, DYNAMIC_FOLLOW_RATE = (R_ADD
                * TIME * R_FOLLOW + R_FOLLOW / 2) / N_USERS,
                DYNAMIC_TWEET_RATE = (R_ADD * TIME * R_TWEET + R_TWEET / 2)
                        / N_USERS;

        if (n_outputs % 50 == 0) {
            DATA_TIME << "\nTime\t\tUsers\t\tFollows\t\tTweets\n\n";
        }
//        if (VERBOSE == 0) {
//            cout << fixed << setprecision(10) << "INCREMENT: " << N_STEPS << "\t"
//                    << "TIME_(min): " << TIME << "\t"
//                    << "N_USERS: " << N_USERS << "\t"
//                    << "N_FOLLOWS: " << N_FOLLOWS << "\t"
//                    << "N_TWEETS: " << N_TWEETS << "\t"
//                    << "R_ADD_NORM: " << R_ADD_NORM << "\t"
//                    << "R_FOLLOW_NORM: " << R_FOLLOW_NORM << "\t"
//                    << "R_TWEET_NORM: " << R_TWEET_NORM << "\t"
//                    << "DYNAMIC_ADD_RATE: " << DYNAMIC_ADD_RATE << "\t"
//                    << "DYNAMIC_FOLLOW_RATE: " << DYNAMIC_FOLLOW_RATE << "\t"
//                    << "DYNAMIC_TWEET_RATE: " << "\t" << DYNAMIC_TWEET_RATE
//                    << "\n";
//        } else if (VERBOSE == 1) {
        DATA_TIME << fixed << setprecision(2) << TIME << "\t\t" << N_USERS << "\t\t"
                << N_FOLLOWS << "\t" << N_TWEETS << "\t\n";
        n_outputs++;
    }
};

// Run a network simulation using the given input file's parameters
void analyze_network(std::string input_file, int seed) {
    Analyzer analyzer(input_file, seed);
    analyzer.run_analysis();
}
