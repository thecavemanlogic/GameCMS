#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main() {

    printf("Test program\n");

    printf("Process ID: %d\n", getpid());
    printf("Parent PID: %d\n", getppid());

    time_t t = time(NULL);
    printf("Current Time: %ld\n", t);
    printf("Time String: %s\n", ctime(&t));

    char buf[256];
    if (!getcwd(buf, sizeof(buf))) {
        perror("getcwd");
    }
    printf("Current Working Directory: %s\n", buf);

    return 0;
}