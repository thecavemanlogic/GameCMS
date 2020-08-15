#ifndef _CONFIGREADER_H
#define _CONFIGREADER_H

// Opens a file at <filepath> for reading.

// Description
// 	
// Parameters
// 	
// Returns
// 	
int startReading(const char* filepath);

// Reads a line from a file opened by startReading.
// Returns:
// 		0 on success
// 		a positive number when failed. The integer represents the line the error occured
// 			on

// Description
// 	
// Parameters
// 	
// Returns
// 	
int nextLine(char** tag, char** value);

// Description
// 	
// Parameters
// 	
// Returns
// 	
void closeReader();

#endif