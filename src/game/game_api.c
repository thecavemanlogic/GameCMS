// Sources consulted:
// https://daniel.haxx.se/docs/poll-vs-select.html
// https://stackoverflow.com/questions/21197977/how-can-i-prevent-scanf-to-wait-forever-for-an-input-character
// https://stackoverflow.com/questions/970979/what-are-the-differences-between-poll-and-select
// https://linux.die.net/man/3/poll
// https://linux.die.net/man/3/select
// https://www.man7.org/linux/man-pages/man2/select.2.html

#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>		// for strdup()
#include <sys/select.h>	// for select()
#include <poll.h>		// for poll()
#include <bsd/string.h>	// for strlcpy()
#include <sys/time.h>	// for gettimeofday()
#include <sys/stat.h>	// for open(), 
#include <fcntl.h>		// for open()'s flags
#include <errno.h>

       #include <sys/syscall.h>
	   #include <linux/seccomp.h>

#include "game_api.h"
#include "timer.h"
#include "gamelog.h"
#include "../utility/config.h"
#include "replay.h"

const char *gameErrorMessages[] = {
	"Success",
	"Player index out of range"
};

const char *playerErrorMessages[] = {
	"Innocent",
	"Runtime error",
	"Early exit",
	"WHAAAT",
	"Out of range",
	"Wrong format",
	"Timeout"
};

GAME_ERROR gameErrorCode = SUCCESS;
int playerCount = 0;				// The number of player processes running
player_t* players = NULL;			// The array that holds all player information
int isCurrentlyRunning = 0;
int runningPlayers = 0;				// The current number of running players
pthread_mutex_t pidCloseLock;

char gameDirectory[128];
int gameId;

//------------------------------------------//
// 		Private function declarations 		//
//------------------------------------------//

int validatePlayerId(int playerId);

void playerProcessCleaner(int _signal);

//------------------------------//
// 		Private Functions 		//
//------------------------------//

// Given a integer, check whether it is in the range of
// [0, playerCount)
int validatePlayerId(int playerId) {
	if ((playerId < playerCount) && (playerId >= 0)) {
		return 1;
	}

	gameErrorCode = INDEX_OUT_OF_RANGE;
	return 0;
}

void splitAndRun() {

}

int playerStart(player_t* player) {
	char buf[256];

	// Read the language code
	snprintf(buf, sizeof(buf), "users/%d/lang.txt", player->id);
	FILE* f = fopen(buf, "r");
	if (!f) {
		printf("Could not open '%s'!\n", buf);
		exit(1);
	}
	int langi;
	fscanf(f, "%d", &langi);
	fclose(f);

	lang_config_t* lang = &config.languages[langi];
	
	// Create necessary pipes
	int to_player[2], to_game[2];
	pipe(to_player);
	pipe(to_game);

	int pid = fork();

	if (pid == 0) {			// if child

		close(to_player[1]);
		close(to_game[0]);
		dup2(to_player[0], 0);	// redirect stdin
		dup2(to_game[1], 1);	// redirect stdout
		
		// Redirect stderr
		snprintf(buf, sizeof(buf), "users/%d/stderr/%d.txt", player->id, gameId);
		int fd = open(buf, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		dup2(fd, 2);
		close(fd);

		// Generate executable file path
		char filepath[256];
		snprintf(filepath, sizeof(filepath), "users/%d/code", player->id);

		// Generate executable char array
		snprintf(buf, sizeof(buf), lang->runCommand, filepath);
		char* bookmark;
		char* exec[16] = { 0 };

		fprintf(stderr, "Language: %s\n", lang->name);
		fprintf(stderr, "RunCommand: %s\n", lang->runCommand);

		exec[0] = strtok_r(buf, " ", &bookmark);
		fprintf(stderr, "Arg 0: %s\n", exec[0]);

		for (int i = 1; i < 16; ++i) {

			exec[i] = strtok_r(NULL, " ", &bookmark);

			fprintf(stderr, "Arg %d: %s\n", i, exec[i]);

			if (!exec[i]) break;
		}


		// prctl(PR_SET_NO_NEW_PRIVS, 1);

		// if (unshare(CLONE_FS)) {
		// 	perror("unshare()");
		// }

		// int rc = syscall(SYS_seccomp, SECCOMP_SET_MODE_STRICT, 0, NULL);
		// if (rc == -1) perror("seccomp()");

		// PR_SET_NO_NEW_PRIVS

		execv(exec[0], exec);

		perror("Could not start player process.");
		
		exit(1);
	}
	else if (pid >= 0) {	// if parent
		close(to_player[0]);
		close(to_game[1]);
		player->active = 1;
		player->pid = pid;
		player->fd_in = to_player[1];
		player->fd_out = to_game[0];
		player->in = fdopen(to_player[1], "w");
		player->out = fdopen(to_game[0], "r");
		if (player->in == NULL || player->out == NULL) {
			return -1;
		}

		player->timeout = lang->timeout;

		pthread_mutex_init(&player->lock, NULL);
		pthread_mutex_init(&player->run_lock, NULL);
		pthread_mutex_init(&player->read_lock, NULL);

		// Lock so playerRunner does not start looping
		pthread_mutex_lock(&player->run_lock);

		// Run playerRunner as a new thread
		pthread_create(&player->thread, NULL, playerRunner, player);
	}
	else {
		perror("Could not fork new process");
		return -1;
	}

	puts("Sucess starting player!");
	
	return 0;
}

//----------------------------------//
//		Public (API) Functions		//
//----------------------------------//

void GameAPI_printGameError(const char* msg) {
	fprintf(stderr, "(Game Error) - %s: %s\n", msg, gameErrorMessages[gameErrorCode]);
}

void GameAPI_printPlayerError(const char* msg) {
	fprintf(stderr, "(Player Error) - %s: %s\n", msg, gameErrorMessages[gameErrorCode]);
}

void GameAPI_playerPrintStats(int playerId) {
	if (!validatePlayerId(playerId)) return;

	player_t *player = &players[playerId];

	printf("Id: %d\n", player->id);
	printf("Average Response Time: %f microseconds\n", ((double)player->totalResponseTime / player->numberOfResponses));
}

int GameAPI_initProcess() {
	gamelog_start();

	pthread_mutex_init(&pidCloseLock, NULL);

	gamelog("Initializing process... ");

	isCurrentlyRunning = 1;

	// Install the signal handler
	signal(SIGCHLD, playerProcessCleaner);

	// Read the player list
	FILE* f = fopen("players.txt", "r");
	if (f == NULL) {
		gamelog("Error: could not open players.txt");
		exit(1);
	}

	fscanf(f, "%d", &playerCount);

	runningPlayers = playerCount;
	pthread_mutex_lock(&pidCloseLock);
	players = calloc(playerCount, sizeof(player_t));

	// Get the game directory
	if (!getcwd(gameDirectory, sizeof(gameDirectory))) {
		perror("getcwd()");
	}

	// Get the game id
	char* temp = strstr(gameDirectory, "games/");
	if (!temp) {
		printf("gameDir = %s\n", gameDirectory);
		perror("NAH");
		exit(1);
	}

	temp += strlen("games/");
	char* temp2 = NULL;
	gameId = strtol(temp, &temp2, 10);

	chdir("../../");

	// loadConfig("config.dat");

	for (int i = 0; i < playerCount; ++i) {

		fscanf(f, "%d", &players[i].id);
		
		playerStart(&players[i]);
	}

	fclose(f);

	chdir(gameDirectory);

	// TODO: Remove and replace with an initial timeout
	sleep(1);	// Make sure other processes have time to initialize

	return playerCount;
}

void GameAPI_killProcess() {

	gamelog("Shutting down...");

	isCurrentlyRunning = 0;

	// Rank players based on score
	player_t** ranked = malloc(sizeof(player_t*) * playerCount);
	for (int i = 0; i < playerCount; ++i) {
		ranked[i] = &players[i];
	}

	// Perform a basic sort
	player_t* p;
	for (int i = 0; i < playerCount - 1; ++i) {
		if (ranked[i]->score < ranked[i + 1]->score) {
			p = ranked[i];
			ranked[i] = ranked[i + 1];
			ranked[i + 1] = p;

			for (int j = i - 1; j >= 0; ++j) {
				
				if (ranked[j]->score < ranked[j + 1]->score) {
					p = ranked[j];
					ranked[j] = ranked[j + 1];
					ranked[j + 1] = p;
				}

			}

		}
	}

	
	FILE* f = fopen("results.txt", "w");
	fprintf(f, "%d\n", playerCount);
	for (int i = 0; i < playerCount; ++i) {
		player_t* player = ranked[i];
		
		fprintf(f, "%d %d %d\n", player->id, player->score, player->penalty);

		if (player->active) {
			kill(player->pid, SIGKILL);
		}

		pthread_join(player->thread, NULL);

		fclose(player->in);
		fclose(player->out);		
	}
	fclose(f);

	gamelog("Waiting for all players to close...");
	// printf("Waiting for all players to close...");
	pthread_mutex_lock(&pidCloseLock);
	// printf("done\n");

	// Free the name here in case the player process exits early
	// for (int i = 0; i < playerCount; ++i) {
	// 	free(players[i].name);
	// }

	free(players);
	players = NULL;

	playerCount = 0;

	gamelog("Player processes successfully terminated, now exiting...");
	exit(0);
}

void GameAPI_playerWrite(int playerId, const char* message, int startTimeout) {
	if (!validatePlayerId(playerId) || !players[playerId].active) return;

	strlcat(players[playerId].buffer, message, sizeof(players[playerId].buffer));

	if (startTimeout) {
		pthread_mutex_unlock(&players[playerId].run_lock);
		pthread_mutex_lock(&players[playerId].read_lock);
	}
}

char* GameAPI_playerRead(int playerId) {
	if (!validatePlayerId(playerId) || !players[playerId].active) return NULL;

	char *ret;

	pthread_mutex_lock(&players[playerId].read_lock);

	ret = (players[playerId].error == INNOCENT) ? players[playerId].buffer : NULL;

	pthread_mutex_unlock(&players[playerId].read_lock);

	return ret;
}

int GameAPI_playerChangeScoreBy(int playerId, int change) {
	if (!validatePlayerId(playerId) || !players[playerId].active) return 0;

	players[playerId].score += change;
	return players[playerId].score;
}

int GameAPI_playerSetScoreTo(int playerId, int newScore) {
	if (!validatePlayerId(playerId) || !players[playerId].active) return 0;

	players[playerId].score = newScore;
	return players[playerId].score;
}

int GameAPI_playerGetScore(int playerId) {
	if (!validatePlayerId(playerId) || !players[playerId].active) return 0;

	return players[playerId].score;
}

int GameAPI_playerAddPenalty(int playerId, int penalty) {
	if (!validatePlayerId(playerId) || !players[playerId].active) return -1;

	players[playerId].penalty += penalty;
	return players[playerId].penalty;
}

void GameAPI_playerThrowError(int playerId, PLAYER_ERROR error) {
	if (!validatePlayerId(playerId) || !players[playerId].active) return;

	players[playerId].error = error;
}

void GameAPI_playerBatchWrite(const char* data, int startTimeout) {
	// Unlock each player's lock so playerRunner can do
	// the work
	for (int i = 0; i < playerCount; ++i) {
		// printf("batch writing to player %d (%4s)\n", i, data);
		if (players[i].active) GameAPI_playerWrite(i, data, startTimeout);
		// else printf("Player %d is not active\n", i);
	}
}

int GameAPI_isPlayerRunning(int playerId) {
	if (!validatePlayerId(playerId)) return 0;
	else return players[playerId].active;
}

void *playerRunner(void *_player) {
	player_t* player = (player_t*) _player;

	while (1) {
		// Only lock the player so that this thread can be on standby
		// ready to execute.
		pthread_mutex_lock(&player->run_lock);
		if (!player->active) break;

		// Write the data to the player
		fwrite(player->buffer, 1, strlen(player->buffer), player->in);
		fflush(player->in);

		// if (ferror(player->in) || ferror(player->out)) {
		// 	printf("ERROR with player %s\n", player->name);
		// }

		// printf("Sending \"%s\" to %s\n", player->buffer, player->name);

		game_timer_t timer;
		timer_start(&timer);

		// Set timeout using poll()
		struct pollfd fds[1];
		fds[0].fd = player->fd_out;
		fds[0].events = POLLIN | POLLPRI;
		int ret = poll(fds, 1, player->timeout);

		// Record timing information
		long long time = timer_stop(&timer);
		player->totalResponseTime += time;
		player->numberOfResponses++;

		if (ret == 0) {
			player->error = TIMEOUT;
			char buf[64];
			snprintf(buf, sizeof(buf), "player %d timed out! exiting now...", player->id);
			gamelog(buf);

			pthread_mutex_unlock(&player->read_lock);
			kill(player->pid, SIGKILL);
			break;
		}
		else if (ret < 0) {
			perror("poll()");
			if (errno == EINTR) {
				errno = 0;
				// TODO: retry?
			}
			printf("Just continuing on...\n");
			pthread_mutex_unlock(&player->read_lock);
			break;
		}

		// Read the data from the player
		// For some reason fread does not work...
		// fread(player->buffer, 1, sizeof(player->buffer), player->out);
		fgets(player->buffer, sizeof(player->buffer), player->out);
		// printf("%s GOT = %s", player->name, player->buffer);

		// Unblock GameAPI_playerRead()
		pthread_mutex_unlock(&player->read_lock);
	}
	
	// Mark the player as now terminated
	pthread_mutex_lock(&player->lock);
	player->active = 0;
	pthread_mutex_unlock(&player->lock);

	// printf("ending thread...\n");

	return NULL;
}

// Signal handler for SIGCHLD. Closes related pipes and files as well.
void playerProcessCleaner(int _signal) {

	gamelog("Terminating process...");

	if (_signal) {}

	int pid = waitpid(0, NULL, 0);

	// find the player
	player_t* player = NULL;
	for (int i = 0; i < playerCount; ++i) {
		if (pid == players[i].pid) {
			player = &players[i];
			break;
		}
	}

	if (player->active) {
		// Mark the player as now terminated
		pthread_mutex_lock(&player->lock);
		player->active = 0;
		pthread_mutex_unlock(&player->lock);

		// Allow the thread to continue execution and
		// eventually exit.
		pthread_mutex_unlock(&player->run_lock);
	}
	
	// printf("closing %s\n", player->name);

	pthread_mutex_unlock(&player->read_lock);

	--runningPlayers;
	if (runningPlayers == 0) pthread_mutex_unlock(&pidCloseLock);

	gamelog("Process terminated");
}

int GameAPI_replayWrite(const void* buf, int size) {
	return replayWrite(buf, size);
}

int GameAPI_replayPushFrame() {
	return replayPushFrame();
}

void GameAPI_openReplay(const char* rendererName) {
	int _players[playerCount];
	
	for (int i = 0; i < playerCount; ++i) {
		_players[i] = players[i].id;
	}

	replay_options_t options = { 0 };
	options.playerCount = playerCount;
	options.players = _players;

	if (replayOpen(rendererName, &options)) {
		perror("replayOpen()");
	}
}

void GameAPI_closeReplay() {
	replayClose();
}