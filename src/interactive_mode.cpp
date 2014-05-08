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
 * the original authors (Isaac Tamblyn, Kevin Ryczko & Adam Domurad), as well, in the case of a derivation,
 * subsequent authors. 
 */

#include "interactive_mode.h"
#include "analyzer.h"

#include <lua.hpp>
#include <stdexcept>
#include <iostream>

#include <luawrap-lib/include/luawrap/luawrap.h>

lua_State* init_lua_state(bool minimal = false);
lua_State* get_lua_state(AnalysisState& state);

/* State of the interactive mode. Holds a Lua context. */

struct InteractiveModeLuaState {
    lua_State* L = NULL;
    AnalysisState* state;

    void ensure_init(AnalysisState& s) {
        if (L == NULL) {
            L = init_lua_state();
            luawrap::dofile(L, "./.libs/interactive_mode.lua");
            luawrap::dofile(L, s.config.lua_script.c_str());
        }
        state = &s;
    }

    AnalysisState& operator*() {
        return *state;
    }
    AnalysisState* operator->() {
        return state;
    }

    /* Returns false if user has called exit() or quit()*/
    bool show_menu() {
        luawrap::globals(L)["on_interact"].push();
        return luawrap::call<bool>(L);
    }
};

static InteractiveModeLuaState state;

lua_State* get_lua_state(AnalysisState& as) {
    state.ensure_init(as);
    return state.L;
}

/*
 * Bindings for interactive mode.
 * Essentially, functions are installed once,
 * and values are copied over whenever the simulation is paused.
 *
 * The interactive mode script is expected to return after (optionally)
 * setting the next time (real, or in simulation-time) in order to analyze
 * the simulation over time.
 *
 * Using set_next_sim_time(1) consistently will allow
 * a script
 * */
struct InteractiveModeBindings {
    static void sync_state(lua_State* L) {
        using namespace luawrap;

        LuaValue G = globals(L);
        G["n_entities"] = state->network.n_entities;
        G["max_entities"] = state->network.max_entities;
        G["analysis_time"] = state->time;
        G["analysis_step"] = state->stats.n_steps;

        // Output from stats:
        auto& S = state->stats;
        G["rate_total"] = S.event_rate;
        G["rate_add"] = S.event_rate * S.prob_add;
        G["rate_follow"] = S.event_rate * S.prob_follow;
        G["rate_retweet"] = S.event_rate * S.prob_retweet;
        G["rate_tweet"] = S.event_rate * S.prob_tweet;
    }

    static void install_functions(lua_State* L) {
        using namespace luawrap;

        LuaValue G = globals(L);
        G["followers"].bind_function(followers);
        G["n_followers"].bind_function(n_followers);
        G["followings"].bind_function(followings);
        G["n_followings"].bind_function(n_followings);

        G["followers_print"].bind_function(followers_print);
        G["tweets"].bind_function(tweets);

        G["entity"].bind_function(entity);
        G["create_entity"].bind_function(create_entity);
        G["entities"].bind_function(entities);

        G["save_network_state"].bind_function(save_network_state);
        G["load_network_state"].bind_function(load_network_state);
    }

    /* Interrupt menu functions: */
    static int n_followers(int id) {
        return state->network.n_followers(id);
    }

    static void save_network_state(const char* fname) {
        analyzer_save_network_state(*state, fname);
    }

    static void load_network_state(const char* fname) {
        analyzer_load_network_state(*state, fname);
    }

    static LuaValue entity(int id) {
        return entity_to_table(state->network[id]);
    }

    static LuaValue create_entity(int id) {
        if (analyzer_create_entity(*state)) {
            return entity_to_table(state->network.back());
        }
        return LuaValue::nil(state.L);
    }

    static int n_followings(int entity) {
        return state->network.n_followings(entity);
    }

    static std::vector<int> followers(int entity) {
        return state->network.follower_set(entity).as_vector();
    }
    static std::vector<int> followings(int entity) {
        std::vector<int> ret;
        for (int id : state->network.following_set(entity)) {
            ret.push_back(id);
        }
        return ret;
    }

    // Can be used if simply dumping a direct member by name,
    // otherwise must use value["member-name"] = expression...;
#define DUMP(obj, member) \
    value[#member] = obj. member

    static LuaValue entity_to_table(Entity& e) {
        auto value = LuaValue::newtable(state.L);
        value["entity_type"] = state.state->entity_types[e.entity_type].name;
        DUMP(e, preference_class);
        DUMP(e, region_bin);
        DUMP(e, ideology_tweet_percent);
        DUMP(e, ideology_bin);
        DUMP(e, n_tweets);
        DUMP(e, n_retweets);
        DUMP(e, creation_time);
        DUMP(e, avg_chatiness);
        DUMP(e, chatty_entities);

        auto table = LuaValue::newtable(state.L);
        value["location"] = table;

        value["language_id"] = (int)e.language;
        value["language_name"] = language_name(e.language);

        return value;
    }

    static LuaValue tweet_to_table(Tweet& tweet) {
        auto value = LuaValue::newtable(state.L);
        DUMP(tweet, id_tweeter);
        DUMP(tweet, id_link);
        DUMP(tweet, retweet_next_rebin_time);
        DUMP(tweet, retweet_time_bin);
        DUMP(tweet, generation);
        DUMP(tweet, creation_time);

        value["id_original_author"] = tweet.content->id_original_author;
        value["language_id"] = (int)tweet.content->language;
        value["language_name"] = language_name(tweet.content->language);

        return value;
    }

    static LuaValue entities() {
        auto value = LuaValue::newtable(state.L);

        for (auto& entity : state->network) {
            value[value.objlen() + 1] = entity_to_table(entity);
        }

        return value;
    }

    static LuaValue tweets() {
        auto value = LuaValue::newtable(state.L);

        for (auto* node : state->tweet_bank.as_node_vector()) {
            auto table = tweet_to_table(node->data);
            table["rate_react_total"] = node->rates.tuple_sum;
            value[value.objlen() + 1] = table;
        }

        return value;
    }

    static void followers_print(int entity) {
        state->network.follower_set(entity).print();
    }
};

/** Initialization code below **/

extern "C" {
// Linenoise dependency provides history and tab-completion.
// Defined in dependencies/lua-linenoise.
int luaopen_linenoise(lua_State *L);
}

/**
 * Configure a lua-state ready to pick up whatever libraries
 * are in '.libs'. We connect to lua-repl for an interactive
 * with command-completion.
 */
lua_State* init_lua_state(bool minimal) {
    lua_State* L = lua_open();
    luaL_openlibs(L);
    // configure_lua_packages:
    LuaValue globals = luawrap::globals(L);
    LuaValue package = globals["package"];
    package["path"] = package["path"].as<std::string>() + ";./.libs/?.lua";
    package["cpath"] = package["cpath"].as<std::string>() + ";./.libs/?.so";

    if (!minimal) {
        // Linenoise loading, see above.
        package["preload"]["linenoise"].bind_function(luaopen_linenoise);

        InteractiveModeBindings::install_functions(L);
    }
    return L;
}

void sync_lua_state(AnalysisState& s) {
    InteractiveModeBindings::sync_state(get_lua_state(s));
}

bool start_interactive_mode(AnalysisState& s) {
    sync_lua_state(s);
    bool menu = false;
    try {
        menu = state.show_menu();
    } catch (std::runtime_error& err) {
        std::cout << err.what() << std::endl;
    } catch (...) {
        std::cout << "Recovering from an assert/debug statement ..." << std::endl;
    }
    return menu;
}
