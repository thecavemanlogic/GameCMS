#include <stdio.h>
#include <string.h>

#include "configreader.h"

FILE* file = NULL;
int line = 0;
char buf[256];

int startReading(const char* filepath) {
	file = fopen(filepath, "r");
	line = 0;

	if (file) return 0;
	else return -1;
}

int nextLine(char** tag, char** value) {
	if (!file) return -1;
	buf[0] = ' ';
	char* equalSignPosition;
	char* bookmark = NULL;

readline:
	++line;
	if (!fgets(buf, sizeof(buf), file)) return -1;
	if (strlen(buf) == 2) goto readline;

	// Strips out any comments
	char* code;
	if (buf[0] == '#') goto readline;
	code = strtok_r(buf, "#", &bookmark);

	equalSignPosition = strchr(code, '=');

	char* t = strtok_r(code, "\t =\r\n", &bookmark);
	// printf("code = %p\tbookmark = %p\n", code, bookmark);
	char* v = NULL;
	if (strchr(bookmark + 1, '"')) {
		v = strtok_r(NULL, "\"", &bookmark);
		v = strtok_r(NULL, "\"", &bookmark);
	}
	else {
		v = strtok_r(NULL, " =\t\r\n", &bookmark);
	}
	
	// if comment or blank line
	if (!t && !v) goto readline;
	// If value is missing
	else if (!v) {
		fprintf(stderr, "configreader: error, value missing on line %d\n", line);
		return line;
	}
	// If the equal sign is not in the
	// correct position
	else if (equalSignPosition < t || equalSignPosition > v) {
		fprintf(stderr, "configreader: error, equal sign not inbetween tag and value on line %d\n", line);
		return line;
	}

	*tag = t;
	*value = v;

	return 0;
}

void closeReader() {
	fclose(file);
	file = NULL;
	line = 0;
}