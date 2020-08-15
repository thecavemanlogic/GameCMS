#ifndef _GAME_SCHEDULER_H
#define _GAME_SCHEDULER_H

typedef enum {
    GAME_SCHED_NOT_ENOUGH_PLAYERS = -1,
    GAME_SCHED_NO_MORE_GAMES = -2
} GAME_SCHED_ERR;

// When called, this function will create a new game directory
// if another game can be scheduled. If -1 is returned, then
// no more games are scheduled.

// Description
// 	
// Parameters
// 	
// Returns
// 	
GAME_SCHED_ERR scheduleNextGame();

// Loads the scheduler's state from state/gamescheduler.dat
// if it exists there.

// Description
// 	
// Parameters
// 	
// Returns
// 	
int loadGameSchedulerState();

// Loads the scheduler's current state to state/gamescheduler.dat
// if the directory exists.

// Description
// 	
// Parameters
// 	
// Returns
// 	
int saveGameSchedulerState();

int getCurrentGameId();

#endif