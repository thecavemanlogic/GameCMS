#ifndef _REGISTRY_H
#define _REGISTRY_H

#define MAX_USERS             100
#define MAX_USER_NAME_LENGTH  32
#define MAX_PASSWORD_LENGTH     32
#define DEFAULT_PASSWORD_LENGTH 16
#define SALT_LENGTH             16

#include "config.h"

#include <openssl/sha.h>

typedef struct {
	unsigned char hash[SHA256_DIGEST_LENGTH];
} hash_t;

typedef struct {
    unsigned char salt[SALT_LENGTH];
} salt_t;

typedef struct {
    char username[MAX_USER_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    salt_t salt;
} credentials_t;

typedef enum {

    // User permissions
    PERM_SUBMIT          = 1 << 0,
    PERM_CHANGE_PASSWORD = 1 << 1,
    PERM_ASK_QUESTIONS   = 1 << 2,
    PERM_BECOME_PLAYER   = 1 << 3,

    // Admin permissions
    PERM_BAN_USER        = 1 << 4,
    PERM_SILENCE         = 1 << 5,

} PERMISSIONS;

#define DEFAULT_USER_PERMISSIONS ( PERM_SUBMIT | PERM_CHANGE_PASSWORD | PERM_ASK_QUESTIONS | PERM_BECOME_PLAYER )
#define DEFAULT_ADMIN_PERMISSIONS ( PERM_BAN_USER | PERM_SILENCE )

typedef struct {
    int id;
    char name[32];
    int permissions;
    int isAdmin;

    int isPlayer;
    int score;
    int penalty;
    language_id_t lang;
} user_t;

// Description
// 	Initializes the registry
// 
// Parameters
//  None
// 
// Returns
// 	0 on success, -1 on error
int registryInit();

// Returns the number of players that are currently registered
// in the registry

// Description
// 	Returns the number of users in the registry
// 
// Parameters
//  None
// 
// Returns
// 	The number of users in the registry
int registryGetUserCount();

// Description
//  Registers a new user in the registry. The password
//  of cred is optional, as if the length is zero, a random
//  password is generated. Salt should not be initialized.
// 
// Parameters
//  cred - the credentials for the new user
// 
// Returns
// 	0 on success, -1 on failure
int registryRegisterUser(credentials_t* cred);

// Description
// 	Removes a player based on a name
// 
// Parameters
//  pname - the name of the player to remove
// 	
// Returns
// 	0 on success, -1 on failure
int registryRemoveUser(const char* pname);

// Description
// 	Validates player credentials
// 
// Parameters
// 	cred - the credentials to validate
// 
// Returns
// 	1 if the credentials are valid, 0 if not
int registryValidateLogin(credentials_t* cred);

// Description
// 	Translates a user id to a name
// 
// Parameters
// 	pname - the name of the user to get the id of
// 
// Returns
// 	the id of the player on success, -1 on failure
int registryGetUserId(const char* pname);

// Description
// 	Retrieves and returns the user with a certain id.
// 
// Parameters
// 	id - the id of the user to get
//  ret - the location to write data to
// 
// Returns
// 	The user on success, NULL on failure
user_t* registryGetUser(int id);

// Description
// 	Gets the user at a index offset in the registry vector
// 
// Parameters
// 	index - the index to retrieve information from
//  ret - the address to write data to
// 
// Returns
// 	The user on success, NULL on failure
user_t* registryGetUserAt(size_t index);

// Description
// 	Marks a user as a player by adding the user
//  to the player vector.
// 
// Parameters
// 	userId - the id of the user to make a player
// 
// Returns
// 	0 on success, -1 on failure
int registryAddPlayer(int userId);

// Description
// 	Returns the number of players in the registry
// 
// Parameters
//  None
// 
// Returns
// 	The number of players in the registry
int registryGetPlayerCount();

// Description
// 	Sorts the player vector by their score.
// 
// Parameters
//  None
// 
// Returns
// 	Nothing
void registryRankPlayers();

// Description
// 	Generates the filepath of a player's program executable into a buffer
// 
// Parameters
// 	buf - the buffer to write the directory to
//  bufSize - the total size of the buffer
//  playerId - the id of the player
// 
// Returns
// 	0 on success, -1 on failure
int registryGetPlayerProgram(char* buf, size_t bufSize, int playerId);

// Description
// 	Generates the filepath of a users directory into a buffer
// 
// Parameters
// 	userId - the id of the user
// 
// Returns
// 	0 on success, -1 on error
int registryGetUserDirectory(char* buf, size_t bufSize, int userId);

// Description
// 	Updates players' scores and penalties from a game's results.txt
// 
// Parameters
// 	gameId - the id of the game
// 
// Returns
// 	0 on success, -1 on error
int registryLoadGameResults(int gameId);

// Description
// 	Clears all information stored in the registry
// 
// Parameters
// 	None
// 
// Returns
// 	Nothing
void registryClear();

// Description
// 	Writes all registry information into a file for later use and
//  cleans up any malloc'd memory.
// 
// Parameters
// 	None
// 
// Returns
// 	Nothing
int registryClose();

#endif