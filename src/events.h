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

#ifndef EVENT_H_
#define EVENT_H_

#include "dependencies/google/sparse_hash_set"
#include "util.h"
#include "serialization.h"

// 0 - add, 1 - follow, 2 - tweet
const int number_of_diff_events = 2;

struct Tweet;

struct Rate_Function {
    std::string function_type;
    double slope, y_intercept, const_val, amplitude, exp_factor;
    std::vector<double> monthly_rates;
    Rate_Function() {
        slope = y_intercept = const_val = amplitude = exp_factor = -1;
        function_type = "not specified";
    }

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(function_type, slope, y_intercept, const_val, amplitude, exp_factor, monthly_rates);
    }
};

struct Add_Rates {
    Rate_Function RF;
};

struct AnalysisState; // Forward declare

// Defined in events.cpp, handled by specified lua_script, eg INTERACT.lua.

void lua_hook_follow(AnalysisState& state, int id_follower, int id_followed);
void lua_hook_add(AnalysisState& state, int id_follower);
void lua_hook_unfollow(AnalysisState& state, int id_follower, int id_followed);
void lua_hook_tweet(AnalysisState& state, int id_follower, Tweet& tweet);
void lua_hook_retweet(AnalysisState& state, int id_follower, Tweet& tweet);
void lua_hook_exit(AnalysisState& state);
void lua_hook_new_network(AnalysisState& state);
void lua_hook_step_analysis(AnalysisState& state);
void lua_hook_load_network(AnalysisState& state);
void lua_hook_save_network(AnalysisState& state);

#endif
