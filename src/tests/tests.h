/*
 * tests.h:
 *  Main header for unit tests. Provides convenience methods.
 */

#ifndef TESTS_H_
#define TESTS_H_

#include <dependencies/UnitTest++.h>
#include <map>
#include <string>

// Specify tests in terms of differences from these default values
// Note: Relies on unspecified values evaluating to 0
inline std::map<std::string, std::string> make_test_defaults() {
   std::map<std::string, std::string> config;
   config["MAX_ENTITIES"] = "1000";
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

   config["N_ENTITIES"] = "10";
   return config;
}

#endif
