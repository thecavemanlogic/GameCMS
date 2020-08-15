#include <string.h>
#include <stdlib.h>
#include <bsd/string.h>

#include "http.h"
#include "../dstruct/key_value.h"

// typedef struct {
//     int number;
//     char* status;
// } http_status_t;

const http_status_t httpStatuses[] = {
    { 100, "Continue" },
    { 101, "Switching Protocols" },
    { 102, "Processing" },

    { 200, "OK" },
    { 201, "Created" },
    { 202, "Accepted" },
    { 203, "Non-Authorized Information" },
    { 204, "No Content" },
    { 205, "Reset Content" },
    { 206, "Partial Content" },

    { 300, "Multiple Choices" },
    { 301, "Moved Permanently" },
    { 302, "Found" },
    { 303, "See Other" },
    { 304, "Not Modified" },
    { 305, "Use Proxy" },
    // { 306, "Unused" },
    { 307, "Temporary Redirect" },
    { 308, "Permanent Redirect" },

    { 400, "Bad Request" },
    { 401, "Unauthorized" },
    { 402, "Payment Required" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
    { 405, "Method Not Allowed" },
    { 406, "Not Acceptable" },
    { 407, "Proxy Authentication Required" },
    { 408, "Request Timeout" },
    { 409, "Conflict" },
    { 410, "Gone" },
    { 411, "Length Required" },
    { 412, "Precondition Failed" },
    { 413, "Request Entity Too Large" },
    { 414, "Request-URI Too Long" },
    { 415, "Unsupported Media Type" },
    { 416, "Requested Range Not Satisfiable" },
    { 417, "Expectation Failed" },
    { 426, "Upgrade Required" },
    { 428, "Precondition Failed" },
    { 429, "Too Many Requests" },
    { 431, "Request Header Fields Too Large" },
    { 451, "Unavailable For Legal Reasons" },

    { 500, "Internal Server Error" },
    { 501, "Not Implemented" },
    { 502, "Bad Gateway" },
    { 503, "Service Unavailable" },
    { 504, "Gateway Timeout" },
    { 505, "HTTP Version Not Supported" },
    { 510, "Not Extended" },
    { 511, "Network Authentication Required" },
    { 598, "Network Read Timeout Error" },
    { 599, "Network Connect Timeout Error" },
};

// List of Common MIME types found here:
// Source: https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
key_value_t extToMIME[] ={
    { "aac",    "audio/aac" },
    { "abw",    "application/x-abiword" },
    { "arc",    "application/x-freearc" },
    { "avi",    "video/x-msvideo" },
    { "azw",    "application/vnd.amazon.ebook" },
    { "bin",    "application/octet-stream" },
    { "bmp",    "image/bmp" },
    { "bz",     "application/x-bzip" },
    { "bz2",    "application/x-bzip2" },
    { "csh",    "application/x-csh" },
    { "css",    "text/css" },
    { "csv",    "text/csv" },
    { "doc",    "application/msword" },
    { "docx",   "application/vnd.openxmlformats-officedocument.wordprocessingml.document" },
    { "eot",    "application/vnd.ms-fontobject" },
    { "epub",   "application/epub+zip" },
    { "gz",     "application/gzip" },
    { "gif",    "image/gif" },
    { "htm",    "text/html" },
    { "html",   "text/html" },
    { "ico",    "image/vnd.microsoft.icon" },
    { "ics",    "text/calendar" },
    { "jar",    "application/java-archive" },
    { "jpeg",   "image/jpeg" },
    { "jpg",    "image/jpeg" },
    { "js",     "text/javascript" },
    { "json",   "application/json" },
    { "jsonld", "application/ld+json" },
    { "mid",    "audio/midi" },       // Either one???
    { "midi",   "audio/x-midi" },     // 
    { "mjs",    "text/javascript" },
    { "mp3",    "audio/mpeg" },
    { "mpeg",   "video/mpeg" },
    { "mpkg",   "application/vnd.apple.installer+xml" },
    { "odp",    "application/vnd.oasis.opendocument.presentation" },
    { "ods",    "application/vnd.oasis.opendocument.spreadsheet" },
    { "odt",    "application/vnd.oasis.opendocument.text" },
    { "oga",    "audio/ogg" },
    { "ogv",    "video/ogg" },
    { "ogx",    "application/ogg" },
    { "opus",   "audio/opus" },
    { "otf",    "font/otf" },
    { "png",    "image/png" },
    { "pdf",    "application/pdf" },
    { "php",    "application/x-httpd-php" },
    { "ppt",    "application/vnd.ms-powerpoint" },
    { "pptx",   "application/vnd.openxmlformats-officedocument.presentationml.presentation" },
    { "rar",    "application/vnd.rar" },
    { "rtf",    "application/rtf" },
    { "sh",     "application/x-sh" },
    { "svg",    "image/svg+xml" },
    { "swf",    "application/x-shockwave-flash" },
    { "tar",    "application/x-tar" },
    { "tif",    "image/tiff" },
    { "tiff",   "image/tiff" },
    { "ts",     "video/mp2t" },
    { "ttf",    "font/ttf" },
    { "txt",    "text/plain" },
    { "vsd",    "application/vnd.visio" },
    { "wav",    "audio/wav" },
    { "weba",   "audio/webm" },
    { "webm",   "video/webm" },
    { "webp",   "image/webp" },
    { "woff",   "font/woff" },
    { "woff2",  "font/woff2" },
    { "xhtml",  "application/xhtml+xml" },
    { "xls",    "application/vnd.ms-excel" },
    { "xlsx",   "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" },
    { "xml",    "application/xml" },    // Exceptions, see link
    { "xul",    "application/vnd.mozilla.xul+xml" },
    { "zip",    "application/zip" },
    { "3gp",    "video/3gpp" },     // Exceptions, see link
    { "3g2",    "video/3gpp2" },    // Exceptions, see link
    { "7z",     "application/x-7z-compressed" },
};

typedef enum {
    PAYLOAD_NONE,
    PAYLOAD_BUFFER,
    PAYLOAD_FILE,
} PAYLOAD_TYPE;

const char CRLF[] = { 13, 10, 0 };

int readline(FILE* f, char* buf, size_t bufsize) {

    // Read the line
    if (!fgets(buf, bufsize, f)) {
        return -1;
    }

    size_t len = strlen(buf);

    // Check for CRLF sequence
    if (strstr(buf, CRLF) != (buf + len - 2)) {
        return -1;
    }

    buf[len - 2] = '\0';
    buf[len - 1] = '\0';

    return 0;
}

int readCookies(http_request_t* req, char* str) {
    char* bookmark, *bookmark2;
    char* cookie;
    key_value_t kv;

    while ((cookie = strtok_r(str, "; ", &bookmark))) {
        char* temp = NULL;

        temp = strtok_r(cookie, "=", &bookmark2);
        if (temp) kv.name = strdup(temp);

        temp = NULL;
        temp = strtok_r(NULL, "=", &bookmark2);
        if (temp) kv.value = strdup(temp);

        vector_push(&req->cookies, &kv);

        str = NULL;
    }

    return 0;
}

int readHttpHeaders(FILE* f,  http_request_t* req) {
    char buf[MAX_LINE_LENGTH];
    char* bookmark = NULL;

    while (1) {

        if (readline(f, buf, MAX_LINE_LENGTH)) {
            return -1;
        }
        
        if (strlen(buf) == 0) {
            break;
        }

        char* name = strtok_r(buf, ": ", &bookmark);
        char* value = strtok_r(NULL, "", &bookmark);

        key_value_t* kv = malloc(sizeof(key_value_t));
        kv->name = strdup(name);
        kv->value = strdup(value);

        vector_push(&req->headers, kv);

        if (!strcmp(name, "Cookie")) {
            readCookies(req, value);
        }

    }

    return 0;
}

// Functions for HTTP Requests

int readHttpRequest(FILE* f, http_request_t* req) {

    vector_init(&req->headers, sizeof(key_value_t), freeKeyValue);
    vector_init(&req->cookies, sizeof(key_value_t), freeKeyValue);

    char buf[MAX_LINE_LENGTH];

    // Get the header
    if (readline(f, buf, MAX_LINE_LENGTH)) return -1;

    char* bookmark = NULL;
    char* bookmark2 = NULL;

    // Verify the verb
    req->verb = strtok_r(buf, " ", &bookmark);
    if ((strcmp(req->verb, "GET") != 0) && (strcmp(req->verb, "POST") != 0)) {
        return -1;
    }
    if (req->verb) req->verb = strdup(req->verb);
    else return -1;

    // Verify the path
    char *temp = strtok_r(NULL, " ", &bookmark);
    req->path = strtok_r(temp, "?", &bookmark2);
    if (req->path) req->path = strdup(req->path);
    else return -1;

    // Read the query if there is one
    req->query = strtok_r(NULL, "", &bookmark2);
    if (req->query) req->query = strdup(req->query);

    // Verify the version
    req->version = strtok_r(NULL, " ", &bookmark);
    if (req->version) req->version = strdup(req->version);
    else return -1;

    if (readHttpHeaders(f, req)) {
        return -1;
    }

    int payloadSize = 0;
    const char *psize = getHttpHeader(req, "Content-Length", buf, sizeof(buf));
    if (psize) payloadSize = atoi(psize);

    if (payloadSize) {
        req->payload = malloc(payloadSize + 1);
        if (!fread(req->payload, 1, payloadSize, f)) {
            return -1;
        }
        ((char*)req->payload)[payloadSize] = '\0';
    }

    return 0;
}

const char* getHttpHeader(const http_request_t* req, const char* name, char* buf, size_t size) {
    key_value_t kv;
    int i = vector_find(&req->headers, findKeyValueByName, name, &kv);
    if (i != -1) {
        strlcpy(buf, kv.value, size);
        return buf;
    }
    else {
        return NULL;
    }
}

const char* getHttpCookie(const http_request_t* req, const char* name, char* buf, size_t size) {
    key_value_t kv;
    int i = vector_find(&req->cookies, findKeyValueByName, name, &kv);
    if (i != -1) {
        strlcpy(buf, kv.value, size);
        return buf;
    }
    else {
        return NULL;
    }
}

void freeHttpRequest(http_request_t* req) {
    free(req->verb);
    free(req->path);
    free(req->version);
    free(req->query);

    vector_free(&req->headers);
    vector_free(&req->cookies);

    free(req->payload);

    memset(req, 0, sizeof(http_request_t));
}

// Functions for HTTP Responses

void initHttpResponse(http_response_t* res) {
    res->status = HTTP_200_OK;
    vector_init(&res->headers, sizeof(key_value_t), freeKeyValue);
    vector_init(&res->cookies, sizeof(key_value_t), freeKeyValue);
    res->payloadType = PAYLOAD_NONE;
}

void httpAddHeader(http_response_t* res, const char* name, const char* value) {
    key_value_t kv;
    kv.name = strdup(name);
    kv.value = strdup(value);

    vector_push(&res->headers, &kv);
}

void httpAddCookie(http_response_t* res, const char* name, const char* value) {
    key_value_t kv;
    kv.name = strdup(name);
    kv.value = strdup(value);

    vector_push(&res->cookies, &kv);
}

// TODO: check if we have permissions to the file before 
// returning
int httpAddPayloadFile(http_response_t* res, const char* filepath) {

    // Make sure there isn't another payload already hooked onto this
    // response struct
    if (res->payloadType != PAYLOAD_NONE) return -1;
    
    // Make sure file exists
    struct stat buf;
    if (stat(filepath, &buf)) {
        // If not, set the status accordingly
        res->status = HTTP_404_NOT_FOUND;
        return -1;
    }

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%ld", buf.st_size);
    httpAddHeader(res, "Content-Length", buffer);

    // To-do: auto-check the mime-type
    const char* ext = strrchr(filepath, '.');
    if (ext) {
        ext += 1;
        int found = 0;
        for (unsigned int i = 0; i < sizeof(extToMIME) / sizeof(key_value_t); ++i) {
            if (!strcmp(extToMIME[i].name, ext)) {
                httpAddHeader(res, "Content-Type", extToMIME[i].value);
                found = 1;
                break;
            }
        }
        
        if (!found) {
            httpAddHeader(res, "Content-Type", "application/octet-stream");
        }
    }
    else {
        httpAddHeader(res, "Content-Type", "application/octet-stream");
    }
    

    res->payloadType = PAYLOAD_FILE;
    res->payload.uri = strdup(filepath);

    return 0;
}

int httpAddPayloadBuffer(http_response_t* res, const void* buf, size_t size) {
    // Make sure there isn't another payload already hooked onto this
    // response struct
    if (res->payloadType != PAYLOAD_NONE) return -1;

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%ld", size);
    httpAddHeader(res, "Content-Length", buffer);

    // To-do: get a content-type
    httpAddHeader(res, "Content-Type", "text/html");

    res->payloadType = PAYLOAD_BUFFER;
    res->payload.buffer.buffer = buf;
    res->payload.buffer.size = size;

    return 0;
}

void httpRedirectTo(http_response_t* res, const char* url) {

    res->status = HTTP_303_SEE_OTHER;
    httpAddHeader(res, "Location", url);
}

void sendHttpResponse(http_response_t* res, FILE* f) {

    // Write the header
    const http_status_t* status = &httpStatuses[res->status];
    fprintf(f, "HTTP/1.1 %d %s\r\n", status->number, status->status);

    // Write the headers and cookies
    key_value_t* kv;
    for (void* i = res->headers.data; i < res->headers.end; i += res->headers.msize) {
        kv = (key_value_t*) i;
        fprintf(f, "%s: %s\r\n", kv->name, kv->value);
    }

    for (void* i = res->cookies.data; i < res->cookies.end; i += res->cookies.msize) {
        kv = (key_value_t*) i;
        fprintf(f, "Set-Cookie: %s=%s\r\n", kv->name, kv->value);
    }

    fprintf(f, "\r\n");

    if (res->payloadType == PAYLOAD_FILE) {

        struct stat buf;
        stat(res->payload.uri, &buf);

        size_t bytesLeft = buf.st_size;

        // Assume the file exists because of stat
        // TODO: check permissions first
        FILE* file = fopen(res->payload.uri, "r");

        unsigned char buffer[4096];

        while (bytesLeft > sizeof(buffer)) {
            fread(buffer, 1, sizeof(buffer), file);
            fwrite(buffer, 1, sizeof(buffer), f);
            bytesLeft -= sizeof(buffer);
        }

        if (bytesLeft > 0) {
            fread(buffer, 1, bytesLeft, file);
            fwrite(buffer, 1, bytesLeft, f);
        }

        fclose(file);
    }
    else if (res->payloadType == PAYLOAD_BUFFER) {
        fwrite(res->payload.buffer.buffer, 1, res->payload.buffer.size, f);
    }

    fflush(f);
}

void sendHttpRedirect(FILE* f, const char* filepath) {
    http_response_t res;
    initHttpResponse(&res);
    httpRedirectTo(&res, filepath);
    sendHttpResponse(&res, f);
    freeHttpResponse(&res);
}

void sendHttpFile(FILE* f, const char* filepath) {
    http_response_t res;
    initHttpResponse(&res);
    httpAddPayloadFile(&res, filepath);
    sendHttpResponse(&res, f);
    freeHttpResponse(&res);
}

void freeHttpResponse(http_response_t* res) {
    vector_free(&res->headers);
    vector_free(&res->cookies);
}

int doesFileExist(const char* filepath, struct stat* buf) {
    struct stat buf2;
    return !stat(filepath, buf ? buf : &buf2);
}

void writeHttpResponse(const http_response_t* res, FILE* f) {
    key_value_t* kv;
    for (void* i = res->headers.data; i < res->headers.end; i += res->headers.msize) {
        kv = (key_value_t*) i;
        fprintf(f, "%s: %s\r\n", kv->name, kv->value);
        // printf("%s: %s\r\n", kv->name, kv->value);
    }

    for (void* i = res->cookies.data; i < res->cookies.end; i += res->cookies.msize) {
        kv = (key_value_t*) i;
        fprintf(f, "Set-Cookie: %s=%s\r\n", kv->name, kv->value);
        // printf("Set-Cookie: %s=%s\r\n", kv->name, kv->value);
    }
}

void printHttp(http_request_t* req) {
    printf("VERB:    %s\n", req->verb);
    printf("PATH:    %s\n", req->path);
    printf("VERSION: %s\n", req->version);
    for (size_t i = 0; i < req->headers.size; ++i) {
        key_value_t kv;
        vector_get(&req->headers, i, &kv);
        printf("%s: %s\n", kv.name, kv.value);
    }
}