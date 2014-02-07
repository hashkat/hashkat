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

SUITE(SanityChecks) {
    const int TEST_SEED = 0xDEADBEEF;

    TEST(SanityCheck) {
        // Specify variables in the followback_template.yaml file
        StringMap C;
        C["max_entities"] = "10000"; // One more than initial
        C["max_time"] = "100";
        C["initial_entities"] = "9999";
        C["followback_probability"] = "1.0";
        C["add_rate"] = "0.0"; // Don't add any users
        C["use_flawed_followback"] = "true";

        ParsedConfig config = create_config(C);

        AnalysisState state(config, TEST_SEED);
        analyzer_main(state);
        double follow_backs = state.stats.event_stats[EV_FOLLOWBACK];
        double follows = state.stats.n_follows;
        cout << "Followbacks: " << follow_backs << endl;
        cout << "Follows: " << follows << endl;
        CHECK(follow_backs == follows/2);
    }
}
