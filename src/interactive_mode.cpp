#include "interactive_mode.h"
#include "analyzer.h"

#include <lua.hpp>
#include <stdexcept>
#include <iostream>

#include <luawrap-lib/include/luawrap/luawrap.h>
#include <luawrap-lib/include/luawrap/macros.h>

static lua_State* init_lua_state();

struct InterruptMenuState {
    lua_State* L = NULL;
    LuaValue menu_object;
    AnalysisState* state;

    void ensure_init(AnalysisState& s) {
        if (L == NULL) {
            L = init_lua_state();
            menu_object = luawrap::dofile(L, "./.libs/interactive_mode.lua");
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
        using namespace luawrap;

        LuaValue G = globals(L);
        G["followers"].bind_function(followers);
        G["followings"].bind_function(followings);

        // Inline function definition, using above 'state' global.
        // Dereferencing this will bring you to AnalysisState.
        LUAWRAP_FUNCTION(G, n_followers,
                int id = get<int>(state.L, 1);
                push(state.L, state->network.n_followers(id))
        );

        LUAWRAP_FUNCTION(G, entity,
                int id = get<int>(state.L, 1);
                push(state.L, entity_to_table(state->network[id]))
        );

        LUAWRAP_FUNCTION(G, n_followings, state->network.n_followings(get<int>(state.L, 1)));
        LUAWRAP_FUNCTION(G, create_entity,
                if (analyzer_create_entity(*state)) {
                    luawrap::push(state.L, entity_to_table(state->network.back()));
                } else {
                    lua_pushnil(state.L);
                }
        );
        G["followers_print"].bind_function(followers_print);
        G["tweets"].bind_function(tweets);
        G["entities"].bind_function(entities);
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

#define DUMP(obj, member) \
    value[#member] = obj. member

    static LuaValue entity_to_table(Entity& e) {
        auto value = LuaValue::newtable(state.L);
        DUMP(e, entity_type);
        DUMP(e, preference_class);
        DUMP(e, region_bin);
        DUMP(e, subregion_bin);
        DUMP(e, ideology_tweet_percent);
        DUMP(e, ideology_bin);
        DUMP(e, n_tweets);
        DUMP(e, n_retweets);
        DUMP(e, creation_time);
        DUMP(e, avg_chatiness);
        DUMP(e, uses_hashtags);
        DUMP(e, humour_bin);
        DUMP(e, chatty_entities);

        auto table = LuaValue::newtable(state.L);
        table["x"] = e.location.x;
        table["y"] = e.location.x;
        value["location"] = table;

        value["language_id"] = (int)e.language;
        value["language_name"] = language_name(e.language);

        return value;
    }

    static LuaValue tweet_to_table(Tweet& tweet) {
        auto value = LuaValue::newtable(state.L);
        value["id_tweeter"] = tweet.id_tweeter;
        value["id_link"] = tweet.id_link;
        value["retweet_next_rebin_time"] = tweet.retweet_next_rebin_time;
        value["retweet_time_bin"] = tweet.retweet_time_bin;
        value["generation"] = tweet.generation;
        value["creation_time"] = tweet.creation_time;
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
        FollowerSet::Context context(*state, entity);
        state->network.follower_set(entity).print(context);
    }
};

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
static lua_State* init_lua_state() {
    lua_State* L = lua_open();
    luaL_openlibs(L);
    // configure_lua_packages:
    LuaValue globals = luawrap::globals(L);
    LuaValue package = globals["package"];
    package["path"] = package["path"].as<std::string>() + ";./.libs/?.lua";
    package["cpath"] = package["cpath"].as<std::string>() + ";./.libs/?.so";

    // Linenoise loading, see above.
    package["preload"]["linenoise"].bind_function(luaopen_linenoise);

    InterruptMenuFunctions::install_functions(L);
    return L;
}

bool show_interactive_menu(AnalysisState& s) {
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
