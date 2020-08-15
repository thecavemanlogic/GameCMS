#ifndef _CONFIG_H
#define _CONFIG_H

#define MAX_CONFIG_VALUE_LENGTH 64

#define LANGUAGE_COUNT 6

typedef enum {
    LANG_C,
    LANG_CPP,
    LANG_PYTHON,
    LANG_JAVA,
    LANG_CSHARP,
	LANG_JAVASCRIPT,
} language_id_t;

typedef struct {

	// The full name of the language, or how it is popularly expressed.
	char name[16];

	// How the language should be referenced in the config file
	char shorthand[8];

	// The command to build source files of this language
	char buildCommand[MAX_CONFIG_VALUE_LENGTH];

	char runCommand[MAX_CONFIG_VALUE_LENGTH];

	// The maximum memory to be used by this language
	int maxMemory;

	// The amount of time in milliseconds this language has to respond in a frame
	int timeout;

	// The file extension of the source file
	char extension[8];

	// The file extension of the executable
	char runExtension[8];
} lang_config_t;

typedef struct {
	char root[MAX_CONFIG_VALUE_LENGTH];
} system_config_t;

typedef struct {
	int useHttps;
	int maxClients;
	int maxPayloadSize;
	int maxRequestsPerMinute;
	int maxHeaderCount;
	int maxCookieCount;
	char root[MAX_CONFIG_VALUE_LENGTH];
} server_config_t;

typedef struct {
	int minPlayers;
	int maxPlayers;
	int count;
} game_config_t;

typedef struct {

	// Language-specifig configurations
	lang_config_t languages[LANGUAGE_COUNT];

	// System
	system_config_t system;

	// Server
	server_config_t server;

	// Game
	game_config_t game;
} config_t;

extern config_t config;

// Description
// 	
// Parameters
// 	
// Returns
// 	
int loadConfig(const char* filename);

// Description
// 	
// Parameters
// 	
// Returns
// 	
lang_config_t* getLangAddress(const char* lang);

#endif