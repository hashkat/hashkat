#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <uv.h>

#include "util.h"

#include "analyzer.h"

#include <luawrap-lib/include/luawrap/luawrap.h>

// Defined in interactive_mode.cpp:
lua_State* init_lua_state();

static void finish(uv_work_t *req, int status) {
    // Do nothing
}


static void idle(uv_idle_t* handle, int status) {
    // Do nothing
}

struct ThreadContext {
    ThreadContext() {
        loop = NULL;
    }
    void start() {
        loop = uv_loop_new();

        uv_idle_init(loop, &idler);
        uv_idle_start(&idler, idle);

        ASSERT(uv_thread_create(&id, thread_runner, this) >= 0, "ThreadError");
    }
    void join() {
        uv_thread_join(&id);
    }
    void queue(uv_work_cb work, void* data) {
        uv_work_t req;
        memset(&req, 0, sizeof(req));
        req.data = data;
        uv_queue_work(loop, &req, work, finish);
    }
private:
    static void thread_runner(void* arg) {
        ThreadContext* context = (ThreadContext*)arg;
        uv_run(context->loop, UV_RUN_DEFAULT);
        printf("Exitting\n");
    }
    uv_thread_t id;
    uv_idle_t idler;
    uv_loop_t* loop; // We keep one loop per thread
};

static lua_State* LOGGING_STATE = NULL;
// Automatically starts:

struct MessagePayload {
    AnalysisState& state;
    const char* type;
    int id1, id2;
};

static void log(uv_work_t *req) {
//    lua_State* L = LOGGING_STATE;
//    MessagePayload* msg = (MessagePayload*) req->data;
//
//    luawrap::globals(L)[msg->type].push();
//    lua_pushinteger(L, msg->id1);
//    lua_pushinteger(L, msg->id2);
//    delete msg;
//
//    lua_call(L, 2, 0);
}

static void log(AnalysisState& state, const char* type, int id1, int id2 = -1) {

    if (LOGGING_STATE == NULL) {
        LOGGING_STATE = (lua_State*)1;
//        LOGGING_STATE = init_lua_state();
        ThreadContext context;
        context.start();
        context.queue(log, NULL);
        context.join();
    }

//    context.queue(log, (void*)new MessagePayload {state, type, id1, id2});

}

void log_follow(AnalysisState& state, int id_follower, int id_followed) {
    if (state.config.enable_logging) {
        log(state, "follow", id_follower, id_followed);
    }
}

void log_add(AnalysisState& state, int id) {
    if (state.config.enable_logging) {
        log(state, "add", id);
    }
}

void log_unfollow(AnalysisState& state, int id_follower, int id_followed) {
    if (state.config.enable_logging) {
        log(state, "unfollow", id_follower, id_followed);
    }
}

void log_tweet(AnalysisState& state, int id_tweeter, int id_tweet) {
    if (state.config.enable_logging) {
        log(state, "tweet", id_tweeter, id_tweet);
    }
}


