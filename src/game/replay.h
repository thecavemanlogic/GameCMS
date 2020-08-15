#ifndef _REPLAY_H
#define _REPLAY_H

// Replay file structure:

// Overview:
// 		Header
// 		Frames
// 			The first frame begins after the null terminator of the game title in the header
// * All in little endian format!!

// Header:
// Offset	Size	Description
// 0		4		Magic, must be 'GMRP'
// 4		2		Version number
// 6		2		The number of frames in the recording
// 8		?		The game title that this replay was generated from. Terminated by '\0'

// Frame:
// Offset	Size	Description
// 0		4		Size of the frame
// 4		#		The internal data to be used by the renderer

// Defines how the renderer in the client's browser
// can jump from frame to frame
typedef enum {

	// Allows the recording to jump to any
	// frame within the data.
	PLAYBACK_RANDOM_ACCESS,

	// Requires the renderer to visit any preceding frames that
	// exist before visiting later frames
	PLAYBACK_CHRONOLOGICAL

} PLAYBACK_MODE;

// Defines what kind of replay information is stored in the file
typedef enum {
	
	// Defines that the data is describing an abstract state that is not
	// understood by the GameAPI's internal renderer API.
	REPLAY_RAW,

	// Defines that the data is describing a game-like state. This includes
	// objects with individual positions, angles, and images. A background
	// tilemap is also included
	REPLAY_GAME,

} REPLAY_TYPE;

typedef struct {

	// The playback mode of the replay file
	PLAYBACK_MODE playbackMode;

	// The replay type of the replay file
	REPLAY_TYPE replayType;

	// The number of players in the replay file
	int playerCount;

	// A pointer to an array of player ids
	int *players;

} replay_options_t;

// Description
// 	Opens a new replay file
// 
// Parameters
// 	rendererName - the name of the renderer to use (currently not used)
// 	options - the options to use when creating the replay file
// 
// Returns
// 	0 on success, -1 on failure
int replayOpen(const char* rendererName, replay_options_t* options);

// Description
// 	Writes some data to the current frame
// 
// Parameters
// 	buf - the buffer where the data is located
// 	size - the size of the data in the buffer
// 
// Returns
// 	0 on success, -1 on failure
int replayWrite(const void* buf, int size);

// Description
// 	Writes the current frame to the replay file and clears the
// 	frame's internal buffer.
// 
// Parameters
// 	None
// 
// Returns
// 	0 on success, -1 on failure
int replayPushFrame();

// Description
// 	Closes the replay file and frees all alloc'd memory
// 
// Parameters
// 	None
// 
// Returns
// 	Nothing
void replayClose();

#endif