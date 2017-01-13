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

#ifndef CAPI_H_
#define CAPI_H_

/* Should only be included with 'extern "C"' around it. 
 * This file is consumed by bindings.py (which doesn't understand 'extern "C"'). */

struct AnalysisState;
struct Tweet;

struct EventCallbacks {
    void (*on_add)(int id);
    void (*on_follow)(int id_follower, int id_followed);
    void (*on_unfollow)(int id_follower, int id_followed);
    void (*on_tweet)(const char* tweet);
    void (*on_retweet)(const char* tweet);
    void (*on_exit)(void);
    void (*on_step_analysis)(void);
    void (*on_load_network)(void);
    void (*on_save_network)(void);
};

struct AnalysisState* hashkat_new_analysis_state(int argc, char** argv);
void hashkat_destroy_analysis_state(struct AnalysisState* state);
void hashkat_install_event_callbacks(struct AnalysisState* state, struct EventCallbacks* callbacks);
void hashkat_start_analysis_loop(struct AnalysisState* state);
void hashkat_finish_analysis(struct AnalysisState* state);
// Must be deleted with hashkat_dump_free
const char* hashkat_dump_state(struct AnalysisState* state);
const char* hashkat_dump_stats(struct AnalysisState* state);
const char* hashkat_dump_tweet(struct AnalysisState* state, struct Tweet* tweet);
const char* hashkat_dump_agents(struct AnalysisState* state, int dump_follow_sets);
const char* hashkat_dump_summary(struct AnalysisState* state);
void hashkat_dump_free(struct AnalysisState* state, const char* dump);

// Test bindings:
#endif /* CAPI_H_ */
