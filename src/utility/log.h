#ifndef _LOG_H
#define _LOG_H

typedef struct {
    const char* name;
} log_info_t;

// Description
// 	
// Parameters
// 	
// Returns
// 	
void logData(const log_info_t* li, const char* data);

#endif