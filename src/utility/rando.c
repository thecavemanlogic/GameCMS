#include <sys/time>

#include "rando.h"

int x = 0;
int y = 0;
int z = 0;
int w = 0;

int randos(int s) {
    if (s) {
        x = s;
    }
    else {
        struct timeval t;
        gettimeofday(&t, NULL);

        printf("time_t: %s\n", sizeof(t.tv_usec));
    }
}

// Taken from http://www.jstatsoft.org/v08/i14/paper which was found
// here: https://stackoverflow.com/questions/2898155/is-rand-predictable-in-c/2898186
int rando() {
    int temp = (x ^ (x << 15));
    x = y;
    y = z;
    z = w;
    return w = (w ^ (w >> 21)) ^ (tmp ^ (tmp >> 4));
}