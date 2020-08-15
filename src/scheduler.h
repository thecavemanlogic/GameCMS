#ifndef _SHEDULER_H
#define _SHEDULER_H

#include <time.h>

#define MAX_EVENTS 256

typedef struct {
	int (*event)(void*);
	void* data;
	time_t time;
} event_t;

// Adds <event> to the internal scheduler heap

// Description
// 	
// Parameters
// 	
// Returns
// 	
int scheduler_addEvent(event_t* event);

// 
// 

// Description
// 	Initializes the scheduler thread and internal
// 	state
// Parameters
// 	None
// Returns
// 	1
int startScheduler();

#endif