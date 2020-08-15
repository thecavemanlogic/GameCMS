// File: heap.h
// Author: Caedmon M. Evans
// Description:
//  Dictates how heaps should be implemented in C

#ifndef _HEAP_H
#define _HEAP_H

#include "vector.h"

#define D_VALUE 4
#define CHILD(x) ( x * D_VALUE + 1 )
#define PARENT(x) ( (x - 1) / D_VALUE )

// Description
// 	
// Parameters
// 	
// Returns
// 	
int heap_heapify(vector_t* h);

// Description
// 	
// Parameters
// 	
// Returns
// 	
int heap_insert(vector_t* h, void* value);

// Description
// 	
// Parameters
// 	
// Returns
// 	
int heap_deleteMin(vector_t* h, void* dest);

#endif