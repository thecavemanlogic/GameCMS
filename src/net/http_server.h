#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

#include "tcp_server.h"

typedef struct {

    tcp_server_t tcp_server;

} http_server_t;

// Description
// 	
// Parameters
// 	
// Returns
// 	
http_server_t* createHttpServer();

#endif