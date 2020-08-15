#include <stdio.h>

#include "admin.h"
#include "init.h"
#include "server.h"
#include "builder.h"
#include "game_runner.h"
#include "game_scheduler.h"
#include "utility/config.h"
#include "utility/registry.h"
#include "scheduler.h"


typedef struct {
    const char* name;
    int (*init)(void);
    int (*run)(void);
    int (*close)(void);
} task_t;

task_t tasks[] = {

    { "init", initDirectory, NULL, NULL },

    { "scheduler", startScheduler, NULL, NULL },
    
    { "registry", registryInit, NULL, registryClose },

    { "builder", builderInit, NULL, builderClose },
    { "game scheduler", loadGameSchedulerState, NULL, saveGameSchedulerState },
    { "game runners", initGameRunners, NULL, closeGameRunners },
    { "server", serverInit, NULL, serverClose },

    // Has to be the last because prompt() (in adminRun) blocks
    { "admin", adminInit, adminRun, adminClose },
};

int taskCount = sizeof(tasks) / sizeof(tasks[0]);

void init() {
    for (int i = 0; i < taskCount; ++i) {
        if (tasks[i].init && tasks[i].init()) {
            printf("Warning: an error occured when initializing %s\n", tasks[i].name);
        }
        else {
            printf("Successfully initialized %s\n", tasks[i].name);
        }
    }
}

void run() {
    for (int i = 0; i < taskCount; ++i) {
        if (tasks[i].run && tasks[i].run()) {
            printf("Warning: an error occured when running %s\n", tasks[i].name);
        }
    }
}

void cleanup() {
    for (int i = 0; i < taskCount; ++i) {
        if (tasks[i].close && tasks[i].close()) {
            printf("Warning: an error occured when closing %s\n", tasks[i].name);
        }
        else {
            printf("Successfully closed %s\n", tasks[i].name);
        }
    }
}

int makeRank(void *data) {
    if (data) {}

    registryRankPlayers();

    event_t evt = { 0 };
    evt.event = makeRank;
    evt.time = 5;
    scheduler_addEvent(&evt);

    return 0;
}

int main() {

    init();

    event_t evt = { 0 };
    evt.event = makeRank;
    evt.time = 5;

    scheduler_addEvent(&evt);

    // user_t* user = registryGetUserAt(0);

    // printf("NAME = %s\n", user->name);

    run();
    cleanup();

    return 0;
}