#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include <pthread.h>

#define DEFAULT_PORT 8080

#define DEFAULT_MAX_CLIENTS 60

#include "../dstruct/vector.h"

typedef struct {
    vector_t onConnect;
    vector_t onMessage;
    vector_t onClose;
} protocol_stack_t;

typedef struct {
    void* (*onConnect)(FILE* file);
    int  (*onMessage)(FILE* file, void* data);
    void  (*onClose)(void* data);
} client_handlers_t;

typedef struct {
    int fd;
    int port;
    int async;
    pthread_t thread;
    vector_t clients;
    pthread_mutex_t clientLock;
    size_t maxClients;
    client_handlers_t handlers;
} tcp_server_t;

typedef struct {
    int id;
    int sock;
    int running;
    pthread_t thread;
    tcp_server_t* server;
    // protocol_stack_t pstack;
    client_handlers_t* handlers;
} tcp_client_t;

// Description
// 	
// Parameters
// 	
// Returns
// 	
tcp_server_t* createTcpServer(int port);

// Description
// 	
// Parameters
// 	
// Returns
// 	
int tcpAddEventListeners(tcp_server_t* server, client_handlers_t* handlers);

// Description
// 	
// Parameters
// 	
// Returns
// 	
int tcpServerListen(tcp_server_t* server, int async);

// Description
// 	
// Parameters
// 	
// Returns
// 	
int tcpSendFile(FILE* sock, const char* filepath);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void destroyTcpServer(tcp_server_t* server);

#endif