#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <bsd/string.h>

#include "builder.h"
#include "utility/registry.h"

pthread_mutex_t buildLock = PTHREAD_MUTEX_INITIALIZER;
int buildCount = 0;

int builderInit() {
	FILE* f = fopen("state/builder.dat", "rb");
	if (!f) return 1;

	fread(&buildCount, sizeof(buildCount), 1, f);

	fclose(f);

	return 0;
}

int build(int _lang, const char* src, int playerId) {
	lang_config_t* lang = &config.languages[_lang];

	// Get our build number
	pthread_mutex_lock(&buildLock);
	int buildNumber = buildCount++;
	pthread_mutex_unlock(&buildLock);

	// printf("Language: %s\n", lang->name);
	// printf("Extension: %s\n", lang->extension);

	// Format the submission location
	char submissionLocation[64];
	snprintf(submissionLocation, sizeof(submissionLocation), "submissions/%d.%s", buildNumber, lang->extension);

	// Format the player directory
	char finalLocation[64];
	snprintf(finalLocation, sizeof(finalLocation), "users/%d/", playerId);

	// Format the core build command
	char buildCommand[64];
	snprintf(buildCommand, sizeof(buildCommand), lang->buildCommand, submissionLocation, finalLocation);

	// Format the stderr output file location
	char stderrOutput[64];
	snprintf(stderrOutput, sizeof(stderrOutput), "users/%d/error.txt", playerId);

	// Format the final build command
	char buildCommand2[256];
	snprintf(buildCommand2, sizeof(buildCommand2), "%s 2> %s", buildCommand, stderrOutput);

	// Write the file to disk
	FILE* f = fopen(submissionLocation, "w+");
	fwrite(src, strlen(src), 1, f);
	fclose(f);

	// Update submissions.txt
	strlcat(finalLocation, "submissions.txt", sizeof(finalLocation));
	f = fopen(finalLocation, "a");
	if (!f) {
		perror("build()");
		return -1;
	}
	fprintf(f, "%d\n", buildNumber);
	fclose(f);

	// Update lang.dat
	snprintf(finalLocation, sizeof(finalLocation), "users/%d/lang.txt", playerId);
	f = fopen(finalLocation, "w+");
	fprintf(f, "%d", _lang);
	fclose(f);

	// Compile the file
	int r = WEXITSTATUS(system(buildCommand2));
	// if (!r) {
	// 	registryAddPlayer(playerId);
	// }
	
	return r;
}

int builderClose() {
	FILE* f = fopen("state/builder.dat", "wb");
	if (!f) return 1;

	fwrite(&buildCount, sizeof(buildCount), 1, f);

	fclose(f);

	return 0;
}