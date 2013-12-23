#include "tests.h"

#include "../analyze.h"

using namespace std;

// Test the system in edge cases
// UnitTest++ will catch any segfaults, and report them as test errors

SUITE(SanityChecks) {
    const int TEST_SEED = 0xDEADBEEF;

    TEST(simple_config) {
        map<string, string> CONFIG = make_test_defaults();
        simulate_network(CONFIG, TEST_SEED);
    }
}
