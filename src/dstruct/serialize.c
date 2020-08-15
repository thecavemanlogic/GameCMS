#include "serialize.h"

typedef enum {
    MEMBER_CHAR,
    MEMBER_SHORT,
    MEMBER_INT,
    MEMBER_FLOAT,
    MEMBER_STRING,
    MEMBER_STRUCT,
    MEMBER_UNKNOWN
} MEMBER;

typedef enum {
    MAKEUP_POINTER = 1 << 0,
    MAKEUP_STRING  = 1 << 1,
    MAKEUP_BYTE    = 1 << 2
} MAKEUP_FLAGS;

typedef struct {
    MAKEUP_FLAGS flags;
    unsigned char memberCount;
} makeup_chunk_t;

typedef struct {
    unsigned int chunkCount;
    makeup_chunk_t chunks[];
} makeup_t;

typedef enum {
    // The data type is [memberType]*
    FLAG_POINTER,

    // Skip this value
    FLAG_SKIP
};

void serialize_read(FILE* f, const char* makeup, void* dest) {
    int flag = 0;
    int memberCount = 0;
    int memberType = 0;
    const char* b = makeup;

    while (*b) {

        // 
        if (*b == 'p') {
            flag = FLAG_POINTER;
            ++b;
        }
        else if (*b == '.') {
            flag = FLAG_SKIP;
            ++b;
        }
        
        if (*b >= '0' && *b <= '9') {
            memberCount = strtol(b, &b, 10);
        }

        switch (*b) {
            case 'c': memberType = MEMBER_CHAR; break;

            // For the format specifier for a short
            // Source: https://stackoverflow.com/questions/19809527/wrong-format-specifier-in-scanfd-unsigned-short-int-after-getspointer-ch
            case 'h': memberType = MEMBER_SHORT; break;
            case 'd': memberType = MEMBER_INT; break;
            case 'f': memberType = MEMBER_FLOAT; break;
            case 's': memberType = MEMBER_STRING; break;
            case '{': memberType = MEMBER_STRUCT; break;
            default: memberType = MEMBER_UNKNOWN; break;
        }

        if (memberType == MEMBER_UNKNOWN) return -1;
        else if (memberType == MEMBER_STRUCT) {

        }
        else if (memberType == MEMBER_STRING) {

        }
        else {

        }

        ++b;
    }
}

void serialize_write(FILE* f, const char* makeup, const void* src);