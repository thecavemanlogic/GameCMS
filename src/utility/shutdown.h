#ifndef _SHUTDOWN_H
#define _SHUTDOWN_H

// Description
// 	
// Parameters
// 	
// Returns
// 	
void setupShutdown(void (*action)(void));

// Description
// 	
// Parameters
// 	
// Returns
// 	
void registerChild(int pid);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void shutdownNow();

#endif