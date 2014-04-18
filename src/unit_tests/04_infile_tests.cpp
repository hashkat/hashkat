#include <fstream>
#include <iostream>
#include <map>

#include "tests.h"

#include "../config_static.h"
#include "../config_dynamic.h"
#include "../network.h"
#include "../analyzer.h"
#include "../io.h"

using namespace std;

static bool str_replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

// Test the system in edge cases using custom INFILE's.
// UnitTest++ will catch any segfaults, and report them as test errors
typedef map<string, string> StringMap;
ParsedConfig create_config(StringMap& replacements) {
    const char* OUT_FILE = "src/tests/INFILE_temporary.yaml";
    // File with templated arguments
    fstream input("src/tests/INFILE_template.yaml", fstream::in);
    fstream output(OUT_FILE, fstream::out);

    string line;
    while (getline(input, line)) {
        StringMap::iterator it = replacements.begin();
        for (; it != replacements.end(); ++it) {
            str_replace(line, "$" + it->first, it->second);
        }
        output << line << '\n';
    }
    output.close();

    return parse_yaml_configuration(OUT_FILE);
}

SUITE(infile_tests) {
    const int TEST_SEED = 0xDEADBEEF;

    TEST(followback_and_language_check) {
//        // Specify variables in the followback_template.yaml file
//        StringMap C;
//        C["max_entities"] = "10000"; // One more than initial
//        C["max_time"] = "100";
//        C["initial_entities"] = "9999";
//        C["followback_probability"] = "1.0";
//        C["add_rate"] = "0.0"; // Don't add any users
//        C["use_followback"] = "true";
//
//        C["english_weight"] = "0";
//        C["french_weight"] = "0";
//        C["french_and_english_weight"] = "1";
//
//        ParsedConfig config = create_config(C);
//
//        AnalysisState state(config, TEST_SEED);
//        analyzer_main(state);
//        double follow_backs = state.stats.event_stats[EV_FOLLOWBACK];
//        double follows = state.stats.n_follows;
//        cout << "Followbacks: " << follow_backs << endl;
//        cout << "Follows: " << follows << endl;
//        CHECK(follow_backs == follows/2);
//        LanguageProbabilities probs; // abuse, used to store amount
//        for (int i = 0; i < state.network.n_entities; i++) {
//            int lang = state.network[i].language;
//            probs[lang]++;
//        }
//        for (int i = 0; i < N_LANGS; i++) {
//            printf("%d: %d\n", i, (int)probs[i]);
//        }
//        CHECK(probs[LANG_FRENCH_AND_ENGLISH] == 9999); // Should all be either
    }
}
