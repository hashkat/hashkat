#include "interrupt_menu.h"
#include "analyzer.h"

#include <lua.hpp>
#include <stdexcept>
#include <iostream>
#include <luawrap-lib/include/luawrap/luawrap.h>

static lua_State* init_lua_state();

struct InterruptMenuState {
    lua_State* L = NULL;
    LuaValue menu_object;
    AnalysisState* state;

    void ensure_init(AnalysisState& s) {
        if (L == NULL) {
            L = init_lua_state();
            menu_object = luawrap::dofile(L, "./.libs/interrupt_menu.lua");
        }
        state = &s;
    }

    AnalysisState& operator*() {
        return *state;
    }
    AnalysisState* operator->() {
        return state;
    }

    void sync_state() {
        LuaValue G = luawrap::globals(L);
        G["n_entities"] = state->network.n_entities;
        G["max_entities"] = state->network.max_entities;
        G["time"] = state->time;

        // Output from stats:
        auto& S = state->stats;
        G["rate_total"] = S.event_rate;
        G["rate_add"] = S.event_rate * S.prob_add;
        G["rate_follow"] = S.event_rate * S.prob_follow;
        G["rate_retweet"] = S.event_rate * S.prob_retweet;
        G["rate_tweet"] = S.event_rate * S.prob_tweet;
    }

    /* Returns false if user has called exit() or quit()*/
    bool show_menu() {
        menu_object["show_menu"].push();
        return luawrap::call<bool>(L);
    }
};

static InterruptMenuState state;

struct InterruptMenuFunctions {
    static void install_functions(lua_State* L) {
        LuaValue G = luawrap::globals(L);
        G["followers"].bind_function(followers);
        G["followings"].bind_function(followings);
        G["followers_print"].bind_function(followers_print);
        G["tweets"].bind_function(tweets);
    }
    /* Interrupt menu functions: */
    static std::vector<int> followers(int entity) {
        std::vector<int> ret;
        for (int id : state->network.follower_set(entity)) {
            ret.push_back(id);
        }
        return ret;
    }
    static std::vector<int> followings(int entity) {
        std::vector<int> ret;
        for (int id : state->network.following_set(entity)) {
            ret.push_back(id);
        }
        return ret;
    }

    static LuaValue tweet_to_table(Tweet& tweet) {
        LuaValue value = LuaValue::newtable(state.L);
        value["id_tweeter"] = tweet.id_tweeter;
        value["id_link"] = tweet.id_link;
        value["generation"] = tweet.generation;
        value["creation_time"] = tweet.creation_time;
        value["id_original_author"] = tweet.content->id_original_author;
        value["language_id"] = (int)tweet.content->language;
        value["language_name"] = language_name(tweet.content->language);

        return value;
    }
    static LuaValue tweets() {
        LuaValue value = LuaValue::newtable(state.L);

        for (Tweet tweet : state->tweet_bank.as_vector()) {
            value[value.objlen() + 1] = tweet_to_table(tweet);
        }

        return value;
    }
    static void followers_print(int entity) {
        FollowerSet::Context context(*state, entity);
        state->network.follower_set(entity).print(context);
    }
};


extern "C" {
// Defined in dependencies/lua-linenoise:
int luaopen_linenoise(lua_State *L);
}


/**
 * Configure a lua-state ready to pick up whatever libraries
 * are in '.libs'. We connect to lua-repl for an interactive
 * with command-completion.
 */
static lua_State* init_lua_state() {
    lua_State* L = lua_open();
    luaL_openlibs(L);
    // configure_lua_packages:
    LuaValue globals = luawrap::globals(L);
    LuaValue package = globals["package"];
    package["path"] = package["path"].as<std::string>() + ";./.libs/?.lua";
    package["cpath"] = package["cpath"].as<std::string>() + ";./.libs/?.so";
    package["preload"]["linenoise"].bind_function(luaopen_linenoise);
    InterruptMenuFunctions::install_functions(L);
    return L;
}

bool show_interrupt_menu(AnalysisState& s) {
    state.ensure_init(s);
    state.sync_state();
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
