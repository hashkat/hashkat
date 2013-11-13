#ifndef __UTIL_H_
#define __UTIL_H_

// Mixed bag of useful functions

#include <cstdio>
#include <cstdlib>

// Dirt simple error handling, exits the program:
static inline void panic(const char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(2); // Return with error code
}

#endif
