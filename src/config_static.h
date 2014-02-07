
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
const int FOLLOW_LIST_THRESHOLD1 = 96; // Initial follow set limit, statically included in every entity object. (Short-object-optimization)
// '96' rationale: Chosen because 95% of Twitter users have <100 follows.
const int FOLLOWER_LIST_THRESHOLD1 = 96; // Initial follower set limit, statically included in every entity object. (Short-object-optimization)
// '96' rationale: Somewhat arbitrarily chosen to be same as follow.
const int FOLLOW_SET_MEM_PER_USER = 512 * 4 * 8;
// Allocate enough space for 512 follows for each entity. Increase for a more dense follow network.

const int TIME_CAT_FREQ = 30 * 24 * 60;

// Output frequency:
const int STDOUT_OUTPUT_RATE = 1000; // Once per X file outputs

// Uncomment this to run expensive but informative checks:
#define SLOW_DEBUG_CHECKS

#endif
