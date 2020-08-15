#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "scheduler.h"

#define EVENT_COUNT 20

time_t start = 0;

int evt(void* data) {
    int d = (int)(long long) data;

    printf("%d - %ld\n", d, time(NULL) - start);

    return 0;
}

int main() {

    event_t e = { 0 };

    if (startScheduler()) {
        perror("Could not start scheduler");
    }

    start = time(NULL);

    e.event = evt;

    e.data = 0;
    e.time = 5;
    scheduler_addEvent(&e);

    sleep(1);

    e.data = 1;
    e.time = 2;
    scheduler_addEvent(&e);

    sleep(60);

    return 0;
}