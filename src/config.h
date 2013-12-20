/* Configurable values that must be available at compile time for efficiency reasons. */

#ifndef CONFIG_H_
#define CONFIG_H_

// Follow set allocation tuning:
const int FOLLOW_LIST_THRESHOLD1 = 96; // Initial follow set limit, statically included in every person object. (Short-object-optimization)
const int FOLLOWER_LIST_THRESHOLD1 = 96; // Initial follower set limit, statically included in every person object. (Short-object-optimization)
const int FOLLOW_SET_GROWTH_MULTIPLE = 2; // Grow the follow set by double everytime it fails
const int FOLLOW_SET_MEM_PER_USER = 512 * 4 * 8; // Allocate enough space for 512 follows for each user.

const int RETWEETS_STORED = 4;

// Output frequency:
const int STDOUT_OUTPUT_RATE = 1000; // Once per X file outputs

// Uncomment this to run expensive but informative checks:
//#define SLOW_DEBUG_CHECKS

#endif
