#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#include "shutdown.h"

#define MAX_SHUTDOWN_CHILDREN 10

void (*shutdownAction)(void) = NULL;

int children[MAX_SHUTDOWN_CHILDREN];
int childCount = 0;
int alreadyCalled = 0;

void whenSignal(int sig) {
	if (sig) {}

	if (alreadyCalled) return;

	alreadyCalled = 1;

	for (int i = 0; i < childCount; ++i) {
		if (kill(children[i], SIGTERM)) {
			perror("shutdown: kill()");
		}
	}

	for (int i = 0; i < childCount; ++i) {
		waitpid(children[i], NULL, 0);
	}

	if (shutdownAction) shutdownAction();
}

void setupShutdown(void (*action)(void)) {
	shutdownAction = action;

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	sa.sa_handler = whenSignal;

	if (sigaction(SIGTERM, &sa, NULL)) {
		perror("sigaction():");
		exit(1);
	}

	if (sigaction(SIGINT, &sa, NULL)) {
		perror("sigaction():");
		exit(1);
	}
}

void registerChild(int pid) {
	if (childCount >= MAX_SHUTDOWN_CHILDREN) {
		fprintf(stderr, "registerChild: maximum children reached\n");
	}
	children[childCount++] = pid;
}

void shutdownNow() {
	whenSignal(SIGTERM);
}