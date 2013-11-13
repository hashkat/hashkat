#ifndef __UTIL_H_
#define __UTIL_H_

// Mixed bag of useful functions

#include <cassert>
#include <cstdio>
#include <cstdlib>

// Dirt simple error handling, exits the program:
static inline void panic(const char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(2); // Return with error code
}

// Checks that are turned off in release mode:
#define DEBUG_CHECK(expr, msg) \
    if (!(expr)) { \
        printf("%s:%d %s\n", __FILE__, __LINE__, msg); \
        exit(2); \
    }

#endif
