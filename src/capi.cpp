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

#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>

#include "dependencies/ini.h"
#include "dependencies/UnitTest++.h"
#include "dependencies/lcommon.h"

#include "dependencies/lcommon/typename.h"
#include "dependencies/lcommon/perf_timer.h"

#include "config_static.h"
#include "config_dynamic.h"

#include "network.h"
#include "analyzer.h"
#include "io.h"

using namespace std;

// From main.cpp:
bool has_flag(int argc, char** argv, std::string test);
const char* get_var_arg(int argc, char** argv, std::string test, const char* default_val);

extern "C" {
#include "capi.h"
AnalysisState* hashkat_new_analysis_state(int argc, char** argv) {
#ifndef __sun
    std::locale loc("");
    std::cout.imbue(loc);
#endif
    if (has_flag(argc, argv, "--stdout-nobuffer")) {
        // Important mainly for colorization tool
        setvbuf(stdout,NULL,_IONBF, BUFSIZ); // Make stdout unbuffered
    } else {
        setvbuf(stdout,NULL,_IOLBF, BUFSIZ); // Make stdout line-buffered
    }

    cout.setf(std::ios::unitbuf);
    // NOTE: We rely on hashkat_pre.py to create a -generated version of our input file!
    for (int i = 0; i < argc; i++) {
        char* arg = argv[i]; 
//        printf("Argument %d is '%s'.\n", i+1, arg);
    }
    std::string INFILE = get_var_arg(argc, argv, "--input", "INFILE.yaml");
    INFILE += "-generated";
    printf("Loading input configuration from '%s'.\n", INFILE.c_str());
    ParsedConfig config = parse_yaml_configuration(INFILE.c_str());
    if (has_flag(argc, argv, "--handle-ctrlc")) {
        config.handle_ctrlc = true;
    }
    // or running analysis:
    Timer t;
    int seed = std::stoi(get_var_arg(argc, argv, "--seed", "1"));
    if (has_flag(argc, argv, "--rand")) {
            time_t t;
            time(&t);
            seed = (int)t;
    }

    printf("Starting simulation with seed '%d'.\n", seed);
    return new AnalysisState(config, seed);
}

void hashkat_destroy_analysis_state(AnalysisState* state) {
    output_network_statistics(*state);
    delete state;
}

void hashkat_install_event_callbacks(AnalysisState* state, EventCallbacks* callbacks) {
    state->event_callbacks = *callbacks;
}

void hashkat_start_analysis_loop(AnalysisState* state) { 
    analyzer_main(*state);
}
void hashkat_finish_analysis(AnalysisState* state) { 
    state->stats.user_did_exit = true;
}

static const char* copy_str(const char* str, size_t n) {
    char* new_str = new char[n + 1];
    memcpy(new_str, str, n + 1);
    return new_str;
}

const char* hashkat_dump_stats(AnalysisState* state_ptr) { 
    AnalysisState& state = *state_ptr;
    stringstream output;
    JsonWriter writer {state, output};
    double time = state.time;
    int n_agents = state.network.size();
    state.stats.serialize(writer);
    writer(NVP(time), NVP(n_agents));
    string output_str = output.str();
    return copy_str(output_str.c_str(), output_str.size());
}

const char* hashkat_dump_state(AnalysisState* state_ptr) { 
    AnalysisState& state = *state_ptr;
    stringstream output;
    JsonWriter writer {state, output};
    state.serialize(writer);
    string output_str = output.str();
    return copy_str(output_str.c_str(), output_str.size());
}

void hashkat_dump_free(struct AnalysisState* state, const char* dump) {
    delete[] dump;
}
}

struct TweetApiProxy {
    Tweet tweet;
    template <typename Archive>
    void serialize(Archive& ar) {
        tweet.api_serialize(ar);
    }
};
 
struct AgentApiProxy {
    Agent* agent; 
    bool dump_follow_sets;
    template <typename Archive>
    void serialize(Archive& ar) {
        agent->api_serialize(ar, dump_follow_sets);
    }
};

extern "C" {
const char* hashkat_dump_summary(AnalysisState* state) {
    stringstream output;
    JsonWriter writer {*state, output};
    auto n_agents = state->network.size();
    vector<vector<int>> follower_sets;
    vector<vector<int>> following_sets;
    for (Agent& agent : state->network) {
        follower_sets.push_back(agent.follower_set.as_vector());
        following_sets.push_back(agent.following_set.as_vector());
    }

    vector<TweetApiProxy> live_tweets;
    for (auto& t : state->tweet_bank.as_vector()) {
        live_tweets.push_back({t});
    }
    writer(NVP(live_tweets), NVP(n_agents), 
        NVP(follower_sets), NVP(following_sets));
    string output_str = output.str();
    return copy_str(output_str.c_str(), output_str.size());
}

const char* hashkat_dump_tweet(AnalysisState* state, Tweet* tweet) {
    stringstream output;
    JsonWriter writer {*state, output};
    tweet->api_serialize(writer);
    string output_str = output.str();
    return copy_str(output_str.c_str(), output_str.size());
}

const char* hashkat_dump_agents(AnalysisState* state, int dump_follow_sets) {
    stringstream output;
    JsonWriter writer {*state, output};
    auto n_agents = state->network.size();
    writer( cereal::make_size_tag(n_agents));
    for (Agent& agent : state->network) {
        AgentApiProxy proxy {&agent, dump_follow_sets != 0};
        writer(proxy);
    }
    string output_str = output.str();
    return copy_str(output_str.c_str(), output_str.size());
}
}

