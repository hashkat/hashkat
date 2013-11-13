#ifndef __ANALYZE_H_
#define __ANALYZE_H_

#include <string>
#include "dependencies/mtwist.h"
#include "state.h"

/* The Analyze class encapsulates the many-parameter analyze function, and it's state. */
class Analyzer {
public:
    /* Reads configuration from the given input file */
    void init_from_file();
    /* Run the main analysis routine using this config. */
    void run();

    Analyzer() :
            random_gen_state(1) { // Just seed with 1 -- for now
        // Note: call init_from_file before use.
    }

private:
    // The network state
    Network network;

    /* Mersenne-twister random number generator */
    MTwist random_gen_state;

//    TODO: Move to mersenne twister
//    /* Using Mersenne-twister, grab an integer from 0 to max, non-inclusive (ie appropriate for array lengths). */
//    int randInt(int max) {
//        return random_gen_state.genrand_int32() % max;
//    }

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
};

#endif
