#include <stdio.h>
#include <string.h>

#include "config.h"

config_t config = {
	.languages = {
		{	// C - 0
			.name = "C",
			.shorthand = "c",
			.extension = "c",
			.runExtension = "out",
			.buildCommand = "gcc %s -o %scode.out",
			.runCommand = "%s.out",
			.timeout = 20,
		},
		{	// C++ - 1
			.name = "C++",
			.shorthand = "c++",
			.extension = "cpp",
			.runExtension = "out",
			.buildCommand = "g++ %s -o %scode.out",
			.runCommand = "%s.out",
			.timeout = 20,
		},
		{	// Python - 2
			.name = "Python",
			.shorthand = "python",
			.extension = "py",
			.runExtension = "py",
			.buildCommand = "cp %s %scode.py",
			.runCommand = "/usr/bin/python3 %s.py",
			.timeout = 400,
		},
		{	// Java - 3
			.name = "Java",
			.shorthand = "java",
			.extension = "java",
			.runExtension = "class",
			.buildCommand = "javac -cp %s -d %s",
			.runCommand = "/usr/bin/java %s.class",
			.timeout = 20,
		},
		{	// C# - 4
			.name = "C#",
			.shorthand = "c#",
			.extension = "cs",
			.runExtension = "",
			.buildCommand = "",
			.runCommand = "",
			.timeout = 20,
		},
		{	// Javascript - 5
			.name = "Javascript",
			.shorthand = "js",
			.extension = "js",
			.runExtension = "",
			.buildCommand = "",
			.runCommand = "",
			.timeout = 40,
		},
	},

	.system = {
		.root = ".",
	},
	.server = {
		.useHttps = 0,
		.maxClients = 64,
		.maxPayloadSize = -1,
		.maxRequestsPerMinute = -1,
		.maxHeaderCount = -1,
		.maxCookieCount = -1,
		.root = "",
	},
	.game = {
		.minPlayers = 2,
		.maxPlayers = 2,
		.count = 64,
	},
};

int loadConfig(const char* filename) {

	if (!filename) filename = "config.dat";

	FILE* f = fopen(filename, "r");
	if (!f) return -1;

	fread(&config, sizeof(config_t), 1, f);

	fclose(f);

	return 0;
}

lang_config_t* getLangAddress(const char* lang) {
	for (unsigned int i = 0; i < LANGUAGE_COUNT; ++i) {
		if (!strcmp(lang, config.languages[i].shorthand)) {
			return &config.languages[i];
		}
	}

	return NULL;
}