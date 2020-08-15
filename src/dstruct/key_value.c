#include <stdlib.h>

#include "key_value.h"

void freeKeyValue(void* data) {
    key_value_t* kv = (key_value_t*) data;

    free(kv->name);
    free(kv->value);
}

int findKeyValueByName(const void* _kv, const void* _data) {
    key_value_t* kv = (key_value_t*) _kv;
    return !strcmp(kv->name, (char*)_data);
}