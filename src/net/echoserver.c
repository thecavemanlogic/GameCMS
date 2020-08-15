#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "tcp_server.h"

void* onConnect(FILE* f) {
    fputs("Welcome!\n", f);
    fflush(f);
    return NULL;
}

int onMessage(FILE* f, void* data) {
    char buf[256] = { 0 };

    if (data) {}

    fgets(buf, sizeof(buf), f);
    fwrite(buf, strlen(buf), 1, f);
    fflush(f);

    return 0;
}

int main() {

    tcp_server_t* server = createTcpServer(9909);
    if (!server) {
        perror("Could not create server!\n");
        exit(1);
    }

    client_handlers_t ch = {
        .onConnect = onConnect,
        .onMessage = onMessage,
        .onClose = NULL
    };

    tcpAddEventListeners(server, &ch);

    tcpServerListen(server, 1);

    sleep(3);

    destroyTcpServer(server);

}