#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "../contest_api.h"
#include "../util.h"

int main() {

	ContestAPI_initContest();

	player_info_t* c = ContestAPI_playerInfoCreate((char*)"player1");
	player_info_t* a = ContestAPI_playerInfoCreate((char*)"player2");

	ContestAPI_playerUpload(c, "player1.c");
	ContestAPI_playerUpload(a, "player2.py");

	printf("Calling ContestAPI_gameInit(2, c, a)...\n");
	game_t* game = ContestAPI_gameInit(2, c, a);

	ContestAPI_gameStart(game, (char*)"./run_game.sh");

	waitpid(game->pid, NULL, 0);
	ContestAPI_gameEnd(game);

	ContestAPI_playerInfoFree(c);
	ContestAPI_playerInfoFree(a);
}