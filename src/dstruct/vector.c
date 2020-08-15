#include <stdlib.h>
#include <string.h>

#include "vector.h"

// Description
//  Resize vector <v> to <newCapcity>
// Returns
//  nothing
void vector_resize(vector_t* v, size_t newCapacity) {
    v->data = realloc(v->data, v->msize * newCapacity);
    v->capacity = newCapacity;
    if (newCapacity < v->size) v->size = newCapacity;

    v->end = v->data + v->size * v->msize;
}

void vector_init(vector_t* v, size_t msize, void (*freePolicy)(void* data)) {
    // v->data = malloc(VECTOR_INITIAL_CAPACITY * msize);
    v->data = calloc(VECTOR_INITIAL_CAPACITY, msize);
    v->end = v->data;
    v->size = 0;
    v->capacity = VECTOR_INITIAL_CAPACITY;
    v->msize = msize;
    v->freePolicy = freePolicy;
}

void vector_push(vector_t* v, void* data) {
    if (v->size >= v->capacity) {
        vector_resize(v, v->capacity + v->capacity / 2);
    }

    memcpy(v->end, data, v->msize);

    ++v->size;
    v->end += v->msize;
}

int vector_pop(vector_t* v) {
    if (v->size == 0) return -1;
    v->end -= v->msize;
    --v->size;

    if (v->freePolicy) v->freePolicy(v->end);

    return 0;
}

int vector_remove(vector_t* v, size_t index) {
    if (v->size == 0) return - 1;

    if (index >= v->size) return -1;

    void* i = v->data + v->msize * index;
    size_t r = v->size - index - 1;

    if (v->freePolicy) v->freePolicy(i);
    memmove(i, i + v->msize, r * v->msize);

    --v->size;
    v->end -= v->msize;

    return 0;
}

void vector_removeif(vector_t* v, int (*policy)(void*, void*), void* _ldata) {
    for (void* i = v->data; i < v->end; i += v->msize) {

        // printf("ANALYZING ");

        if (policy(i, _ldata)) {

            // puts("BAD");

            if (v->freePolicy) v->freePolicy(i);
            
            memmove(i, i + v->msize, v->end - i);
            v->end -= v->msize;
            --v->size;

            i -= v->msize;
        }
        else {
            // puts("GOOD");
        }

    }
}

void vector_clear(vector_t* v) {
    if (v->freePolicy) {
        for (void* i = v->data; i < v->end; i += v->msize) {
            v->freePolicy(i);
        }
    }
    v->end = v->data;
    v->size = 0;
}

void vector_free(vector_t* v) {
    if (v->freePolicy) {
        for (void* i = v->data; i < v->end; i += v->msize) {
            v->freePolicy(i);
        }
    }

    free(v->data);
}

int vector_get(const vector_t* v, size_t index, void* val) {
    if (index >= v->size) return -1;

    memcpy(val, v->data + v->msize * index, v->msize);

    return 0;
}

int vector_set(vector_t* v, size_t index, void* val) {
    if (index >= v->size) return -1;

    memcpy(v->data + v->msize * index, val, v->msize);

    return 0;
}

void vector_foreach(vector_t* v, void(*foo)(void*, void*), void* data) {
    for (void* i = v->data; i < v->end; i += v->msize) {
        foo(i, data);
    }
}

int vector_find(const vector_t* v, int (*policy)(const void*, const void*), const void* data, void* dest) {
    int index = 0;
    for (void* i = v->data; i < v->end; i += v->msize, ++index) {
        if (policy(i, data)) {
            if (dest) memcpy(dest, i, v->msize);
            return index;
        }
    }
    return -1;
}


void vector_write(const vector_t* v, void (*writePolicy)(const void*, FILE*), FILE* f) {

    fwrite(v, sizeof(vector_t), 1, f);

    if (writePolicy) {
        for (void* i = v->data; i < v->end; i += v->msize) {
            writePolicy(i, f);
        }
    }
    else {
        fwrite(v->data, v->msize, v->size, f);
    }
}

void vector_read(vector_t* v, void (*readPolicy)(void*, FILE*), void (*freePolicy)(void* data), FILE* f) {
    fread(v, sizeof(vector_t), 1, f);
    v->data = malloc(v->capacity * v->msize);
    v->end = v->data + v->msize * v->size;
    v->freePolicy = freePolicy;

    if (readPolicy) {
        for (void* i = v->data; i < v->end; i += v->msize) {
            readPolicy(i, f);
        }
    }
    else {
        fread(v->data, v->msize, v->size, f);
    }
    
}