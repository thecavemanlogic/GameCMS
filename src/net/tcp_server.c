#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#include "tcp_server.h"

void* clientTask(void* _client) {
	tcp_client_t* client = (tcp_client_t*) _client;

	FILE* sock = fdopen(client->sock, "r+");

	client_handlers_t* handlers = &client->server->handlers;

	void* data = NULL;

	if (handlers->onConnect) data = handlers->onConnect(sock);

	if (!handlers->onMessage) {
		fprintf(stderr, "Error: onMessage is an invalid function!\n");
		return NULL;
	}

	while (1) {

		if (ferror(sock) || feof(sock)) break;

		if (handlers->onMessage(sock, data)) break;
	}

	if (handlers->onClose) handlers->onClose(data);

	fclose(sock);
	client->sock = -1;

	pthread_mutex_lock(&client->server->clientLock);
	client->running = 0;
	pthread_mutex_unlock(&client->server->clientLock);
	
	// puts("Client exited\n");

	return NULL;
}

int removeClientIfClosed(void* _client, void* ldata) {

	if (ldata) {}

	tcp_client_t* client = *((tcp_client_t**) _client);
	return (client->running == 0);
}

void* serverTask(void* data) {
    tcp_server_t* server = (tcp_server_t*)data;

	server->fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 0;
    struct sockaddr_in address; 

	if (setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt()");
		return NULL;
	}

	address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons((server->port) ? server->port : DEFAULT_PORT);

	if (bind(server->fd, (const struct sockaddr*)(&address), sizeof(address))) {
		perror("bind()");
		shutdown(server->fd, SHUT_RDWR);
		server->fd = 0;
		return NULL;
	}

	if (listen(server->fd, 3) < 0) 
    { 
        perror("listen"); 
        return NULL;
    }

	struct sockaddr clientsock = { 0 };
	socklen_t socklen = sizeof(clientsock);

	int clientfd;
	int clientid = 0;
	while ((clientfd = accept(server->fd, &clientsock, &socklen)) >= 0) {

        if (server->clients.size < server->maxClients) {

			// printf("New client added\n");

            tcp_client_t* client = calloc(sizeof(tcp_client_t), 1);

            client->id = ++clientid;
            client->running = 1;
            client->server = server;
            client->sock = clientfd;
			client->handlers = &server->handlers;

            pthread_mutex_lock(&server->clientLock);
			vector_push(&server->clients, &client);
			pthread_mutex_unlock(&server->clientLock);

			pthread_create(&client->thread, NULL, clientTask, client);
        }
		else {
			close(clientfd);
		}

		pthread_mutex_lock(&server->clientLock);
		vector_removeif(&server->clients, removeClientIfClosed, NULL);
		pthread_mutex_unlock(&server->clientLock);
	}

	if (clientfd < 0) {
		perror("accept()");
	}

	// close(server->fd);

	// printf("Server shutdown\n");

	return NULL;
}

void clientClosePolicy(void* _client) {
	tcp_client_t* client = *((tcp_client_t**) _client);
	// tcp_client_t* client = (tcp_client_t*) _client;

	// puts("Shutting client down...\n");

	if (client->sock != -1) shutdown(client->sock, SHUT_RDWR);

	pthread_join(client->thread, NULL);

	free(client);
}

tcp_server_t* createTcpServer(int port) {

    tcp_server_t* server = calloc(sizeof(tcp_server_t), 1);

	server->port = port;

    pthread_mutex_init(&server->clientLock, NULL);
    server->maxClients = DEFAULT_MAX_CLIENTS;

	vector_init(&server->clients, sizeof(tcp_client_t*), clientClosePolicy);

    return server;
}

int tcpAddEventListeners(tcp_server_t* server, client_handlers_t* handlers) {
	server->handlers = *handlers;
	return 0;
}

int tcpServerListen(tcp_server_t* server, int async) {

	int r = pthread_create(&server->thread, NULL, serverTask, (void*)server);
	// serverTask(server);

	if (r) {
        perror("pthread_create(): failed to create server thread");
		return -1;
    }

	if (!async) pthread_join(server->thread, NULL);

	server->async = async;

	return 0;
}

void destroyTcpServer(tcp_server_t* server) {
	if (!server) return;

	// Wonderful shutdown() function found here:
	// https://www.gnu.org/software/libc/manual/html_node/Closing-a-Socket.html
	if (server->fd) {
		shutdown(server->fd, SHUT_RDWR);
		close(server->fd);
		server->fd = 0;
	}

	// vector_foreach(&server->clients, closeClient);
	vector_clear(&server->clients);

	// vector_free(&server->cleanup);
	vector_free(&server->clients);

	if (server->async) pthread_join(server->thread, NULL);

	free(server);

	return;
}

int tcpSendFile(FILE* sock, const char* filepath) {
    struct stat buf;

    if (stat(filepath, &buf)) {
		return -1;
	}

    FILE* f = fopen(filepath, "r");

    size_t sz = buf.st_size;
    char mbuf[4096];
    while (sz > sizeof(mbuf)) {
        fread(mbuf, sizeof(mbuf), 1, f);
        fwrite(mbuf, sizeof(mbuf), 1, sock);
        sz -= sizeof(mbuf);
    }

    fread(mbuf, sz, 1, f);
    fwrite(mbuf, sz, 1, sock);
    fflush(sock);

    fclose(f);

    return 0;
}