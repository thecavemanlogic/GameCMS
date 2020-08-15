// File: 
// Author: Caedmon M. Evans
// Description:
//  

#ifndef _SERVER_H
#define _SERVER_H

#include "net/http.h"

// The number of characters - 1 that a session cookie will use
#define SESSION_COOKIE_SIZE 16

// A data structure to store a valid and currently active session
typedef struct {

    // The player id the session is linked to
    int id;
    
    // The session id. The '+ 1' includes the NULL terminator
    char cookie[SESSION_COOKIE_SIZE + 1];

    // The timestamp of when this session should expire
    time_t timeout;

    // Any misc. data that we need to store about this session
    void* data;

} session_t;

// A generic struct to help simplify the navigation process
// for finding files
typedef struct {
    
    // The HTTP verb, either GET or POST
    const char* verb;

    // THE HTTP path
    const char* path;

    // A boolean, whether the client needs a valid session id
    // to access the requested path
    const int needsAuthentication;

    // The function to call if the client authenticated. If this
    // value is null, then treat the path as a static webpage by placing
    // it into this format specifier "html/{path}.html"
    void (*action)(FILE* f, http_request_t* req);

} response_type_t;

// Description
// 	
// Parameters
// 	
// Returns
// 	
int serverInit();

// Description
// 	
// Parameters
// 	
// Returns
// 	
int serverClose();

#endif