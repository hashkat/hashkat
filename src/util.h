/*
 * This file is part of the #KAT Social Network Simulator.
 *
 * The #KAT Social Network Simulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The #KAT Social Network Simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the #KAT Social Network Simulator.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Addendum:
 *
 * Under this license, derivations of the #KAT Social Network Simulator typically must be provided in source
 * form. The #KAT Social Network Simulator and derivations thereof may be relicensed by decision of 
 * the original authors (Kevin Ryczko & Adam Domurad, Isaac Tamblyn), as well, in the case of a derivation,
 * subsequent authors. 
 */

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
inline static void error_exit(const std::string& msg) {
    fprintf(stderr, "%s\n", msg.c_str());
    abort();
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
#ifndef NDEBUG
#define DEBUG_CHECK ASSERT
#else
#define DEBUG_CHECK(expr, msg)
#endif

// XXX: NB: Not true for all compilers.
typedef int int32;
typedef long long int64;
typedef int32 ref_t;

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

inline bool file_exists(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

inline double parse_num(std::string s) {
    double ret = 0.0;
    std::stringstream string_converter(s); //** Think of this like 'cin', but instead of standard input its an arbitrary string.
    string_converter >> ret; //** Will adapt to the appropriate type! also like 'cin' would.
    return ret;
}

const double ZEROTOL = 1e-16; // enough precision for really really low add rate

#define LOG(msg, ...) \
    printf(msg, ...);

#endif
