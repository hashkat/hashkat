
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

enum Language {
    // Resolves using X above, fills enum:
    LANG_ENGLISH,
    LANG_FRENCH_AND_ENGLISH,
    LANG_FRENCH,
    /* All languages before LANG_FRENCH contain English */
    N_LANGS // Not a real entry; evaluates to amount of languages
};
#undef X

inline bool language_understandable(Language known, Language seen) {
    if (known == seen) {
        return true;
    }
    // Otherwise, we can only understand this language if we know
    // English and French. Since English&French are the only considered
    // languages, we must understand the language in this case.
    return (known == LANG_FRENCH_AND_ENGLISH);
}

inline const char* language_name(int bin) {
    if (bin == LANG_ENGLISH) return "LANG_ENGLISH";
    if (bin == LANG_FRENCH_AND_ENGLISH) return "LANG_FRENCH_AND_ENGLISH";
    if (bin == LANG_FRENCH) return "LANG_FRENCH";
    return NULL;
}

const int TIME_CAT_FREQ = 30 * 24 * 60;

// Output frequency:
const int STDOUT_OUTPUT_RATE = 1000; // Once per X file outputs

// Uncomment this to run (somewhat) expensive but informative checks:
#define SLOW_DEBUG_CHECKS

/*
 * Parameters for tweet relevance functions
 */

const int N_BIN_HUMOUR = 2; // Should be exactly as in INFILE.yaml!

// NOTE WELL: The below limits should be respected by your INFILE.yaml!!
const int N_BIN_PREFERENCE_CLASS = 2;
const int N_BIN_ENTITY_TYPES = 5;
const int N_BIN_REGIONS = 2;
const int N_BIN_IDEOLOGIES = 2;

const int N_FOLLOW_MODELS = 5;
#endif
