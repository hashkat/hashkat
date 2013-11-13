#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>

// Local includes:
#include "analyze.h"
#include "util.h"

using namespace std;

static double INFILE(string demand) {
    ifstream INCAR;

    INCAR.open("INFILE");
    if (!INCAR) {
        panic("Input file could not be found!\n");
    }

    vector<string> input;
    string text;
    double value;

    while (!INCAR.eof()) {
        INCAR >> text;
        input.push_back(text);
    }
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == demand) {
            value = atof(input[i + 2].c_str());
        }
    }
    INCAR.close();
    return value;
}

/* Reads configuration from the given input file */
void Analyzer::init_from_file() {
    N_USERS = INFILE("N_USERS");
    MAX_USERS = INFILE("MAX_USERS");
    network.preallocate(MAX_USERS);
    VERBOSE = INFILE("VERBOSE");
    RANDOM_INCR = INFILE("RANDOM_INCR");
    P_OUT = INFILE("P_OUT");
    P_IN = INFILE("P_IN");

    T_FINAL = INFILE("T_FINAL");

    R_FOLLOW = INFILE("R_FOLLOW");
    R_TWEET = INFILE("R_TWEET");
    R_ADD = INFILE("R_ADD");
}

void POUT(Network& network, int MAX_USERS) {
    int N_FOLLOW_DATA[MAX_FOLLOWING];
    for (int i = 0; i < MAX_FOLLOWING; i++) {
        N_FOLLOW_DATA[i] = 0;
    }

    ofstream OUTPUT;
    OUTPUT.open("P_OUT.dat");

    OUTPUT << "##### THIS IS THE P_OUT(K) FUNCTION DATA #####\n\n";

    for (int i = 0; i < MAX_USERS; i++) {
        N_FOLLOW_DATA[network[i].n_following]++;
    }
    for (int i = 0; i < MAX_FOLLOWING; i++) {
        OUTPUT << i - 0.5 << "\t" << N_FOLLOW_DATA[i] / double(MAX_USERS)
                << endl;
        OUTPUT << i + 0.5 << "\t" << N_FOLLOW_DATA[i] / double(MAX_USERS)
                << endl;
    }

    OUTPUT.close();
}

/* Run the main analysis routine using this config. */
void Analyzer::run() {
    double TIME = 0.0;    // the initial time is 0 minutes
    double zerotol = 1e-16; // enough precision for really really low add rate
    double R_TOTAL = R_ADD + R_FOLLOW * N_USERS + R_TWEET * N_USERS;

    //Normalize the rates

    double R_ADD_NORM = R_ADD / R_TOTAL;
    double R_FOLLOW_NORM = R_FOLLOW * N_USERS / R_TOTAL;
    double R_TWEET_NORM = R_TWEET * N_USERS / R_TOTAL;

    //#######################################################################

    //DECLARE OUTPUT FILE TO PLOT DATA
    //################################################
    ofstream DATA_TIME;
    DATA_TIME.open("DATA_vs_TIME");

    //###############################################

    //CHECK OUR NUMBER OF STEPS, TWEETS and FOLLOWS

    long int N_STEPS = 0, N_TWEETS = 0, N_FOLLOWS = 0;

    //RANDOM SEED
    // If we try the same seeds, we should expect to get the same answer.
    srand(time(NULL));

    //THIS IS THE MAIN LOOP
    //###############################################################################################
    while (TIME < T_FINAL && N_USERS < MAX_USERS) {
        // get the first uniform number [0,1). This selects a number from a long int (default RAND_MAX)

        double u_1 = (rand() + 1) / (double(RAND_MAX) + 1);
        // IT it seems like u_1 has more than 16 digits...what's with that?

        // DECIDE WHAT TO DO
        //##############################################################################
        // If we find ourselves in the add user chuck of our cumuative function
        if (u_1 - (R_ADD_NORM) <= zerotol) {
            N_USERS++;
            //call to function to decide which user to add
        }

        // If we find ourselves in the bond node chunk of our cumulative function

        else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM) <= zerotol) {
            N_FOLLOWS++;
            double val = u_1 - R_ADD_NORM;
            int user = val / (R_FOLLOW_NORM / N_USERS);  // this finds the user
            Person& p = network[user];
            p.follows[user] = int(round((rand() / double(RAND_MAX)) * N_USERS));
            p.n_following++;
        }

        // if we find ourselves in the tweet chuck of the cumulative function
        else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM + R_TWEET_NORM) <= zerotol) {
            N_TWEETS++;
            double val = u_1 - R_ADD_NORM - R_FOLLOW_NORM;
            int user = val / (R_TWEET_NORM / N_USERS); // this finds the user
        }

        else {
            cout << "Disaster, event out of bounds" << endl;
        }
        //##############################################################################

        // something wierd is going on here -- when n_users changes dynamically
        // we need that 2, but when n_users is fixed we dont need the 2

        double DYNAMIC_ADD_RATE = N_USERS / TIME, DYNAMIC_FOLLOW_RATE = (R_ADD*TIME*R_FOLLOW + R_FOLLOW/2)/ N_USERS, DYNAMIC_TWEET_RATE = (R_ADD*TIME*R_TWEET + R_TWEET/2) / N_USERS;

        // ##################################################################################################
        if (VERBOSE == 0) {
            DATA_TIME << fixed << setprecision(10) << "INCREMENT: " << N_STEPS << "\t"
                    << "TIME_(min): " << TIME << "\t"
                    << "RANDOM_NUM1: " << u_1 << "\t"
                    << "N_USERS: " << N_USERS << "\t"
                    << "N_FOLLOWS: " << N_FOLLOWS << "\t"
                    << "N_TWEETS: " << N_TWEETS << "\t"
                    << "R_ADD_NORM: " << R_ADD_NORM << "\t"
                    << "R_FOLLOW_NORM: " << R_FOLLOW_NORM << "\t"
                    << "R_TWEET_NORM: " << R_TWEET_NORM << "\t"
                    << "DYNAMIC_ADD_RATE: " << DYNAMIC_ADD_RATE << "\t"
                    << "DYNAMIC_FOLLOW_RATE: " << DYNAMIC_FOLLOW_RATE << "\t"
                    << "DYNAMIC_TWEET_RATE: " << "\t" << DYNAMIC_TWEET_RATE
                    << endl;
        } else if (VERBOSE == 1) {
            DATA_TIME << fixed << setprecision(5) << "TIME_(min):" << "\t"
                    << TIME << "\t"
                    << "NUMBER_OF_USERS:" << "\t" << N_USERS << "\t"
                    << "NUMBER_OF_FOLLOWS:" << "\t" << N_FOLLOWS << "\t"
                    << "NUMBER_OF_TWEETS:" << "\t" << N_TWEETS << "\t" << endl;
        }
        //SP else -> no output is written : There needs to be a more efficient way to do this. i.e. outside the main loop
        //##################################################################################################

        // TIME INCREMENTATION
        //####################################################

        //SP again we need to do this outside of the main loop.
        if (RANDOM_INCR == 1) {
            // get second random number
            double u_2 = (rand() + 1) / (double(RAND_MAX) + 1);

            // increment by random time
            TIME += -log(u_2) / R_TOTAL;
        } else {
            TIME += 1 / R_TOTAL;
        }
        //##################################################

        N_STEPS++;
        //update the rates if n_users has changed
        R_TOTAL = R_ADD + N_USERS * R_FOLLOW + N_USERS * R_TWEET;
        R_ADD_NORM = R_ADD / R_TOTAL;
        R_FOLLOW_NORM = R_FOLLOW * N_USERS / R_TOTAL;
        R_TWEET_NORM = R_TWEET * N_USERS / R_TOTAL;

    }

    //PRINT WHY PROGRAM STOPPED
    //#######################################################################################
    if (TIME < T_FINAL) {
        cout  << "\nReached maximum number of users.\n\n- - Program Terminated - -\n\n";
    } else {
        cout << "\nReached maximum amount of time steps.\n\n- - Program Terminated - -\n\n";
    }
    DATA_TIME.close();
    //###################################################################################################

    if (P_OUT == 1) {
        POUT(network, MAX_USERS);
    } else {
        // nothing is done here
    }

    DATA_TIME.close();
}
