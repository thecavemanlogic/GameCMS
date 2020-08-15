// File: game_api.h
// Author: Caedmon M. Evans
// Description:
//  Declares functions for monitoring and interacting with player processes

#ifndef _GAME_API_H
#define _GAME_API_H

#include <stdio.h>
#include <pthread.h>

// #include "../util.h"

typedef enum GAME_ERROR {

	SUCCESS,

	INDEX_OUT_OF_RANGE,

} GAME_ERROR;

typedef enum PLAYER_ERROR {

	// No errors are or were discovered with the player
	INNOCENT,

	// The process exits early and it's exit code is not 0.
	RUNTIME,

	// The process exits early and it's exit code is 0.
	EARLY_EXIT,

	// The in or out player pipe is closed.
	WHAAAT,

	// The output from the player was out of range.
	OUT_OF_RANGE,

	// The output from the player was in the wrong format.
	WRONG_FORMAT,

	// The player ran out of time before it could output a single line.
	TIMEOUT,

	OTHER
} PLAYER_ERROR;

typedef enum PLAYER_STATUS {

	PLAYER_STATUS_STOPPED,

	PLAYER_STATUS_PROCESS_RUNNING,

	PLAYER_STATUS_THREAD_RUNNING,

	PLAYER_STATUS_RUNNING

} PLAYER_STATUS;

// Player Instance
// Holds information necessary for running a player process
// in a game. This is used for holding information for game scripts.
typedef struct {

	// The global id of the player
	int id;

	// Flag for seeing if the player is currently active
	char active;

	// Process Id of the player process
	int pid;

	// Input and output file descriptors for the process
	int fd_in, fd_out;

	// Input and output streams for the process
	FILE *in, *out;

	// The score of the player at the end of a game
	int score;

	// The penalty of the player at the end of a game
	int penalty;

	// The error code, if any, that the player process performs
	int error;

	// Internal buffer used to store the input of the player whenever the
	// last read call was called on this player instance.
	char buffer[256];

	// The amount of time allotted to this player, preferably based on programming language.
	// Measured in milliseconds.
	int timeout;

	// Holds the thread in charge of sending and receiving data in the correct timeframe
	pthread_t thread;

	// General lock for changing or accessing non-thread-safe player information
	pthread_mutex_t lock;

	// Keeps the player's thread on stand by so that we can avoid allocating new threads
	pthread_mutex_t run_lock;

	// Prevents the player's data from being read until the process returns or the timeout
	// expires.
	pthread_mutex_t read_lock;

	// Total number of microseconds spent between receiving data and returning output.
	long long totalResponseTime;

	// Total number of "round trips" of data for the player
	long long numberOfResponses;

} player_t;

// Description
// 	Sends a message to a player process. If startTimeout is a non-zero value,
// 	then the file stream is flushed to the player and the time out sequence
// 	is started.
// 
// Parameters
// 	playerId - the player id of the process
// 	message - the message to send to the process
// 	startTimeout - a flag indicating whether to start the timeout timer
// 
// Returns
// 	Nothing
void GameAPI_playerWrite(int playerId, const char* message, int startTimeout);

// Description
// 	Read a line from the a player's stdout. This function is not
// 	thread-safe.
// 
// Parameters
// 	playerId - the id of the player process to read from
// 
// Returns
// 	the line of the output, NULL on failure
char* GameAPI_playerRead(int playerId);

// Description
// 	Starts a timeout on a player process.
// 
// Parameters
// 	playerId - the id of the player to start a timeout
// 
// Returns
// 	a PLAYER_ERROR:
// 		INNOCENT - when the player returns output before the timer runs out
// 		TIMEOUT - the timeout expired before the player could send any output
// 		OUT_OF_RANGE - playerId is not a valid player id
PLAYER_ERROR GameAPI_playerStartTimeout(int playerId);

// Description
// 	Changes a player's score by a certain amount
// 
// Parameters
// 	playerId - the id of the player whose score to change
// 	change - the amount to change the player's score by
// 
// Returns
// 	the new player score
int GameAPI_playerChangeScoreBy(int playerId, int change);

// Description
// 	Sets a player's score to a certain amount.
// 
// Parameters
// 	playerId - the id of the player whose score to change
// 	newScore - the score to set the player's score to
// 
// Returns
// 	The new player score
int GameAPI_playerSetScoreTo(int playerId, int newScore);

// Description
// 	Returns the player's score
// 
// Parameters
// 	playerId - the id of the player to get a score from
// 
// Returns
// 	The score of the player
int GameAPI_playerGetScore(int playerId);

// Description
// 	Adds a penalty score to a player.
// 
// Parameters
// 	playerId - the id of the player to change the penalty score
// 	penalty - the amount of penalty to add to the player
// 
// Returns
// 	The new penalty of the player
int GameAPI_playerAddPenalty(int playerId, int penalty);

// Description
// 	Throws an error on a player.
// 
// Parameters
// 	playerId - the id of the player to throw the error on
// 	error - the error to throw on the player
// 
// Returns
// 	Nothing
void GameAPI_playerThrowError(int playerId, PLAYER_ERROR error);

typedef struct player_list_t {
	int playerCount;
	player_t* players;
} player_list_t;

// Description
// 	Initializes the game process and all the player processes.
// 
// Parameters
// 	None
// 
// Returns
// 	0 on success, -1 on failure
int GameAPI_initProcess();

// Description
// 	Writes some data to all active player processes.
// 
// Parameters
// 	data - the data to write to the player processes
// 	startTimeout - the flag indicating whether to start the timeout on the players
// 
// Returns
// 	Nothing
void GameAPI_playerBatchWrite(const char* data, int startTimeout);

// Description
// 	Closes all player processes and frees all API-specific memory
// 
// Parameters
// 	None
// 
// Returns
// 	Nothing
void GameAPI_killProcess();

// Description
// 	Checks if a player is active
// 
// Parameters
// 	playerId - the id of the player to check the activity status of
// 
// Returns
// 	1 if the player is running, 0 if the player is not
int GameAPI_isPlayerRunning(int playerId);

//------------------------------------------//
//		Functions for Internal Use Only		//
//------------------------------------------//

// Description
// 	Function that handles writing to, polling, and reading from
// 	player processes.
// 
// Parameters
// 	_player - the player object for the thread to operate on
// 
// Returns
// 	None
void *playerRunner(void *_player);

// Description
// 	Prints player statistics to stdout
// 
// Parameters
// 	playerId - the id of the player to print the statistics of
// 
// Returns
// 	Nothing
void GameAPI_playerPrintStats(int playerId);

// Description
// 	Opens and initializes the game's replay file
// 
// Parameters
// 	None
// 
// Returns
// 	Nothing
void GameAPI_openReplay();

// Description
// 	Writes some data to the current frame
// 
// Parameters
// 	buf - the buffer where the data is located
// 	size - the size of the data in the buffer
// 
// Returns
// 	0 on success, -1 on failure
int GameAPI_replayWrite(const void* buf, int size);

// Description
// 	Writes the current frame to the replay file and clears the
// 	frame's internal buffer.
// 
// Parameters
// 	None
// 
// Returns
// 	0 on success, -1 on failure
int GameAPI_replayPushFrame();

// Description
// 	Opens and initializes the game's replay file
// 
// Parameters
// 	None
// 
// Returns
// 	Nothing
void GameAPI_closeReplay();

#endif