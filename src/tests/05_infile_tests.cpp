#include "tests.h"

#include "../config_static.h"
#include "../config_dynamic.h"
#include "../network.h"
#include "../analyzer.h"
#include "../io.h"

using namespace std;

// Test the system in edge cases using custom INFILE's.
// UnitTest++ will catch any segfaults, and report them as test errors

SUITE(SanityChecks) {
    const int TEST_SEED = 0xDEADBEEF;

    TEST(simple_config) {
        ParsedConfig config = parse_yaml_configuration("src/tests/infile_template.yaml");

        AnalysisState state(config, TEST_SEED);
        analyzer_main(state);
    }
}
