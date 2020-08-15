#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <bsd/string.h>
#include <sys/stat.h>

#include "utility/registry.h"
#include "utility/config.h"
#include "builder.h"

int running = 0;

//////////////////////////////////
// 			Commands			//
//////////////////////////////////

int cmd_help(char* args);
int cmd_shutdown(char* args);
int cmd_register(char* args);
int cmd_listusers(char* args);
int cmd_removeuser(char* args);
int cmd_reset(char* args);
int cmd_build(char* args);
int cmd_addplayer(char* args);
int cmd_exec(char* args);

void execCommand(char* line);

struct command_t
{
	const char* name;
	int (*func)(char* args);
	const char* usage;
	const char* help;
};

struct command_t commands[] = {
	{ "help", 			cmd_help, 			"", 								"displays this help page" },
	{ "init", 			NULL, 				"", 								"Not supported" },
	{ "shutdown", 		cmd_shutdown, 		"", 								"shutdowns this process and all children" },
	{ "register", 		cmd_register, 		"user_name1 [user_name2] [...]", 	"registers new users" },
	{ "listusers", 		cmd_listusers, 		"", 								"lists all users in the registry" },
	{ "remove", 		cmd_removeuser, 	"user_name1 [user_name2] [...]", 	"removes currently registered users" },
	{ "reset", 			cmd_reset, 			"section1 [section2] [...]",		"resets directories to be empty" },
	{ "build",			cmd_build,			"userid filepath",					"builds a file into the submission system" },
	{ "addplayer",		cmd_addplayer,		"addplayer [player1] [...]",		"makes an user a player" },
	{ "exec",			cmd_exec,			"",									"executes a file" },
};

int cmd_help(char* args) {
	if (args) {
		printf("notice: shutdown does not take any arguments\n");
	}

	int commandCount = (sizeof(commands) / sizeof(struct command_t));

	printf("Number registered commands: %d\n", commandCount);

	const char* fmt = "\t%-20s %-40s %s\n";

	printf(fmt, "Command", "Description", "Usage");

	for (int i = 0; i < commandCount; ++i) {
		printf(fmt, commands[i].name, commands[i].help, commands[i].usage);
	}

	return 0;
}

int cmd_register(char* args) {
	char* bookmark;

	FILE* f = fopen("passwords.txt", "a");

	credentials_t rego = { 0 };

	char *name = strtok_r(args, " ", &bookmark);

	while (name) {

		strlcpy(rego.username, name, sizeof(rego.username));
		rego.password[0] = '\0';

		printf("Registering %s...", name);
		int r = registryRegisterUser(&rego);
		
		if (!r) {
			printf("success\n");
			fprintf(f, "%s %s\n", rego.username, rego.password);
		}
		else printf("failure\n");

		name = strtok_r(NULL, " ", &bookmark);
	}

	fclose(f);

	printf("\n%d of %d user slots filled\n", registryGetUserCount(), MAX_USERS);

	return 0;
}

int cmd_shutdown(char* args) {
	if (args) {
		printf("notice: shutdown does not take any arguments\n");
	}

	running = 0;

	return 0;
}

int cmd_listusers(char* args) {
	if (args) {
		printf("notice: listusers does not take any arguments\n");
	}

	int n = registryGetUserCount();

	printf("Number of Users: %d\n", n);

	printf("ID\tScore\tName\n");

	for (int i = 0; i < n; ++i) {
		user_t* p = registryGetUserAt(i);
		if (!p) continue;

		printf("%d\t%d\t%s\n", p->id, p->score, p->name);
	}

	return 0;
}

int cmd_removeuser(char* args) {
	char* bookmark;

	char *name = strtok_r(args, " ", &bookmark);

	while (name) {

		printf("removing %s...", name);
		int r = registryRemoveUser(name);
		
		if (!r) printf("success\n");
		else printf("failure\n");

		name = strtok_r(NULL, " ", &bookmark);
	}

	printf("\n%d of %d user slots filled\n", registryGetUserCount(), MAX_USERS);

	return 0;
}

int cmd_reset(char* args) {
	if (args) {}

	char ans[4];
	
	printf("Are you sure you want to reset the current environment? [y/n]\n");

	fgets(ans, sizeof(ans), stdin);

	char* r = strchr(ans, '\r');
	char* n = strchr(ans, '\n');
	
	if (r) *r = '\0';
	if (n) *n = '\0';

	if (!strcmp(ans, "yes") || !strcmp(ans, "Yes") || !strcmp(ans, "Y") || !strcmp(ans, "y")) {}
	else if (!strcmp(ans, "no") || !strcmp(ans, "No") || !strcmp(ans, "N") || !strcmp(ans, "n")) {
		goto skip_reset;
	}
	else {
		puts("Answer unclear, skipping just in case");
		goto skip_reset;
	}

	system("rm -r users");
	system("mkdir users");
	system("rm passwords.txt");
	registryClear();

	system("rm -r state");
	system("mkdir state");

	system("rm -r games");
	system("mkdir games");

	system("rm -r submissions");
	system("mkdir submissions");

	exit(0);

skip_reset:
	return 0;
}

int cmd_build(char* args) {

	char filepath[64];
	int userId;

	// Get the user id and filepath
	if (sscanf(args, "%d %63s", &userId, filepath) != 2) {
		puts("Not enough argumets provided");
		return -1;
	}

	// Get the file data
	struct stat s;
	if (stat(filepath, &s)) {
		puts("File not found");
		return -1;
	}

	char* src = calloc(1, s.st_size + 1);
	FILE* f = fopen(filepath, "r");
	fread(src, 1, s.st_size, f);
	fclose(f);

	// Get the language
	char* extension = strrchr(filepath, '.') + 1;
	int lang = -1;

	for (int i = 0; i < LANGUAGE_COUNT; ++i) {
		if (!strcmp(extension, config.languages[i].extension)) {
			lang = i;
		}
	}

	if (lang == -1) return -1;

	if (build(lang, src, userId)) {
		puts("Build unsuccessful");
	}
	else {
		puts("Build successful");
	}

	free(src);

	return 0;
}

int cmd_addplayer(char* args) {
	char* bookmark;

	if (args == NULL) return 0;

	char* id_s = strtok_r(args, " ", &bookmark);

	while (id_s) {

		int id = atoi(id_s);

		int r = registryAddPlayer(id);

		printf("Making user %d a player...%s\n", id, r ? "failed" : "success" );

		id_s = strtok_r(NULL, " ", &bookmark);
	}

	return 0;
}

int cmd_exec(char* args) {
	char* bookmark;

	if (args == NULL) return 0;

	char* filename = strtok_r(args, " ", &bookmark);

	while (filename) {

		printf("Executing %s...\n", filename);
		fflush(stdout);

		FILE* f = fopen(filename, "r");
		if (f) {
			char buf[256];

			while (fgets(buf, sizeof(buf), f)) {
				if (strlen(buf) <= 2) continue;
				execCommand(buf);
			}

			fclose(f);
		}
		else {
			puts("failed");
		}

		filename = strtok_r(NULL, " ", &bookmark);
	}

	return 0;
}

void execCommand(char* line) {

	char* bookmark = NULL;

	char* cmd = strtok_r(line, " \t\r\n", &bookmark);
	char* args = strtok_r(NULL, "\r\n", &bookmark);

	for (unsigned int i = 0; i < (sizeof(commands) / sizeof(struct command_t)); ++i) {
		if (!strcmp(cmd, commands[i].name)) {
			if (commands[i].func) {
				if (commands[i].func(args)) {
					printf("%s failed\n", cmd);
				}
			}
			else {
				printf("%s is not implemented\n", cmd);
			}
			return;
		}
	}

	printf("invalid command: %s\n", cmd);
}

void prompt() {
	running = 1;
	while (running) {
		printf("admin > ");
		fflush(stdout);

		char cmd[256];
		if (!fgets(cmd, sizeof(cmd), stdin)) {
			break;
		}

		execCommand(cmd);
	}
}

void adminInit() {}

int adminRun() {
	prompt();
	return 0;
}

int adminClose() {
	running = 0;
	return 0;
}