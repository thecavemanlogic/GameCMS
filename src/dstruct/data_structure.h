// File: 
// Author: Caedmon M. Evans
// Description:
//  

#ifndef _DATA_STRUCTURE_H
#define _DATA_STRUCTURE_H

typedef struct {
    size_t size;
    size_t msize;
    void (*freePolicy)(void* data);
} data_structure_header_t;

void dsh_init(data_structure_header_t* dsh);



void dsh_free(data_structure_header_t* dsh);

#endif