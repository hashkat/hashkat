#ifndef __UTIL_H_
#define __UTIL_H_

#include <cmath>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <limits>

// Mixed bag of useful functions

// Dirt simple error handling, exits the program:
inline void error_exit(const char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(2); // Return with error code
}

//** AD: This is what I was talking about with giving the compiler hints about the path the a conditional will take.
// Usage: if (likely(rarely failing checks) ) { ... } else { .. error routine ... }
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

// Checks that are always on:
#define ASSERT(expr, msg) \
    if (LIKELY(!(expr))) { \
        printf("FAILED CHECK: %s (%s:%d)\n", msg, __FILE__, __LINE__); \
        throw msg; \
    }

// Checks that are turned off in release mode:
#define DEBUG_CHECK ASSERT

// XXX: NB: Not true for all compilers.
typedef int int32;
typedef long long int64;

template <typename T>
inline bool within_range(T val, T a, T b) {
	// Is val within [a,b) ?
	return val >= a && val < b;
}

inline std::vector<double> parse_numlist(std::string s) {
	std::vector<double> ret;
	std::stringstream sstream(s);
	while (!sstream.eof()) {
		double val;
		sstream >> val;
		ret.push_back(val);
	}
	return ret;
}

inline double parse_num(std::string s) {
    double ret = 0.0;
    std::stringstream string_converter(s); //** Think of this like 'cin', but instead of standard input its an arbitrary string.
    string_converter >> ret; //** Will adapt to the appropriate type! also like 'cin' would.
    return ret;
}

#define LOG(msg, ...) \
    printf(msg, ...);

#endif
