/* Configurable values that must be available at compile time for efficiency reasons. */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

const int FOLLOW_LIMIT1 = 96; // Initial follow set limit, statically included in every person object. (Short-object-optimization)
const int FOLLOW_SET_GROWTH_MULTIPLE = 2; // Grow the follow set by double everytime it fails
const int FOLLOW_SET_MEM_PER_USER = 512 * 4; // Allocate enough space for 512 follows for each user.

#endif
