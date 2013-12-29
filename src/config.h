
#ifndef CONFIG_H_
#define CONFIG_H_

#include <map>
#include <string>

#include "util.h"

/**
 * Static configuration:
 * Configurable values that must be available at compile time for efficiency reasons.
 */

// Follow set allocation tuning:
const int FOLLOW_LIST_THRESHOLD1 = 96; // Initial follow set limit, statically included in every entity object. (Short-object-optimization)
const int FOLLOWER_LIST_THRESHOLD1 = 96; // Initial follower set limit, statically included in every entity object. (Short-object-optimization)
const int FOLLOW_SET_GROWTH_MULTIPLE = 2; // Grow the follow set by double everytime it fails
const int FOLLOW_SET_MEM_PER_USER = 512 * 4 * 8; // Allocate enough space for 512 follows for each entity.

const int RETWEETS_STORED = 4;

const int TIME_CATEGORIZATION_FREQUENCY = 24*60;

// Output frequency:
const int STDOUT_OUTPUT_RATE = 1000; // Once per X file outputs

// Uncomment this to run expensive but informative checks:
//#define SLOW_DEBUG_CHECKS

/**
 * Dynamic configuration:
 * Configurable values that are loaded from the INFILE before running.
 */
struct InfileConfig {
    // Give the wordy types a local 'nickname'
    typedef std::map<std::string, std::string> StringMap;
    typedef std::map<std::string, double> NumberMap;

    InfileConfig(const StringMap& conf) : raw_conf(conf) {
        // For convenience, have all numbers preparsed as if they were numbers (errors result in 0):
        StringMap::const_iterator iterator = conf.begin();
        for (; iterator != conf.end(); ++iterator) {
            num_conf[iterator->first] = parse_num(iterator->second);
        }
    }

    /* Use [] or get() to acquire a number */
    double get(const std::string& key) {
        return num_conf[key];
    }

    /* Use [] or get() to acquire a number */
    double operator[](const std::string& key) {
        return get(key);
    }

    /* Use raw_get() to acquire a string */
    std::string raw_get(const std::string& key) {
        return raw_conf[key];
    }
private:
    StringMap raw_conf;
    NumberMap num_conf;
};

#endif
