#include <stdio.h>
#include <unistd.h>

#include "util.h"
#include <uv.h>

static void finish(uv_work_t *req, int status) {
    // Do nothing
}

struct ThreadContext {
    ThreadContext() {
        loop = NULL;
    }
    void start() {
        loop = uv_loop_new();
        ASSERT(uv_thread_create(&id, thread_runner, this) >= 0, "ThreadError");
    }
    void join() {
        uv_thread_join(&id);
    }
    void queue(uv_work_cb work, void* data) {
        uv_work_t req;
        req.data = data;
        uv_queue_work(loop, &req, work, finish);
    }
private:
    static void thread_runner(void* arg) {
        ThreadContext* context = (ThreadContext*)arg;
        uv_run(context->loop, UV_RUN_NOWAIT);
        printf("Exitting\n");
    }
    uv_thread_t id;
    uv_loop_t* loop; // We keep one loop per thread
};


static void say(uv_work_t *req) {
    fprintf(stdout, "%s\n", req->data);
}

int uv_main(int argc, char** argv) {
    ThreadContext context;
    context.start();

    context.queue(say, (void*)"Hello World!");

    return 0;
}
