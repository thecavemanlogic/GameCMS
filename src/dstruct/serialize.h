// File: serialize.h
// Author: Caedmon M. Evans
// Description:
//  This file holds the declarations for the dstruct serialization library.
//  This library helps dstruct structures serialize data more easily

#ifndef _SERIALIZE_H
#define _SERIALIZE_H

#include <stdio.h>

// Description
// 	Optimizes makeup to the least amount of members as possible
// 
// Parameters
// 	dest - the location to place the optimized makeup
//  makeup - the current makeup format
//  size - the size of destination buffer
// 
// Returns
// 	0 on success, -1 on failure
int serialize_optimize(char* dest, const char* makeup, unsigned int size);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void serialize_read(FILE* f, const char* makeup, void* dest);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void serialize_write(FILE* f, const char* makeup, const void* src);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void serialize_free();

#endif