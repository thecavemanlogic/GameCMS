#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bsd/string.h>

#include "config.h"
#include "configreader.h"

//////////////////////////////////////
// 			Language Sector			//
//////////////////////////////////////

void initLangDefault(lang_config_t* langStruct) {
	langStruct->maxMemory = 1048576;	// 1 megabyte
	langStruct->timeout = 50;			// 50 milliseconds
}

void initDefault(config_t* configStruct) {
	for (unsigned int i = 0; i < sizeof(configStruct->languages) / sizeof(lang_config_t); ++i) {
		initLangDefault(&configStruct->languages[i]);
	}
}

void setLangAttr(lang_config_t* lang, const char* attr, const char* value) {
	if (!strcmp(attr, "buildCommand")) {
		strlcpy(lang->buildCommand, value, MAX_CONFIG_VALUE_LENGTH);
	}
	else if (!strcmp(attr, "maxMemory")) {
		lang->maxMemory = atoi(value);
	}
	else if (!strcmp(attr, "timeout")) {
		lang->timeout = atoi(value);
	}
	else {
		fprintf(stderr, "Unknown/unsupported language attribute: %s\n", attr);
		exit(1);
	}
}

int processLang(char* attr, char* value) {
	char* bookmark;

	char* lang = strtok_r(attr, ".", &bookmark);
	attr = strtok_r(NULL, "", &bookmark);

	lang_config_t* langc = getLangAddress(lang);
	if (!langc) {
		fprintf(stderr, "Uknown or unsupported language: %s\n", lang);
		exit(1);
	}

	setLangAttr(langc, attr, value);

	return 0;
}

//////////////////////////////////
// 			Game Sector			//
//////////////////////////////////

int processGame(char* attr, char* value, config_t* configStruct) {

	if (attr && value && configStruct) {}

	return 0;
}

//////////////////////////////////////
// 			System Sector			//
//////////////////////////////////////

int processSystem(char* attr, char* value, config_t* configStruct) {
	int ret = 0;

	if (!strcmp(attr, "root")) {
		strlcpy(configStruct->system.root, value, MAX_CONFIG_VALUE_LENGTH);
	}
	else {
		ret = 1;
	}

	return ret;
}

//////////////////////////////////////
// 			Server Sector			//
//////////////////////////////////////

int processServer(char* attr, char* value, config_t* configStruct) {
	int ret = 0;

	if (!strcmp(attr, "useHttps")) {
		configStruct->server.useHttps = atoi(value);
	}
	else if (!strcmp(attr, "maxClients")) {
		configStruct->server.maxClients = atoi(value);
	}
	else {
		ret = 1;
	}

	return ret;
}

typedef struct {
	const char* sector;
	int (*func)();
} sectorToFunc;

sectorToFunc mappings[] = {
	{ "system", processSystem },
	{ "game", processGame },
	{ "lang", processLang },
	{ "server", processServer },
};

int compileConfig(const char* configFile, config_t* configStruct) {
	int ret = 0;

	if (startReading(configFile) == -1) {
		ret = -1;
		goto exit;
	}

	char* tag;
	char* value;
	char* bookmark;
	char* sector;
	char* attribute;

	while (!nextLine(&tag, &value)) {

		sector = strtok_r(tag, ".", &bookmark);
		attribute = strtok_r(NULL, "", &bookmark);

		int foundSector = 0;
		for (size_t i = 0; i < sizeof(mappings) / sizeof(sectorToFunc); ++i) {
			if (!strcmp(sector, mappings[i].sector)) {
				if (mappings[i].func(attribute, value, configStruct)) {
					fprintf(stderr, "Call to process %s.%s failed\n", sector, attribute);
					exit(1);
				}
				foundSector = 1;
				break;
			}
		}

		if (!foundSector) {
			fprintf(stderr, "config: unknown sector: '%s'\n", sector);
			exit(1);
		}
	}

exit:
	return ret;
}

config_t _config = {
	.languages = {
		{	// C - 0
			.name = "C",
			.shorthand = "c",
			.extension = "c",
			.buildCommand = "gcc %s -o %scode.out",
		},
		{	// C++ - 1
			.name = "C++",
			.shorthand = "c++",
			.extension = "cpp",
			.buildCommand = "g++ %s -o %scode.out",
		},
		{	// Python - 2
			.name = "Python",
			.shorthand = "python",
			.extension = "py",
			.buildCommand = "cp %s %scode.py",
		},
		{	// Java - 3
			.name = "Java",
			.shorthand = "java",
			.extension = "java",
			.buildCommand = "",
		},
		{	// C# - 4
			.name = "C#",
			.shorthand = "c#",
			.extension = "cs",
			.buildCommand = "",
		},
		{	// Javascript - 5
			.name = "Javascript",
			.shorthand = "js",
			.extension = "js",
			.buildCommand = "",
		},
	},

	.system = {
		.root = ".",
	},
	.server = {
		.useHttps = 0,
		.maxClients = 64,
	},
};

int main(int argc, char** argv) {
	
	char* filename = (char*)"game.config";

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-f") == 0) {
			filename = argv[++i];
		}
		else {
			printf("config: error, invalid option: \"%s\"\n", argv[i]);
			exit(1);
		}
	}

	config = _config;

	initDefault(&config);

	if (compileConfig(filename, &config)) {
		fprintf(stderr, "compileconfig: failed to compile %s\n", filename);
		exit(1);
	}

	FILE* f = fopen("config.dat", "w");
	if (!f) {
		perror("compileconfig");
		exit(1);
	}

	fwrite(&_config, sizeof(_config), 1, f);

	fclose(f);

	return 0;
}