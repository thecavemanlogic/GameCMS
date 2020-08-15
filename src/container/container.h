// File: container.h
// Author: Caedmon M. Evans
// Description:
//  This file contains functions related to securing
//  a program so that user programs can run without
//  compromising the system.

#ifndef _CONTAINER_H
#define _CONTAINER_H

typedef struct {
    int pid;
    char* root;
} container_t;

// Description
// 	Creates a new secure container and executes command
//  after doing so.
// 
// Parameters
// 	command - the command to be executed through execv(2)
// 
// Returns
// 	On success, the container that was created, NULL on
//  error.
container_t* createContainer(const char* command);

// Description
// 	Frees a container object
// 
// Parameters
// 	container - the container to be freed
// 
// Returns
// 	Nothing
void freeContainer(container_t* container);

#endif