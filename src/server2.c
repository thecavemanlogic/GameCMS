#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bsd/string.h>
#include <sys/stat.h>

#include "server.h"
#include "net/tcp_server.h"
#include "utility/registry.h"
#include "utility/config.h"
#include "builder.h"
#include "net/http.h"

// The list of currently active sessions
// TODO: make a dictionary instead
vector_t sessions;
pthread_mutex_t sessionLock = PTHREAD_MUTEX_INITIALIZER;

tcp_server_t* server;

// The valid characters that we can use to build a session id
const char* cookieChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

int findSession(const void* _s, const void* _test) {
    session_t* s = *((session_t**) _s);

    printf("s->id = %d\n", s->id);
    printf("s->cookie = %s\n", s->cookie);


    if (!s) return 0;
    return !strcmp(s->cookie, _test);
}

// Generates 
void generateRandomSessionCookie(session_t* s) {
    char* c;
    int found;

generate:
    c = s->cookie;
    for (int i = 0; i < SESSION_COOKIE_SIZE; ++i, ++c) {
        *c = cookieChars[rand() % sizeof(cookieChars)];
    }
    *c = '\0';

    // Find a matching session
    pthread_mutex_lock(&sessionLock);
    found = vector_find(&sessions, findSession, c, NULL);
    pthread_mutex_unlock(&sessionLock);

    if (found >= 0) goto generate;
}

// Initializes a session struct with a new session id.
// TODO: check if the generated session id already exists
void initSession(session_t* s, int userId) {
    generateRandomSessionCookie(s);
    s->timeout = 0;
    s->data = NULL;
    s->id = userId;
}

// Redirects all users that attempt to go to "/"
// to the login page
void get_root(FILE* f, http_request_t* req) {
    if (req) {}

    sendHttpRedirect(f, "/login");
}

void get_replay(FILE* f, http_request_t* req) {

    int gameId = atoi(strlen("game=") + req->query);

    char buf[64];
    snprintf(buf, sizeof(buf), "games/%d/replay.bin", gameId);
    
    sendHttpFile(f, buf);
}

void get_renderer(FILE* f, http_request_t* req) {
    if (req) {}

    sendHttpFile(f, "html/renderer.js");
}

// Takes <payload> and parses the credentials located inside
// and checks whether the they are valid.
int validateLogin(char* payload, char* name) {
	char* username = NULL;
	char* password = NULL;
	char* bookmark = NULL;

	strtok_r(payload, "=", &bookmark);
	username = strtok_r(NULL, "\r\n", &bookmark);
	strtok_r(NULL, "\r\n=", &bookmark);
	password = strtok_r(NULL, "\r\n", &bookmark);

	credentials_t ro = { 0 };
    strlcpy(ro.username, username, sizeof(ro.username));
    strlcpy(ro.password, password, sizeof(ro.password));

	int r = registryValidateLogin(&ro);

    printf("%s %s logged in\n", username, r ? "successfully" : "failed to");

    if (name) strlcpy(name, username, MAX_USER_NAME_LENGTH);

	return r;
}

// Checks if the loggin information is correct, then,
// it redirects the client to the specified page
void post_login(FILE* f, http_request_t* req) {
    char name[MAX_USER_NAME_LENGTH];
    if (validateLogin(req->payload, name)) {
        int userId = registryGetUserId(name);

        // Create a new session
        session_t* s = malloc(sizeof(session_t));
        initSession(s, userId);
        pthread_mutex_lock(&sessionLock);
        vector_push(&sessions, &s);
        pthread_mutex_unlock(&sessionLock);

        // printf("user id = %d\n", userId);
        // printf("s->id = %d\n", s->id);

        // puts("Checking the vector_t:\n");
        session_t* ss;
        vector_get(&sessions, 0, &ss);
        // printf("ss->id = %d\n", ss->id);
        // printf("ss->cookie = %s\n", ss->cookie);
        // printf("ss->timeout = %ld\n", ss->timeout);
        // printf("ss->data = %p\n", ss->data);

        // Reuse the name buf to convert id into a string
        snprintf(name, sizeof(name), "%d", userId);

        http_response_t res;
        initHttpResponse(&res);
        httpAddCookie(&res, "SessionId", s->cookie);
        httpAddCookie(&res, "UserId", name);
        httpRedirectTo(&res, "/submit");
        sendHttpResponse(&res, f);
        freeHttpResponse(&res);
    }
    else {
        sendHttpRedirect(f, "html/login.html");
    }
}

// 
void post_register(FILE* f, http_request_t* req) {
    if (req) {}

    sendHttpFile(f, "html/404.html");
}

// Builds the source code located in <req>
// and notifies the client of the build status
void post_submit(FILE* f, http_request_t* req) {
    // fprintf(f, "HTTP/1.1 200 OK\r\n");
    
    char buf[64];
    char* xstatus = "Good";
    char* badstatus = "Bad";

    // Make sure the language header is present
    int lang;
    const char* lang_s = getHttpHeader(req, "Language", buf, sizeof(buf));
    if (lang_s) lang = atoi(lang_s);
    else {
        xstatus = badstatus;
        goto end;
    }

    // Make sure the UserId cookie is present
    int userId;
    const char* user_s = getHttpCookie(req, "UserId", buf, sizeof(buf));
    if (user_s) userId = atoi(user_s);
    else {
        xstatus = badstatus;
        goto end;
    }

    if (build(lang, req->payload, userId)) {
        xstatus = badstatus;
    }
    else {
        registryAddPlayer(userId);
    }

    http_response_t res;

end:
    
    initHttpResponse(&res);
    httpAddHeader(&res, "X-Status", xstatus);

    snprintf(buf, sizeof(buf), "users/%d/error.txt", userId);

    struct stat s;
    if (!stat(buf, &s)) {
        httpAddPayloadFile(&res, buf);
    }

    sendHttpResponse(&res, f);
    freeHttpResponse(&res);

    fflush(f);
}

void post_ask(FILE* f, http_request_t* req) {
    if (req) {}

    sendHttpFile(f, "html/404.html");
}

// List of registered responses to requests
response_type_t pages[] = {
    { "GET",    "/",                    0,  get_root },
    // { "GET",    "/login",               0,  NULL },
    // { "GET",    "/register",            0,  NULL },
    // { "GET",    "/404",                 0,  NULL },
    // { "GET",    "/submit",              1,  NULL },
    { "GET",    "/replay",              0,  get_replay },
    // { "GET",    "/replay_page",         1,  NULL },
    // { "GET",    "/questions",           1,  NULL },
    // { "GET",    "/rankings",            1,  NULL },
    // { "GET",    "/viewGame",            1,  NULL },
    { "GET",    "/rockpaperscissors",   1,  get_renderer },

    { "POST",   "/login",               0,  post_login },
    { "POST",   "/register",            0,  post_register },
    { "POST",   "/submit",              1,  post_submit },
    { "POST",   "/ask",                 1,  post_ask },
};

// Returns 1 if the request is authorized, 0 if not
int authenticate(const http_request_t* req) {
    char buf[64];

    // Get the SessionId cookie and return an error
    // if it is not found
    const char* c = getHttpCookie(req, "SessionId", buf, sizeof(buf));
    if (!c) {
        printf("No session id found\n");
        return 0;
    }

    // Find a matching session
    session_t* s = NULL;
    pthread_mutex_lock(&sessionLock);
    vector_find(&sessions, findSession, c, &s);
    pthread_mutex_unlock(&sessionLock);

    // Return if no session was found
    if (!s) {
        printf("Invalid session\n");
        return 0;
    }

    // Get the UserId cookie and return an error
    // if it is not found
    const char* ids = getHttpCookie(req, "UserId", buf, sizeof(buf));
    if (!ids) {
        printf("Invalid user id\n");
        return 0;
    }

    // Return whether the ids match
    return atoi(ids) == s->id;
}

// The function to be called whenever the server receives a new request
int onMessage(FILE* f, void* data) {
    http_request_t req = { 0 };

    if (data) {}

    // Read the HTTP request, close the connection if it is invalid
    if (readHttpRequest(f, &req)) {
        freeHttpRequest(&req);
        return -1;
    }

    // Find the correct page
    response_type_t* page = NULL;
    for (size_t i = 0; i < sizeof(pages) / sizeof(response_type_t); ++i) {

        // Find the correct page
        if (strcmp(req.verb, pages[i].verb)) continue;
        if (strcmp(req.path, pages[i].path)) continue;

        page = &pages[i];

        break;
    }

    printf("%s %s\n", req.verb, req.path);

    if (page) {
        // See if we need to be authenticated, if so, attempt to
        // do so and if it fails redirect the client to the
        // login page
        if (page->needsAuthentication && !authenticate(&req)) {
            printf("failed to authenticate... redirecting\n");
            sendHttpRedirect(f, "/login");
            goto end;
        }

        page->action(f, &req);
    }
    else {
        char buf[64];
        snprintf(buf, sizeof(buf), "html/%s", req.path);
        sendHttpFile(f, buf);
    }

   

end:

    freeHttpRequest(&req);

    return 0;
}

int serverInit() {

    vector_init(&sessions, sizeof(session_t*), free);

    server = createTcpServer(8080);

    client_handlers_t handlers = { 0 };
    handlers.onMessage = onMessage;

    tcpAddEventListeners(server, &handlers);

    tcpServerListen(server, 1);

    return 0;
}

// The function to be called when the server is to be shutdown
int serverClose() {
    destroyTcpServer(server);
    vector_free(&sessions);

    if (builderClose()) {
        fprintf(stderr, "Warning: could not save builder script\n");
    }

    return 0;
}