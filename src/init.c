#include <sys/stat.h>

int initDirectory() {

    mkdir("users", 0777);
    mkdir("games", 0777);
    mkdir("errors", 0777);
    mkdir("submissions", 0777);
    mkdir("state", 0777);

    return 0;
}