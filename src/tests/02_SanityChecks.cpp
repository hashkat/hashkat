#include <map>
#include <string>
#include <dependencies/UnitTest++.h>

#include "../analyze.h"

using namespace std;

// Test the system in edge cases
// UnitTest++ will catch any segfaults, and report them as test errors

// Specify tests in terms of differences from these default values
// Note: Relies on unspecified values evaluating to 0
static map<string, string> make_test_defaults() {
    map<string, string> config;
   config["MAX_USERS"] = "1000";
   config["RANDOM_INCR"] = "1";

   config["T_FINAL"] = "525600.0";

   config["R_ADD"] = "4.7";
   config["SILENT"] = "1"; // Avoid verbosity in test

   config["R_FOLLOW"] = "0.139";
   config["R_TWEET"] = "0.0139";

   config["R_ADD_NORMAL"] = "100";
   config["R_ADD_CELEB"] = "90";
   config["R_ADD_BOT"] = "40";
   config["R_ADD_ORG"] = "10";

   config["R_FOLLOW_NORMAL"] = "100";
   config["R_FOLLOW_CELEB"] = "900";
   config["R_FOLLOW_BOT"] = "0";
   config["R_FOLLOW_ORG"] = "300";

   config["TWEET_THRESHOLDS"] = "1 10 20 2000 3000";
   config["FOLLOW_THRESHOLDS"] = "1 10 20 2000 3000";
   config["FOLLOW_THRESHOLDS_PROBABILITIES"] = "0.1 0.3 0.5 0.7 0.9";


   config["N_USERS"] = "10";
   return config;
}

SUITE(SanityChecks) {
    const int TEST_SEED = 0xDEADBEEF;

    TEST(simple_config) {
        map<string, string> CONFIG = make_test_defaults();
        analyze_network(CONFIG, TEST_SEED);
    }
}
