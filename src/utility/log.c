#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include "log.h"

void logData(const log_info_t* li, const char* data) {
    fprintf(stdout, "[%s:%ld] %s", li->name, time(NULL), data);
}