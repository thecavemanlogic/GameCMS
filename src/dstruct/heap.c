
#include "heap.h"

void swap(void* buf1, void* buf2, size_t msize) {
    unsigned char buf[msize];
    memcpy(buf, buf1, msize);
    memcpy(buf1, buf2, msize);
    memcpy(buf2, buf, msize);
}

// Compare should return a non-zero number if the first
// argument is greater than the second

void percolate_up(vector_t* h, int (*compare)(void*, void*), int index) {
    while (1) {
        int parent_index = PARENT(index);
        if (parent_index == -1) break;

        void* pp = h->data + parent_index * h->msize;
        void* ip = h->data + index * h->msize;

        if (compare(pp, pi)) {
            swap(pp, pi, h->msize);
            index = parent_index
        }
        else {
            break;
        }
    }
}

int heap_heapify(vector_t* h, int (*compare)(void*, void*)) {

}

int heap_insert(vector_t* h, int (*compare)(void*, void*), void* value) {

}

int heap_deleteMin(vector_t* h, int (*compare)(void*, void*), void* dest) {

}