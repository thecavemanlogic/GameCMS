// Functions and structs for timers

#ifndef _TIMER_H
#define _TIMER_H

#include <sys/time.h>

typedef struct game_timer_t {

	struct timeval timestamp1, timestamp2;

} game_timer_t;

// Description
// 	
// Parameters
// 	
// Returns
// 	
void timer_start(game_timer_t* timer);

// Description
// 	
// Parameters
// 	
// Returns
// 	
long long timer_stop(game_timer_t* timer);

#endif