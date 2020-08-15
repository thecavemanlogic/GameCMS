#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <bsd/string.h>
#include <sys/stat.h>
#include <openssl/rand.h>

#include "registry.h"
#include "../dstruct/vector.h"

// Consulted https://www.openssl.org/docs/man1.0.2/man3/RAND_bytes.html

vector_t users;
vector_t players;

static int currentPlayerId = 0;
int id = 0;
pthread_mutex_t userLock = PTHREAD_MUTEX_INITIALIZER;

void initPlayers(void* _u, void* data) {
	user_t** u = _u;

	if (data) {}

	if ((*u)->isPlayer) {
		vector_push(&players, u);
	}
}

void readUser(void* _dest, FILE* f) {
	void** dest = _dest;

	user_t* u = malloc(sizeof(user_t));
	fread(u, sizeof(user_t), 1, f);

	*dest = u;
}

void freeUser(void* _dest) {
	void** dest = _dest;
	free(*dest);
}

int registryInit() {
    FILE* f = fopen("state/registry.dat", "rb");

	pthread_mutex_lock(&userLock);

	vector_init(&players, sizeof(user_t*), NULL);

    if (f) {

		// Load the users and players
		vector_read(&users, readUser, freeUser, f);

        fread(&id, sizeof(id), 1, f);
		fread(&currentPlayerId, sizeof(currentPlayerId), 1, f);
		fclose(f);

		// Recreate the player vector
		vector_foreach(&users, initPlayers, NULL);
    }
    else {
        vector_init(&users, sizeof(user_t*), freeUser);
    }

	pthread_mutex_unlock(&userLock);

    return 0;
}

int registryGetUserCount() {
	return users.size;
}

void generateRandomPassword(char* buf) {
	unsigned int r[DEFAULT_PASSWORD_LENGTH];
	int ret = RAND_bytes((unsigned char*) r, sizeof(r));
	if (ret == -1) {
		fprintf(stdout, "Error: RAND_bytes is not supported\n");
	}

	for (int i = 0; i < DEFAULT_PASSWORD_LENGTH; ++i) {
		buf[i] = r[i] % 94 + '!';
	}

	buf[DEFAULT_PASSWORD_LENGTH] = '\0';
}

void registryGenerateSalt(salt_t* salt) {
	RAND_bytes(salt->salt, sizeof(salt->salt));
}

// to install OpenSSL: sudo apt-get install libssl-dev
void registryGenerateHash(credentials_t* account, hash_t* hash) {
	// SHA256
	// https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, account, sizeof(credentials_t));
    SHA256_Final(hash->hash, &sha256);
}

int registryRegisterUser(credentials_t* cred) {
	int ret = 0;

	if (users.size >= MAX_USERS) return -1;

	// Return an error if the user already exists
	if (registryGetUserId(cred->username) != -1) return -1;

	if (!strlen(cred->username)) return -1;
	if (!strlen(cred->password)) generateRandomPassword(cred->password);
	
	// Initialize user data
	user_t* user = calloc(1, sizeof(user_t));
	strlcpy(user->name, cred->username, sizeof(user->name));
	user->id = id++;
	user->permissions = DEFAULT_USER_PERMISSIONS;

	// Create the user's directory
	char buf[32];
	snprintf(buf, sizeof(buf), "users/%d/", user->id);

	if (mkdir(buf, 0777)) {
		perror("mkdir()");
	}

	// Push the user info into the registry vector
	pthread_mutex_lock(&userLock);
	vector_push(&users, &user);
	pthread_mutex_unlock(&userLock);

	// Generate a random salt
	registryGenerateSalt(&cred->salt);
	
	// Generate the hash
	hash_t hash;
	registryGenerateHash(cred, &hash);

	// Write name
	snprintf(buf, sizeof(buf), "users/%d/name.txt", user->id);
	FILE* f = fopen(buf, "w");
	fprintf(f, "%s", user->name);
	fclose(f);

	// Write salt
	snprintf(buf, sizeof(buf), "users/%d/salt", user->id);
	f = fopen(buf, "wb");
	fwrite(&cred->salt, sizeof(salt_t), 1, f);
	fclose(f);

	// Write hash
	snprintf(buf, sizeof(buf), "users/%d/hash", user->id);
	f = fopen(buf, "wb");
	fwrite(&hash, sizeof(hash), 1, f);
	fclose(f);

	// Create stderr directory
	snprintf(buf, sizeof(buf), "users/%d/stderr/", user->id);
	mkdir(buf, 0777);

    return ret;
}

int findUserByName(const void* member, const void* test) {
	const user_t** pi = (const user_t**) member;
	return !strcmp((*pi)->name, (const char*)test);
}

int registryRemoveUser(const char* pname) {

	int ret = 0;

	pthread_mutex_lock(&userLock);

	int index = vector_find(&users, findUserByName, (void*)pname, NULL);
	if (index == -1) {
		ret = 1;
		goto end;
	}

	vector_remove(&users, index);

end:
	pthread_mutex_unlock(&userLock);

	return ret;
}

int registryValidateLogin(credentials_t* cred) {
	char buf[128];
	hash_t hash;
	hash_t realHash;

	// Make sure the user name is registered
	int id = registryGetUserId(cred->username);
	if (id == -1) return 0;

	// Read in the user's salt
	snprintf(buf, sizeof(buf), "users/%d/salt", id);
	FILE* f = fopen(buf, "rb");
	if (!f) {
		perror("fopen()");
		return 0;
	}
	fread(&cred->salt, sizeof(salt_t), 1, f);
	fclose(f);

	// Generate the hash for the credentials
	registryGenerateHash(cred, &hash);

	// Read in the real hash for the user
	snprintf(buf, sizeof(buf), "users/%d/hash", id);
	f = fopen(buf, "rb");
	if (!f) {
		perror("fopen()");
		return 0;
	}
	fread(&realHash, sizeof(hash_t), 1, f);
	fclose(f);

	// Return whether the hashes are the same
	return !bcmp(&hash, &realHash, sizeof(hash_t));
}

int findUser(const void* m, const void* d) {
	const user_t** u = (const user_t**) m;
	int id = (int)(long long) d;

	return (*u)->id == id;
}

user_t* registryGetUser(int id) {
	pthread_mutex_lock(&userLock);

	user_t* user = NULL;
	vector_find(&users, findUser, (void*)(long long) id, &user);

	pthread_mutex_unlock(&userLock);

	return user;
}

int findUserId(const void* d, const void* ldata) {
    user_t** pi = (user_t**) d;
    return !strcmp((*pi)->name, (char*) ldata);
}

int registryGetUserId(const char* pname) {
    pthread_mutex_lock(&userLock);

    user_t* pi;
	int found = vector_find(&users, findUserId, pname, &pi);

    pthread_mutex_unlock(&userLock);

	if (found != -1) return pi->id;
	else return -1;
}

user_t* registryGetUserAt(size_t index) {
	user_t* user;
	pthread_mutex_lock(&userLock);
	vector_get(&users, index, &user);
	pthread_mutex_unlock(&userLock);
	return user;
}

int registryAddPlayer(int userId) {
	int ret = 0;

	pthread_mutex_lock(&userLock);

	user_t* user;
	int i = vector_find(&users, findUser, (void*)(long long) userId, &user);

	if (i < 0) {
		ret = -1;
		goto end;
	}

	// If the user is already a player return
	if (user->isPlayer) {
		goto end;
	}

	user->isPlayer = 1;
	user->score = 0;
	user->penalty = 0;

	vector_push(&players, &user);

end:
	pthread_mutex_unlock(&userLock);

	return ret;
}

int registryGetPlayerCount() {
	return players.size;
}

void writePlayerAsJSON(void* _u, void* f) {
	user_t* user = *((user_t**)_u);
	
	fprintf(f, "<tr>\
				<td>%s<td> \
				<td>%d<td> \
				<td>%d<td> \
				</tr>\n",
	user->name, user->score, user->penalty);
}

void registryRankPlayers() {

	pthread_mutex_lock(&userLock);

	for (int i = 0; i < (int)players.size - 1; ++i) {
		user_t* u1 = NULL;
		user_t* u2 = NULL;

		// Get the users
		vector_get(&players, i, &u1);
		vector_get(&players, i + 1, &u2);

		// puts("()");

		// If the topmost score is less, swap
		if (u1->score < u2->score) {
			vector_set(&players, i, &u2);
			vector_set(&players, i + 1, &u1);

			for (int j = i - 1; j >= 0; ++j) {
				// Get the ids
				vector_get(&players, j, &u1);
				vector_get(&players, j + 1, &u2);

				if (u1->score < u2->score) {
					vector_set(&players, j, &u2);
					vector_set(&players, j + 1, &u1);
				}
				else break;
			}
		}

	}

	pthread_mutex_unlock(&userLock);

	FILE* f = fopen("html/scoreboard.json", "w+");
	fprintf(f, "<table>");

	fprintf(f, "<tr> \
	<th>Name</th> \
	<th>Score</th> \
	<th>Penalty</th> \
	</tr>\n");

	vector_foreach(&players, writePlayerAsJSON, f);

	fprintf(f, "</table>\n");
	fclose(f);

}

int registryGetPlayerProgram(char* buf, size_t bufSize, int playerId) {
	user_t* u = registryGetUser(playerId);

	snprintf(buf, bufSize, "users/%d/prog.%s", playerId, config.languages[u->lang].runExtension);

	return 0;
}

int registryGetUserDirectory(char* buf, size_t bufSize, int userId) {
	size_t bytesToWrite = snprintf(buf, bufSize, "users/%d/", userId);
	return bytesToWrite >= bufSize;
}

int registryLoadGameResults(int gameId) {
	char buf[64];
	snprintf(buf, sizeof(buf), "games/%d/results.txt", gameId);

	printf("BUF = %s\n", buf);

	FILE* f = fopen(buf, "r");
	if (!f) {
		return -1;
	}

	int playerCount = 0;
	fscanf(f, "%d", &playerCount);

	printf("PLAYER COUNT = %d\n", playerCount);

	for (int i = 0; i < playerCount; ++i) {
		int playerId, score, penalty;

		fscanf(f, "%d %d %d", &playerId, &score, &penalty);

		user_t* user = registryGetUser(playerId);
		if (!user) continue;

		pthread_mutex_lock(&userLock);

		user->score += score;
		user->penalty += penalty;

		pthread_mutex_unlock(&userLock);
	}	

	return 0;
}

void registryClear() {
	pthread_mutex_lock(&userLock);
	vector_clear(&users);
	vector_clear(&players);
	id = 0;
	pthread_mutex_unlock(&userLock);
}

void writeUser(const void* _src, FILE* f) {
	const user_t** src = (const user_t**) _src;

	fwrite(*src, sizeof(user_t), 1, f);
}

int registryClose() {
	int ret = 0;

    FILE* f = fopen("state/registry.dat", "wb");

	if (!f) {
		ret = -1;
	}
	else {
		vector_write(&users, writeUser, f);
		fwrite(&id, sizeof(id), 1, f);
		fwrite(&currentPlayerId, sizeof(currentPlayerId), 1, f);
		fclose(f);
	}

	vector_free(&users);
	vector_free(&players);

    return ret;
}