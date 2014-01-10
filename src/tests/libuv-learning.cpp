#include <stdio.h>
#include <unistd.h>

#include <uv.h>

#include "util.h"
#include "lcommon/Range.h"

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


struct TEntity {
    int entity_type;
    TEntity() {
        entity_type = -1;
    }
};

struct TNetwork {
    TEntity* entities; //** This is a pointer - used to create a dynamic array
    int n_entities, max_entities;
    TNetwork() {
        entities = NULL;
        n_entities = 0;
        max_entities = 0;
    }
    ~TNetwork() { //** This defines how to clean-up our Network object; we free the dynamic array
        free(entities);
    }
    TEntity& operator[](int index) { //** This allows us to index our Network struct as if it were an array.
        return entities[index];
    }

    void preallocate(int n) {
        max_entities = n;
        entities = (TEntity*)malloc(sizeof(TEntity) * max_entities);
        // This is very likely to be a large allocation, check for failures:
        if (entities == NULL) {
            error_exit("Network::preallocate failed");
        }
        for (int i = 0; i < max_entities; i++) {
            // Placement new due to use of malloc
            new (&entities[i]) TEntity();
        }
    }
};

struct TNetworkSlice {
    TEntity* entities; //** This is a pointer - used to create a dynamic array
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
