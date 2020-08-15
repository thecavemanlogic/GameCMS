#include <stdlib.h>
#include <string.h>

#include "hashmap.h"
#include "key_value.h"

struct hashmap_link_t {
    struct hashmap_link_t* next;
    char* key;
    unsigned char value[0];
};

unsigned int strhash(const char* key) {
    const char* i = key;
    unsigned int h = 0;
    while (*i) {
        h += *i * 31;
        ++i;
    }
    return h;
}

void hashmap_init(hashmap_t* h, size_t vsize, void (*freePolicy)(void* data)) {
    h->buffer = calloc(101, sizeof(struct hashmap_link_t*));
    h->size = 101;
    h->vsize = vsize;
    h->freePolicy = NULL;
    h->freePolicy = freePolicy;
}

void hashmap_free(hashmap_t* h) {
    hashmap_clear(h);
    free(h->buffer);
    h->size = 0;
}

void hashmap_set(hashmap_t* h, const char* key, const void* value) {
    unsigned int index = strhash(key) % h->size;

    struct hashmap_link_t* link = h->buffer[index];
    struct hashmap_link_t* link2 = NULL;

    while (link) {
        if (!strcmp(key, link->key)) {
            break;
        }
        link2 = link;
        link = link->next;
    }

    if (link) {
        // free(link->keyValue.value);
        // link->keyValue.value = strdup(value);
        if (h->freePolicy) h->freePolicy(link->value);

        memcpy(link->value, value, h->vsize);
    }
    else if (link2) {
        struct hashmap_link_t* hl = malloc(sizeof(struct hashmap_link_t) + h->vsize);
        hl->next = NULL;
        hl->key = strdup(key);
        // hl->keyValue.value = strdup(value);
        memcpy(hl->value, value, h->vsize);

        link2->next = hl;
    }
    else {
        // printf("Copying %d bytes\n", h->vsize);
        // printf("link->value = %p\n", link->value);
        struct hashmap_link_t* hl = malloc(sizeof(struct hashmap_link_t) + h->vsize);
        hl->next = NULL;
        hl->key = strdup(key);
        // hl->keyValue.value = strdup(value);
        memcpy(hl->value, value, h->vsize);

        h->buffer[index] = hl;
    }
}

int hashmap_get(hashmap_t* h, const void* key, void* dest) {
    unsigned int index = strhash(key) % h->size;
    struct hashmap_link_t* link = h->buffer[index];

    while (link) {
        if (!strcmp(key, link->key)) {
            break;
        }
        link = link->next;
    }

    if (link) {
        // printf("Copying %d bytes\n", h->vsize);
        memcpy(dest, link->value, h->vsize);
        return 0;
    }
    else {
        return 1;
    }

    // if (link) return link->keyValue.value;
    // else return NULL;
}

int hashmap_remove(hashmap_t* h, const char* key) {
    unsigned int index = strhash(key) % h->size;

    struct hashmap_link_t* link = h->buffer[index];
    struct hashmap_link_t* link2 = NULL;

    while (link) {
        if (!strcmp(key, link->key)) {
            break;
        }
        link2 = link;
        link = link->next;
    }

    if (link) {
        
        if (link2) {
            link2->next = link->next;
        }
        else {
            h->buffer[index] = link->next;
        }

        free(link->key);
        // free(link->keyValue.value);
        h->freePolicy(link->value);
        
        free(link);

        return 0;
    }
    else {
        return -1;
    }
}

void hashmap_clear(hashmap_t* h) {
    for (unsigned int i = 0; i < h->size; ++i) {
        struct hashmap_link_t* link = h->buffer[i];
        struct hashmap_link_t* link2 = NULL;
        
        while (link) {
            link2 = link;
            link = link->next;

            free(link2->key);
            // free(link2->keyValue.value);
            h->freePolicy(link2->value);
            free(link2);
        }
    }
}
