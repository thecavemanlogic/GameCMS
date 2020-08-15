#include <sys/time.h>

#include "timer.h"

void timer_start(game_timer_t* timer) {
	gettimeofday(&timer->timestamp1, 0);
}

long long timer_stop(game_timer_t* timer) {
	gettimeofday(&timer->timestamp2, 0);

	long long time1 = timer->timestamp1.tv_sec * 1000000 + timer->timestamp1.tv_usec;
	long long time2 = timer->timestamp2.tv_sec * 1000000 + timer->timestamp2.tv_usec;

	return time2 - time1;
}