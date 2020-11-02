---
title: This will be used as the title-tag of the page head
---

## GameCMS Reference

The GameCMS library has ... functions that contest programs can call and use.

### GameAPI_playerWrite
```c
void GameAPI_playerWrite(int playerId, const char* message, int startTimeout);
```
Sends a message to a player process. If startTimeout is a non-zero value, then the file stream is flushed to the player and the time out sequence is started.
#### Parameters:
* playerId - the player id of the process
* message - the message to send to the process
* startTimeout - a flag indicating whether to start the timeout timer
#### Returns:
Nothing


### GameAPI_playerRead
```c
char* GameAPI_playerRead(int playerId);
```
Read a line from the a player's stdout. This function is not thread-safe.
#### Parameters:
* playerId - the id of the player process to read from
#### Returns:
the line of the output, NULL on failure


### GameAPI_playerStartTimeout
```c
PLAYER_ERROR GameAPI_playerStartTimeout(int playerId);
```
Starts a timeout on a player process.
#### Parameters:
* playerId - the id of the player to start a timeout
#### Returns:
a PLAYER_ERROR:
* INNOCENT - when the player returns output before the timer runs out
* TIMEOUT - the timeout expired before the player could send any output
* OUT_OF_RANGE - playerId is not a valid player id


### GameAPI_playerChangeScoreBy
```c
int GameAPI_playerChangeScoreBy(int playerId, int change);
```
Changes a player's score by a certain amount
#### Parameters:
* playerId - the id of the player whose score to change
* change - the amount to change the player's score by
#### Returns:
the new player score


### GameAPI_playerSetScoreTo
```c
int GameAPI_playerSetScoreTo(int playerId, int newScore);
```
Sets a player's score to a certain amount.
#### Parameters:
* playerId - the id of the player whose score to change
* newScore - the score to set the player's score to
#### Returns:
The new player score


### GameAPI_playerGetScore
```c
int GameAPI_playerGetScore(int playerId);
```
Returns the player's score
#### Parameters:
* playerId - the id of the player to get a score from
#### Returns:
The score of the player


### GameAPI_playerAddPenalty
```c
int GameAPI_playerAddPenalty(int playerId, int penalty);
```
Adds a penalty score to a player.
#### Parameters:
* playerId - the id of the player to change the penalty score
* penalty - the amount of penalty to add to the player
#### Returns:
The new penalty of the player


### GameAPI_playerThrowError
```c
void GameAPI_playerThrowError(int playerId, PLAYER_ERROR error);
```
Throws an error on a player.
#### Parameters:
* playerId - the id of the player to throw the error on
* error - the error to throw on the player
#### Returns:
Nothing


### GameAPI_initProcess
```c
int GameAPI_initProcess();
```
Initializes the game process and all the player processes.
#### Parameters:
None
#### Returns:
0 on success, -1 on failure


### GameAPI_playerBatchWrite
```c
void GameAPI_playerBatchWrite(const char* data, int startTimeout);
```
Writes some data to all active player processes.
#### Parameters:
* data - the data to write to the player processes
* startTimeout - the flag indicating whether to start the timeout on the players
#### Returns:
Nothing


### GameAPI_killProcess
```c
void GameAPI_killProcess();
```
Closes all player processes and frees all API-specific memory.
#### Parameters:
None
#### Returns:
Nothing


### GameAPI_isPlayerRunning
```c
int GameAPI_isPlayerRunning(int playerId);
```
Checks if a player is active
#### Parameters:
* playerId - the id of the player to check the activity status of
#### Returns:
1 if the player is running, 0 if the player is not


###
```c

```

#### Parameters:
* 
#### Returns:
