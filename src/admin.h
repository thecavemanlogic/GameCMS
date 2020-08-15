// File: 
// Author: Caedmon M. Evans
// Description:
//  

#ifndef _ADMIN_H
#define _ADMIN_H

// Description
// 	Does nothing for now
// 
// Parameters
// 	None
// 
// Returns
// 	Garbage
int adminInit();

// Description
// 	Runs the admin prompt in the calling thread, so it will block.
// 
// Parameters
// 	None
// 
// Returns
// 	0
int adminRun();

// Description
// 	Flags the admin as not running
// 
// Parameters
// 	None
// 
// Returns
// 	0
int adminClose();

#endif