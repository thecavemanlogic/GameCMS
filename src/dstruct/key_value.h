#ifndef KEY_VALUE_H
#define KEY_VALUE_H

typedef struct {
    char* name;
    char* value;
} key_value_t;

void freeKeyValue(void* data);

int findKeyValueByName(const void* _kv, const void* _data);

#endif