
#ifndef CONFIG_STATIC_H_
#define CONFIG_STATIC_H_

#include <vector>
#include <string>
#include <limits>

#include "CategoryGrouper.h"
#include "util.h"

/**
 * Static configuration:
 * Configurable values that must be available at compile time for efficiency reasons.
 */

// Follow set allocation tuning:
const int FOLLOW_LIST_THRESHOLD1 = 256; // Initial follow-set size-limit before data-structure is switched to google's sparsehash
const int FOLLOWER_LIST_THRESHOLD1 = 256; // Initial follower-set size-limit before data-structure is switched to google's sparsehash
// TODO Somewhat arbitrarily chosen

const int TIME_CAT_FREQ = 30 * 24 * 60;

// Output frequency:
const int STDOUT_OUTPUT_RATE = 1000; // Once per X file outputs

// Uncomment this to run (somewhat) expensive but informative checks:
#define SLOW_DEBUG_CHECKS

/*
 * Parameters for tweet relevance functions
 */

// N_BIN_PREFERENCE_CLASS should ideally be equal to
// the amount of preference classes actually used in the simulation.
// The memory waste should not be horrible to overshoot by, say, 2x, however.
const int N_BIN_PREFERENCE_CLASS = 10;

// NOTE WELL: The below 2 parameters should match your INFILE.yaml!!
const int N_BIN_HUMOUR = 2;
const int N_BIN_DISTANCE = 4;

const int N_BIN_REGIONS = 2;
const int N_BIN_SUBREGIONS = 2;
const int N_IDEOLOGIES = 2;

// Account for 'no ideology' being represented by 0.
const int N_BIN_IDEOLOGIES = N_IDEOLOGIES + 1;

#endif
