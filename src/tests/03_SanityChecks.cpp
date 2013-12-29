#include "tests.h"

#include "../config.h"
#include "../network.h"
#include "../analyze.h"

using namespace std;

// Test the system in edge cases
// UnitTest++ will catch any segfaults, and report them as test errors

SUITE(SanityChecks) {
    const int TEST_SEED = 0xDEADBEEF;

    TEST(simple_config) {
        AnalysisState state(make_test_defaults());
        simulate_network(state, TEST_SEED);
    }
}
