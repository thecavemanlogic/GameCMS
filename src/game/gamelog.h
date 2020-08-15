#ifndef _GAMELOG_H
#define _GAMELOG_H

#define DEBUG

// Description
// 	
// Parameters
// 	
// Returns
// 	
void gamelog_start();

// Writes msg to stderr, including additional logging info

// Description
// 	
// Parameters
// 	
// Returns
// 	
void gamelog_internal_debug(const char* file, int linenum, const char* msg);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void gamelog_internal(const char* msg);

#ifdef DEBUG
	#define gamelog(msg) gamelog_internal_debug(__FILE__, __LINE__, msg)
#else
	#define gamelog(msg) gamelog_internal(msg)
#endif

#endif