#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

#include "scheduler.h"

#define D_VALUE 4
#define GET_PARENT(x) ( (x - 1) / D_VALUE )
#define GET_CHILD(x) ( x * D_VALUE + 1 )

pthread_t thread;

event_t events[MAX_EVENTS];
int eventCount = 0;

pthread_mutex_t eventLock = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t eventCond = PTHREAD_COND_INITIALIZER;

void swap(event_t* e1, event_t* e2) {
	event_t temp = *e1;
	*e1 = *e2;
	*e2 = temp;
}

void percolate_up(int index) {
	while (1) {
		int parent_index = GET_PARENT(index);
        if (parent_index < 0) break;

        if (events[parent_index].time > events[index].time) {
			swap(&events[parent_index], &events[index]);
			index = parent_index;
        }
        else {
            break;
        }
	}
}

void percolate_down() {
	int index = 0;

	while (1)
	{
		int child_index = GET_CHILD(index);
        if (child_index >= eventCount) break;

		int end_child = (child_index + D_VALUE < eventCount) ? child_index + D_VALUE : eventCount;
		int lowestCost = 0x7FFFFFFF;
		int lowestIndex = -1;
		int didFind = 0;

		for (int i = child_index; i < end_child; ++i) {
			if (events[i].time < events[index].time && events[i].time < lowestCost) {
				lowestCost = events[i].time;
				lowestIndex = i;
				didFind = 1;
			}
		}

		if (didFind) {
			swap(&events[index], &events[lowestIndex]);
			index = lowestIndex;
		}
		else {
			break;
		}
	}	
}

void addEvent(event_t* e) {
	events[eventCount] = *e;
	percolate_up(eventCount);
	++eventCount;
}

void removeEvent(event_t* e) {
	*e = events[0];
	events[0] = events[--eventCount];
	percolate_down();
}

int scheduler_addEvent(event_t* event) {
	int ret = 0;

	pthread_mutex_lock(&eventLock);

	if (eventCount >= MAX_EVENTS) {
		ret = -1;
	}
	else {
		event_t e = *event;
		e.time += time(NULL);
		if (e.time < events[0].time || eventCount == 0) {
			addEvent(&e);
			// ualarm(50000, 0);
			// alarm(0);
			// printf("Sending alarm\n");
			// pthread_kill(thread, 0);
			// kill(0, 0);
			// puts("SOUNDING ALARM");
			pthread_kill(thread, SIGINT);
		}
		else {
			addEvent(&e);
		}

		// printf("ADDED EVENT\n");
	}

	pthread_mutex_unlock(&eventLock);

	return ret;
}

static volatile int running = 0;
static volatile int sleeping = 0;

// No action needed to take for this, we just
// need something to interrupt the sleep()
// function
void catch_signal(int sig) { 
	if (sig) {}
}

void* schedulerTask(void* data) {
	event_t event;

	struct sigaction sa = { 0 };
	sa.sa_handler = catch_signal;

	if (sigaction(SIGINT, &sa, NULL)) {
		perror("sigaction()");
	}

	if (data) {}

	while (running) {

		if (eventCount > 0) {

			// Get the time for the next task
			pthread_mutex_lock(&eventLock);
			int waittime = events[0].time - time(NULL);
			pthread_mutex_unlock(&eventLock);

			// Wait until we get event time arrives
			// or a new event is scheduled
			// struct timespec temp;
			// gettimeofday(&temp, NULL);
			// temp.tv_sec += waittime;
			// pthread_cond_timedwait(&eventCond, &eventLock, &temp);
			// printf("Sleeping for %d seconds\n", waittime);
			if (sleep(waittime) > 0) {
				continue;
			}

			// Get the next event
			pthread_mutex_lock(&eventLock);
			removeEvent(&event);
			pthread_mutex_unlock(&eventLock);

			// Execute the task
			event.event(event.data);
		}
		else {
			// puts("LONG SLEEP");
			sleep(600);
			// puts("EXIT OF SLEEP");
		}
	}

	return NULL;
}

int startScheduler() {

	if (!running) running = 1;
	else return -1;

	pthread_create(&thread, NULL, schedulerTask, NULL);

	// Make sure our signal handler is installed
	sleep(1);

	return 0;
}

int stopScheduler() {
	running = 0;

	return 0;
}