#include <stdio.h>
#include <string.h>

#include "replay.h"

const char* frames[] = {
    "CHICKEN",
    "POT",
    "PIE"
};

const int frameCount = sizeof(frames) / sizeof(char*);

int main() {
    int p[2];
    p[0] = 5320495;
    p[1] = 2384722;

    replay_options_t options = { 0 };
    options.playerCount = 2;
    options.players = p;

    replayOpen("BOBBY", &options);

    for (int i = 0; i < frameCount; ++i) {
        replayWrite(frames[i], strlen(frames[i]));
        replayPushFrame();
    }

    replayClose();
}