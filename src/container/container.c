#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <bsd/string.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/capability.h>
#include <sys/prctl.h>

#include "container.h"

container_t* createContainer(const char* command) {
    char buf[256];
    strlcpy(buf, command, sizeof(buf));

    int index = 1;
    char* args[16] = { 0 };
    char* bookmark;
    args[0] = strtok_r(buf, " ", &bookmark);
    printf("arg[0] = %s\n", args[0]);
    while ((args[index++] = strtok_r(NULL, " ", &bookmark))) {
        printf("arg[%d] = %s\n", index - 1, args[index - 1]);
    }

    int pid = fork();

    if (pid == 0) {       // If child

        // prctl(PR_SET_NO_NEW_PRIVS, 1);

        // Set the required permission bits
        // cap_t cap = cap_init();

        // printf("Capabilities Mode: %s\n", cap_mode_name(cap_get_mode()));

        // if (cap_set_secbits(CAP_SETPCAP) == -1) {
        //     perror("cap_set_secbits()");
        // }

        // cap_free(cap);

        int flags = 0;

        // if (unshare(flags)) perror("unshare()");
        // else puts("unshare succeeded");

        if (chroot(".")) perror("chroot()");
        else puts("chroot succeeded");

        // getcwd(buf, sizeof(buf));
        // printf("%s\n", buf);

        // FILE* f = fopen("prog", "r");
        // if (f) printf("file exists\n");
        // else fclose(f);

        if (chdir("usr/bin")) {
            perror("chdir()");
        }

        getcwd(buf, sizeof(buf));
        printf("CWD = %s\n", buf);

        if (system("ls")) {
            perror("system(\"ls\")");
        }

        execv(args[0], args);

        perror("execv()");
    }
    else if (pid > 0) {   // If parent
        
    }
    else {              // If error
        
    }

    container_t* c = malloc(sizeof(container_t));
    c->pid = pid;
    c->root = NULL;

    return c;
}

void freeContainer(container_t* container) {
    waitpid(container->pid, NULL, 0);
    free(container);
}