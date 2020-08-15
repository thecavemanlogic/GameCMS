#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hashmap.h"

hashmap_t map;

void freeStr(void* str) {
    free(*((char**)str));
}

int main() {
    hashmap_init(&map, sizeof(char*), freeStr);

    char key[64];

    while (1) {

        printf("Type a word: ");
        fflush(stdout);

        scanf("%63s", key);

        if (!strcmp(key, "q")) {
            break;
        }

        printf("The index of your input is %lu\n", strhash(key) % map.size);

        const char* value = NULL;
        hashmap_get(&map, key, &value);

        if (value) {
            printf("The value of %s is %s\n", key, value);
        }
        else {
            printf("There is no value for %s\n", key);
        }

        char ans[64];
    ask_again:
        printf("Would you like to create a new value? ");
        fflush(stdout);

        scanf("%1s", ans);

        if (!strcmp(ans, "y")) {
            printf("Enter a new value for %s: ", key);
            fflush(stdout);

            scanf("%63s", ans);

            char* temp = strdup(ans);

            hashmap_set(&map, key, &temp);
        }
        else if (!strcmp(ans, "n")) {}
        else {
            goto ask_again;
        }
        
    }

    hashmap_free(&map);
}