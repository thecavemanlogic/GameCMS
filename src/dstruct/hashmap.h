#ifndef _HASHMAP_H
#define _HASHMAP_H

#include "vector.h"

struct hashmap_link_t;

typedef struct {
    struct hashmap_link_t** buffer;
    size_t size;
    size_t vsize;
    void (*freePolicy)(void* data);
} hashmap_t;

unsigned int strhash(const char* str);

// Description
// 	
// 
// Parameters
// 	
// 
// Returns
//  Nothing
void hashmap_init(hashmap_t* h, size_t vsize, void (*freePolicy)(void* data));

void hashmap_free(hashmap_t* h);

void hashmap_set(hashmap_t* h, const char* key, const void* value);

int hashmap_get(hashmap_t* h, const void* key, void* dest);

int hashmap_remove(hashmap_t* h, const char* key);

void hashmap_clear(hashmap_t* h);

#endif