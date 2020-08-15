#include <pthread.h>
#include <stdio.h>
#include <bsd/string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "game_scheduler.h"
#include "utility/registry.h"

pthread_mutex_t scheduleLock = PTHREAD_MUTEX_INITIALIZER;

int game = 0;
int minPlayersPerGame = 2;
int maxPlayersPerGame = 2;

GAME_SCHED_ERR scheduleNextGame() {

    // Get the total number of players in the registry
    int playersInRegistry = registryGetPlayerCount();

    if (playersInRegistry < 2) {
        return GAME_SCHED_NOT_ENOUGH_PLAYERS;
    }

    int ret = 0;
    int g;

    // Get the next game number
    pthread_mutex_lock(&scheduleLock);
    if (game < 10) {
        g = game++;;
    }
    else {
        ret = GAME_SCHED_NO_MORE_GAMES;
    }
    pthread_mutex_unlock(&scheduleLock);

    // Return error if no more games are
    // scheduled
    if (ret) return ret;

    char dir[64];

    // Create the game directory
    snprintf(dir, sizeof(dir), "games/%d/", g);
    mkdir(dir, 0777);

    // Create the player list file
    strlcat(dir, "players.txt", sizeof(dir));
    FILE* f = fopen(dir, "w+");

    // Generate the number of players for this game
    // and store that in the file
    int playerCount = (maxPlayersPerGame - minPlayersPerGame) * rand() + minPlayersPerGame;
    fprintf(f, "%d\n", playerCount);

    // Add playerCount number of players to the list
    for (int i = 0; i < playerCount; ++i) {
        user_t* p = registryGetUserAt(rand() % playersInRegistry);

        fprintf(f, "%d\n", p->id);
    }

    // Close the file
    fclose(f);

    // Return the game number
    return g;
}

int loadGameSchedulerState() {
    int fd = open("state/gamescheduler.dat", O_RDONLY, S_IRUSR);
    if (fd == -1) return 1;

    read(fd, &game, sizeof(game));
    close(fd);

    return 0;
}

int saveGameSchedulerState() {
    int fd = open("state/gamescheduler.dat", O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR);
    write(fd, &game, sizeof(game));
    close(fd);

    return (fd == -1);
}

int getCurrentGameId() {
    int ret;
    pthread_mutex_lock(&scheduleLock);
    ret = game;
    pthread_mutex_unlock(&scheduleLock);
    return ret;
}