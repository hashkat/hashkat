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

enum FollowModel {
    RANDOM_FOLLOW,
    TWITTER_PREFERENTIAL_FOLLOW,
    AGENT_FOLLOW,
    PREFERENTIAL_AGENT_FOLLOW,
    HASHTAG_FOLLOW,
    FOLLOW_BACK_FOLLOW,
    RETWEETING_FOLLOW,
    TWITTER_FOLLOW, // Amalgamation of first 5
    N_FOLLOW_MODELS = TWITTER_FOLLOW, // Does not count TWITTER_FOLLOW itself
    N_TWITTER_FOLLOW_MODELS = HASHTAG_FOLLOW + 1 // Follows within TWITTER_FOLLOW
};

enum TweetType {
    TWEET_STANDARD,
    TWEET_IDEOLOGICAL,
    // Note: Treated the same as standard retweeting.
    TWEET_MUSICAL,
    TWEET_HUMOUROUS,
    // Note: Used only for disambiguating matching and mismatching ideology, for retweeting.
    TWEET_IDEOLOGICAL_DIFFERENT,
    N_TWEET_TYPES
};

enum Language {
    // Resolves using X above, fills enum:
    LANG_ENGLISH,
    LANG_FRENCH_AND_ENGLISH,
    LANG_FRENCH,
    LANG_SPANISH,
    /* All languages before LANG_FRENCH contain English */
    N_LANGS // Not a real entry; evaluates to amount of languages
};

inline bool language_understandable(Language known, Language seen) {
    if (known == seen) {
        return true;
    }
    // Otherwise, we can only understand this language if we know
    // English and French. Since English&French are the only considered
    // languages, we must understand the language in this case.
    return (known == LANG_FRENCH_AND_ENGLISH) && (seen != LANG_SPANISH);
}

inline const char* tweet_type_name(int bin) {
    if (bin == TWEET_STANDARD) return "TWEET_STANDARD";
    if (bin == TWEET_IDEOLOGICAL) return "TWEET_IDEOLOGICAL";
    if (bin == TWEET_MUSICAL) return "TWEET_MUSICAL";
    if (bin == TWEET_HUMOUROUS) return "TWEET_HUMOUROUS";
    if (bin == TWEET_IDEOLOGICAL_DIFFERENT) return "TWEET_IDEOLOGICAL_DIFFERENT";
    return NULL;
}

inline const char* language_name(int bin) {
    if (bin == LANG_ENGLISH) return "LANG_ENGLISH";
    if (bin == LANG_FRENCH_AND_ENGLISH) return "LANG_FRENCH_AND_ENGLISH";
    if (bin == LANG_FRENCH) return "LANG_FRENCH";
    if (bin == LANG_SPANISH) return "LANG_SPANISH";
    return NULL;
}

const int APPROX_MONTH = 30 * 24 * 60;
const int TIME_CAT_FREQ = 30 * 24 * 60;

// Output frequency:
const int STDOUT_OUTPUT_RATE = 100; // Once per X file outputs
const double MINIMUM_TIME_STEP = 1; // We cannot take a time-step more than one second.

/*
 * Parameters for tweet relevance functions
 */

const int N_BIN_PREFERENCE_CLASS = 6;
const int N_BIN_AGENT_TYPES = 200;
const int N_BIN_REGIONS = 3;
const int N_BIN_IDEOLOGIES = 4;


#endif
