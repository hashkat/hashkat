#include <fstream>

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

ParsedConfig create_config(const char* followback_probability) {
    const char* OUT_FILE = "src/tests/followback_temp.yaml";
    // File with templated arguments
    fstream input(fstream::in, "src/tests/followback_template.yaml");
    fstream output(fstream::out, OUT_FILE);

    std::string line;
    while (getline(input, line)) {
        str_replace(line, "$followback_probability", followback_probability);
        output << line << '\n';
    }
    output.close();

    return parse_yaml_configuration(OUT_FILE);
}

SUITE(SanityChecks) {
    const int TEST_SEED = 0xDEADBEEF;

    TEST(simple_config) {
        ParsedConfig config = parse_yaml_configuration("src/tests/infile_template.yaml");

        AnalysisState state(config, TEST_SEED);
        analyzer_main(state);
    }
}
