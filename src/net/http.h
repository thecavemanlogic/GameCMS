#ifndef _HTTP_H
#define _HTTP_H

#include <stdio.h>
#include <sys/stat.h>

#include "../dstruct/vector.h"

#define MAX_HEADERS 60

#define MAX_LINE_LENGTH 500
#define MAX_HEADER_NAME_SIZE 16
#define MAX_HEADER_VALUE_SIZE 64

// Information for HTTP status codes retrieved here
// Source: https://www.restapitutorial.com/httpstatuscodes.html
typedef enum {

    // 1xx Informational
    HTTP_100_CONTINUE,
    HTTP_101_SWITCHING_PROTOCOLS,
    HTTP_102_PROCESSING,

    // 2xx Success
    HTTP_200_OK,
    HTTP_201_CREATED,
    HTTP_202_ACCEPTED,
    HTTP_203_NON_AUTHORIZED_INFORMATION,
    HTTP_204_NO_CONTENT,
    HTTP_205_RESET_CONTENT,
    HTTP_206_PARTIAL_CONTENT,

    // 3xx Redirection
    HTTP_300_MULTIPLE_CHOICES,
    HTTP_301_MOVED_PERMANENTLY,
    HTTP_302_FOUND,
    HTTP_303_SEE_OTHER,
    HTTP_304_NOT_MODIFIED,
    HTTP_305_USE_PROXY,
    // HTTP_306_UNUSED,
    HTTP_307_TEMPORARY_REDIRECT,
    HTTP_308_PERMANENT_REDIRECT,

    // 4xx Client Error
    HTTP_400_BAD_REQUEST,
    HTTP_401_UNAUTHORIZED,
    HTTP_402_PAYMENT_REQUIRED,
    HTTP_403_FORBIDDEN,
    HTTP_404_NOT_FOUND,
    HTTP_405_METHOD_NOT_ALLOWED,
    HTTP_406_NOT_ACCEPTABLE,
    HTTP_407_PROXY_AUTHENTICATION_REQUIRED,
    HTTP_408_REQUEST_TIMEOUT,
    HTTP_409_CONFLICT,
    HTTP_410_GONE,
    HTTP_411_LENGTH_REQUIRED,
    HTTP_412_PRECONDITION_FAILED,
    HTTP_413_REQUEST_ENTITY_TOO_LARGE,
    HTTP_414_REQUEST_URI_TOO_LONG,
    HTTP_415_UNSUPPORTED_MEDIA_TYPE,
    HTTP_416_REQUESTED_RANGE_NOT_SATISFIABLE,
    HTTP_417_EXPECTATION_FAILED,
    HTTP_426_UPGRADE_REQUIRED,
    HTTP_428_PRECONDITION_REQUIRED,
    HTTP_429_TOO_MANY_REQUESTS,
    HTTP_431_REQUEST_HEADER_FIELDS_TOO_LARGE,
    HTTP_451_UNAVAILABLE_FOR_LEGAL_REASONS,

    // 5xx Server Error
    HTTP_500_INTERNAL_SERVER_ERROR,
    HTTP_501_NOT_IMPLEMENTED,
    HTTP_502_BAD_GATEWAY,
    HTTP_503_SERVICE_UNAVAILABLE,
    HTTP_504_GATEWAY_TIMEOUT,
    HTTP_505_HTTP_VERSION_NOT_SUPPORTED,
    HTTP_510_NOT_EXTENDED,
    HTTP_511_NETWORK_AUTHENTICATION_REQUIRED,
    HTTP_598_NETWORK_READ_TIMEOUT_ERROR,
    HTTP_599_NETWORK_CONNECT_TIMEOUT_ERROR,
} HTTP_STATUS;

// For naming the part s
// Source: https://www.ibm.com/support/knowledgecenter/en/SSGMCP_5.2.0/com.ibm.cics.ts.internet.doc/topics/dfhtl22.html
typedef struct {
    int number;
    char* status;
} http_status_t;

// Struct to store an entire HTTP Request
typedef struct http_request {
    char* verb;
    char* path;
    char* version;
    char* query;

    vector_t headers;
    vector_t cookies;

    size_t ContentLength;
    void* payload;
} http_request_t;

typedef struct {
    int status;
    vector_t headers;
    vector_t cookies;
    int payloadType;
    union {
        const char* uri;
        struct {
            const void* buffer;
            size_t size;
        } buffer;
    } payload;
} http_response_t;

// Description
// 	
// Parameters
// 	
// Returns
// 	
int readHttpRequest(FILE* f, http_request_t* req);

// Description
// 	
// Parameters
// 	
// Returns
// 	
const char* getHttpHeader(const http_request_t* req, const char* name, char* buf, size_t size);

// Description
// 	
// Parameters
// 	
// Returns
// 	
const char* getHttpCookie(const http_request_t* req, const char* name, char* buf, size_t size);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void freeHttpRequest(http_request_t* req);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void initHttpResponse(http_response_t* res);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void httpAddHeader(http_response_t* res, const char* name, const char* value);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void httpAddCookie(http_response_t* res, const char* name, const char* value);

// Description
// 	
// Parameters
// 	
// Returns
// 	
int httpAddPayloadFile(http_response_t* res, const char* filepath);

// Description
// 	
// Parameters
// 	
// Returns
// 	
int httpAddPayloadBuffer(http_response_t* res, const void* buf, size_t size);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void httpRedirectTo(http_response_t* res, const char* url);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void sendHttpResponse(http_response_t* res, FILE* f);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void sendHttpRedirect(FILE* f, const char* filepath);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void sendHttpFile(FILE* f, const char* filepath);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void freeHttpResponse(http_response_t* res);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void printHttp(http_request_t* req);

#endif