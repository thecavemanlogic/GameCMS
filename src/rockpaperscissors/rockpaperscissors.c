#include <string.h>
#include <stdlib.h>

#include "game_api.h"

// Transform a string into the correct numerical value
int strToInt(const char* str) {
	if (strcmp(str, "ROCK") == 0) return 0;
	else if (strcmp(str, "PAPER") == 0) return 1;
	else if (strcmp(str, "SCISSORS") == 0) return 2;
	else return -1;
}

int main(int argc, char** argv) {
	int p = GameAPI_initProcess(argv[1]);

	// Make sure we only have 2 players
	if (p != 2) {
		printf("Can only support 2 players!\n");
		exit(1);
	}

	// Initialize the replay file
	GameAPI_openReplay("rockpaperscissors");

	// Notify that we are starting the game
	printf("Starting the game\n");

	// Run 300 games
	for (int i = 0; i < 30; ++i) {
		
		// Signal the players we are ready for input
		GameAPI_playerBatchWrite("Rock, paper, scissors, shoot!\n", 1);

		// Read the inputs for both players
		char *p1s = GameAPI_playerRead(0);
		char *p2s = GameAPI_playerRead(1);

		// Make sure both players didn't time out
		if (!p1s) {
			printf("P1 timed out\n");
			GameAPI_playerSetScoreTo(0, 0);
			break;
		}
		if (!p2s) {
			printf("P2 timed out\n");
			GameAPI_playerSetScoreTo(1, 0);
			break;
		}

		// Remove the newline character from the player's output
		p1s[strlen(p1s) - 1] = '\0';
		p2s[strlen(p2s) - 1] = '\0';

		// Log the player's responses
		printf("%s vs. %s\t", p1s, p2s);

		// Transform the outputs to numerical values
		int p1 = strToInt(p1s);
		int p2 = strToInt(p2s);

		// Make sure both players are still running
		if (!GameAPI_isPlayerRunning(0)) {
			printf("Early exit... player 2 won\n");
			exit(0);
		}
		if (!GameAPI_isPlayerRunning(1)) {
			printf("Early exit... player 1 won\n");
			exit(0);
		}

		unsigned char replayValue[3];
		replayValue[0] = p1;
		replayValue[1] = p2;

		// If both players performed the same action
		if (p1 == p2) {
			puts("Tie");
			GameAPI_playerBatchWrite("Tie\n", 0);
			replayValue[2] = 0b00;
		}
		else if (p1 == p2 - 1 || p1 == p2 + 2) {
			puts("Player 2 won");
			GameAPI_playerWrite(0, "You lost\n", 0);
			GameAPI_playerWrite(1, "You won\n", 0);
			GameAPI_playerChangeScoreBy(1, 1);
			replayValue[2] = 0b10;
		}
		else {
			puts("Player 1 won");
			GameAPI_playerWrite(0, "You won\n", 0);
			GameAPI_playerWrite(1, "You lost\n", 0);
			GameAPI_playerChangeScoreBy(0, 1);
			replayValue[2] = 0b01;
		}

		GameAPI_replayWrite(replayValue, sizeof(replayValue));
		GameAPI_replayPushFrame();

		fflush(stdout);
	}

	GameAPI_closeReplay();

	// GameAPI_playerPrintStats(0);
	// GameAPI_playerPrintStats(1);

	GameAPI_killProcess();
	
	return 1;
}