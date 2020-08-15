// File: 
// Author: Caedmon M. Evans
// Description:
//  

#ifndef _FSTRING_H
#define _FSTRING_H

#include <stdlib.h>

typedef struct {
    char* string;
    size_t length;
    size_t capacity;
    unsigned int hash;
} string_t;

int strequ(const char* str1, const char* str2);

#endif