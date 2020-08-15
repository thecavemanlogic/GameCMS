#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <bsd/string.h>
#include <sys/stat.h>	// for open(), 
#include <fcntl.h>		// for open()'s flags
#include <sys/inotify.h>

#include "game_runner.h"
#include "game_scheduler.h"
#include "utility/registry.h"

#define NUMBER_OF_THREADS 1

pthread_mutex_t runnerLock = PTHREAD_MUTEX_INITIALIZER;

pthread_t threads[NUMBER_OF_THREADS];

static volatile int running = 0;

// No action needed to take for this, we just
// need something to interrupt the sleep()
// function
static void catch_signal(int sig) { 
	if (sig) {}


}

void runGame(int id) {
    int pid = fork();

    if (pid == 0) {     // If the child
        char buf[64];
        snprintf(buf, sizeof(buf), "games/%d/", id);
        
        chdir(buf);

        // Redirect stdout and stderr
        int logfd = open("game_log.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        // perror("something()");
        dup2(logfd, 1);
        dup2(logfd, 2);
        close(logfd);

        char* args[] = { "../../rps", NULL };

        putenv("LD_LIBRARY_PATH=../../");

        if (execv(args[0], args)) {
            perror("execv()");
        }

        // Shouldn't reach here, but if it does, exit.
        exit(1);
    }
    else {              // If error

    }

    waitpid(pid, NULL, WEXITED);

    char file[64];
    snprintf(file, sizeof(file), "games/%d/results.txt", id);
    struct stat buf;

    while (stat(file, &buf)) usleep(50000);

    while (buf.st_size < 2) {
        stat(file, &buf);
        usleep(50000);
    }

    usleep(50000);

    int r = registryLoadGameResults(id);
    if (r) {
        perror("registryLoadGameResults(id)");
    }
    else {
        puts("registryLoadGameResults(id) successful");
    }
}

void* gameRunnerTask(void* data) {

    if (data) {}

    // Install the signal handler
    struct sigaction sa = { 0 };
	sa.sa_handler = catch_signal;

	if (sigaction(SIGINT, &sa, NULL)) {
		perror("sigaction()");
	}

    while (1) {
        int r;
        int game;

    try_again:
        pthread_mutex_lock(&runnerLock);
        r = running;
        pthread_mutex_unlock(&runnerLock);
        
        if (!r) break;
    
        game = scheduleNextGame();
        if (game == GAME_SCHED_NO_MORE_GAMES) {
            break;
        }
        else if (game == GAME_SCHED_NOT_ENOUGH_PLAYERS) {
            sleep(15);
            goto try_again;
        }

        runGame(game);
    }

    return NULL;
}

int initGameRunners() {
    pthread_mutex_lock(&runnerLock);
    int r = running;
    pthread_mutex_unlock(&runnerLock);

    if (r) {
        fprintf(stderr, "Warning: initGameRunners called even though the tasks are still in a running state.\n");
        return -1;
    }

    running = 1;

    for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
        pthread_create(&threads[i], NULL, gameRunnerTask, NULL);
    }

    return 0;
}

int closeGameRunners() {
    pthread_mutex_lock(&runnerLock);
    running = 0;
    pthread_mutex_unlock(&runnerLock);

    for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
        pthread_kill(threads[i], SIGINT);
        pthread_join(threads[i], NULL);
    }

    return 0;
}