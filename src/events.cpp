#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include "util.h"

#include "analyzer.h"
#include "capi.h"
#include "tweets.h"

#include <luawrap-lib/include/luawrap/luawrap.h>

// Defined in interactive_mode.cpp:
lua_State* get_lua_state(AnalysisState& state);
void sync_lua_state(AnalysisState& state);

static void lua_hook(AnalysisState& state, const char* type, int id1 = -1, int id2 = -1) {
    lua_State* L = get_lua_state(state);
    sync_lua_state(state);

    lua_getglobal(L, type);
    lua_pushinteger(L, id1);
    lua_pushinteger(L, id2);
    lua_call(L, 2, 0);
}

// Lua hook implementations. Note that they all follow the same simple structure.

void lua_hook_follow(AnalysisState& state, int id_follower, int id_followed) {
    if (state.event_callbacks.on_follow) {
        state.event_callbacks.on_follow(id_follower, id_followed);
    }
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_follow", id_follower, id_followed);
    }
}

void lua_hook_add(AnalysisState& state, int id) {
    if (state.event_callbacks.on_add) {
        state.event_callbacks.on_add(id);
    }
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_add", id);
    }
}

void lua_hook_unfollow(AnalysisState& state, int id_follower, int id_followed) {
    if (state.event_callbacks.on_unfollow) {
        state.event_callbacks.on_unfollow(id_follower, id_followed);
    }
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_unfollow", id_follower, id_followed);
    }
}

void lua_hook_tweet(AnalysisState& state, int id_tweeter, Tweet& tweet) {
    if (state.event_callbacks.on_tweet) {
        state.event_callbacks.on_tweet(hashkat_dump_tweet(&state, &tweet));
    }
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_tweet", id_tweeter, tweet.id_tweet);
    }
}

void lua_hook_retweet(AnalysisState& state, int id_tweeter, Tweet& tweet) {
    if (state.event_callbacks.on_retweet) {
        state.event_callbacks.on_retweet(hashkat_dump_tweet(&state, &tweet));
    }
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_retweet", id_tweeter, tweet.id_tweet);
    }
}

void lua_hook_exit(AnalysisState& state) {
    if (state.event_callbacks.on_exit) {
        state.event_callbacks.on_exit();
    }
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_exit");
    }
}

void lua_hook_new_network(AnalysisState& state) {
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_new_network");
    }
}

void lua_hook_load_network(AnalysisState& state) {
    if (state.event_callbacks.on_load_network) {
        state.event_callbacks.on_load_network();
    }
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_load_network");
    }
}

void lua_hook_step_analysis(AnalysisState& state) {
    if (state.event_callbacks.on_step_analysis) {
        state.event_callbacks.on_step_analysis();
    }
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_step_analysis");
    }
}

void lua_hook_save_network(AnalysisState& state) {
    if (state.event_callbacks.on_save_network) {
        state.event_callbacks.on_save_network();
    }
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_save_network");
    }
}
