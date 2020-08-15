#include <stdio.h>
#include <stdlib.h>
#include <string.h>	// for memcpy()

#include "replay.h"
#include "game_api.h"

#define INITIAL_FRAME_SIZE 4096
#define MAX_FRAME_SIZE_AFTER_CLOSE 65536

struct {
	// The version of the replay file
	int version;

	// The number of frames in the replay file
	int frames;

	// The PLAYBACK_MODE of the recording
	PLAYBACK_MODE playbackMode;

	// The REPLAY_TYPE of the recording
	REPLAY_TYPE replayType;

	// The number of players in the replay file
	int playerCount;

	// The player array
	int players[];

} header;

const char* magic = "GMRP";

void *frame_buf = NULL;
int frame_capacity = 0;
int frame_size = 0;

FILE* file = NULL;

int replayOpen(const char* game_title, replay_options_t* options) {

	if (options == NULL) return -1;

	// Open the file
	file = fopen("replay.bin", "wb+");
	if (!file) return -1;

	// Write the magic
	fwrite(magic, 1, strlen(magic), file);

	// Update the header
	header.frames = 0;
	header.version = 0;
	header.playbackMode = options->playbackMode;
	header.replayType = options->replayType;
	header.playerCount = options->playerCount;

	// Write the header
	fwrite(&header, sizeof(header), 1, file);
	fwrite(options->players, sizeof(int), options->playerCount, file);
	fwrite(game_title, 1, strlen(game_title) + 1, file);

	// Allocate resources for the internal frame buffer
	frame_buf = malloc(INITIAL_FRAME_SIZE);
	if (!frame_buf) {
		replayClose();
		return -1;
	}

	frame_capacity = INITIAL_FRAME_SIZE;
	frame_size = 0;

	return 0;
}

int replayWrite(const void* buf, int size) {

	if (size == 0) return -1;

	// Resize the frame buffer if needed
	if (frame_size + size > frame_capacity) {
		frame_buf = realloc(frame_buf, frame_capacity * 2);
		frame_capacity *= 2;
	}

	// Copy the contents of buf
	memcpy(frame_buf + frame_size, buf, size);

	frame_size += size;

	return 0;
}

int replayPushFrame() {

	if (frame_size == 0) return -1;

	// Write the frame size to the file
	fwrite(&frame_size, sizeof(frame_size), 1, file);

	// Writes the contents of the frame to the file
	fwrite(frame_buf, 1, frame_size, file);

	++header.frames;

	frame_size = 0;

	return 0;
}

void replayClose() {

	// Make sure the final frame is written
	if (frame_size > 0) replayPushFrame();

	// Write the final "frame" which has a data size of zero
	fwrite(&frame_size, sizeof(frame_size), 1, file);

	// Write update the header at the beginning of the file
	fseek(file, strlen(magic), SEEK_SET);
	fwrite(&header, sizeof(header), 1, file);

	fclose(file);

	// Free the associated memory
	free(frame_buf);
	frame_buf = NULL;
	frame_capacity = 0;
	frame_size = 0;
}