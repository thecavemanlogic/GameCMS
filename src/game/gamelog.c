#include <stdio.h>
#include <sys/time.h>

#include "gamelog.h"

long start_time = 0;

void gamelog_start() {
	gamelog("Initializing logger");

	struct timeval t;
	gettimeofday(&t, NULL);

	start_time = t.tv_sec * 1000000 + t.tv_usec;
}

void gamelog_internal_debug(const char* file, int linenum, const char* msg) {
	struct timeval t;
	gettimeofday(&t, NULL);

	long _time = (t.tv_sec * 1000000 + t.tv_usec) - start_time;
	fprintf(stderr, "%s(%d) [%.7ld] %s\n", file, linenum, _time / 1000, msg);
}

void gamelog_internal(const char* msg) {
	struct timeval t;
	gettimeofday(&t, NULL);

	long _time = (t.tv_sec * 1000000 + t.tv_usec) - start_time;
	fprintf(stderr, "[%ld] %s\n", _time / 1000, msg);
}