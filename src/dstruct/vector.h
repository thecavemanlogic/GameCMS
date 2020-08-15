// File: vector.h
// Author: Caedmon M. Evans
// Description:
//  This file holds the declarations for the vector implementation

#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdio.h>

#define VECTOR_INITIAL_CAPACITY 16

typedef struct {
    void* data;
    void* end;
    size_t size;
    size_t capacity;
    size_t msize;
    void (*freePolicy)(void* data);
} vector_t; 

// Description
// 	Initializes a vector with a capacity of VECTOR_INITIAL_CAPACITY
// 
// Parameters
// 	v - the vector to be initialized
//  msize - the size of the members that the vector will be holding
//  freePolicy - the function to call when a element is removed from
//               the vector
// 
// Returns
// 	Nothing
void vector_init(vector_t* v, size_t msize, void (*freePolicy)(void* data));

// Description
// 	Pushes a new member at the end of a vector
// 
// Parameters
// 	v - the vector to receive the new member
//  data - a pointer to the new member
// 
// Returns
// 	Nothing
void vector_push(vector_t* v, void* data);

// Description
// 	Removes a member from the end of a vector
// 
// Parameters
// 	v - the vector to perform the operation on
// 
// Returns
//  0 on success, -1 on error
int vector_pop(vector_t* v);

// Description
// 	removes an element at an index
// 
// Parameters
// 	v - the vector to remove from
//  index - the index that the removed member is located
// 
// Returns
//  0 on success, -1 on error	
int vector_remove(vector_t* v, size_t index);

// Description
// 	Removes all elements in a vector that fit a certain requirement. policy
//  should return a non-zero value if a member fits the requirement for
//  removal, aand zero if it doesn't. data may be NULL, as it is passed as the
//  second argument to policy. The first argument is a pointer to the member
//  located inside the vector's internal buffer.
// 
// Parameters
//  v - the vector to perform the operation on
//  policy - the function that defines the requirement
//  data - information that may be useful in determining a member's qualification for removal
// 
// Returns
// 	Nothing
void vector_removeif(vector_t* v, int (*policy)(void*, void*), void* data);

// Description
// 	Copies the contents of a member located a a certain index into another
//  location
// 
// Parameters
//  v - the vector to retrieve the value from
//  index - the index to read the member
//  val - the location where vector_get should copy the contents of member to
// 	
// Returns
// 	0 on success, -1 on error
int vector_get(const vector_t* v, size_t index, void* val);

// Description
// 	Overwrites a member at an index with new data
// 
// Parameters
//  v - the vector to perform the operation on
//  index - the index at which the new data will be written to 
//  val - the address of the new member data	
// 
// Returns
// 	0 on success, -1 on error
int vector_set(vector_t* v, size_t index, void* val);

// Description
//  Removes all elements in a vector
// 	
// Parameters
//  v - the vector to perform the operation on
// 	
// Returns
// 	Nothing
void vector_clear(vector_t* v);

// Description
// 	Removes all elements in a vector and cleans up the internals
// 
// Parameters
// 	v - the vector to perform the operation on
// 
// Returns
// 	Nothing
void vector_free(vector_t* v);

// Calls <func> on each member in vector_t <v>

// Description
//  Performs a function on each member in a vector
// 
// Parameters
//  v - the vector to perform the operation on
//  func - the function to call on every member
// 	
// Returns
// 	Nothing
void vector_foreach(vector_t* v, void (*func)(void*, void*), void* data);

// Description
// 	Finds and returns the first member to fit the requirments set by policy. policy
//  should return a non-zero value to indicate that a member met the requirements
//  to be found and zero if the member did not meet the requirements.
// 
// Parameters
// 	v - the vector to perform the operation on
//  policy - the function that defines the requirements
// 
// Returns
//  an integer >= 0 on denoting the index on success, -1 if the member could not be found
int vector_find(const vector_t* v, int (*policy)(const void*, const void*), const void* data, void* dest);

// Writes the entire vector_t <v> into the file stream <f>.

// Description
//  Writes a vector and its contents to a file stream. If writePolicy is NULL,
//  the value store int the vector is written as is.
// 	
// Parameters
//  v - the vector to write to the file stream
//  writePolicy - the function to use to write to the file stream
//  f - the file stream to write to
// 	
// Returns
// 	Nothing
void vector_write(const vector_t* v, void (*writePolicy)(const void*, FILE*), FILE* f);

// Reads the vector_t data from the file stream <f> into
// vector_t <v>.

// Description
// 	Reads a vector and its contents from a file stream. If readPolicy is NULL,
//  then the values are read in as is.
// 
// Parameters
//  v - the vector struct to hold the incoming data
//  readPolicy - the function to correctly read a new value
//  freePolicy - the function to call when a member is removed from the vector
//  f - the file stream to read data from
// 	
// Returns
// 	Nothing
void vector_read(vector_t* v, void (*readPolicy)(void*, FILE*), void (*freePolicy)(void* data), FILE* f);

#endif