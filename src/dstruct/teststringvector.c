#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "vector.h"

vector_t vec;

void print() {
    printf("Printing %ld strings...\n", vec.size);
    for (size_t i = 0; i < vec.size; ++i) {
        char* str = NULL;
        vector_get(&vec, i, &str);
        printf("%s\n", str);
    }
    putchar('\n');
}

void readString(void* _str, FILE* f) {
    char** str = _str;

    char buf[64];
    char* s = buf;
    char c = fgetc(f);
    while (c != '\0') {
        *s = c;
        ++s;
        c = fgetc(f);
    }
    *s = '\0';

    *str = strdup(buf);
}

void writeString(const void* _str, FILE* f) {
    const char** str = _str;
    fwrite(*str, 1, strlen(*str) + 1, f);
}

void freeString(void* _str) {
    char** str = _str;
    free(*str);
}

int main() {

    srand(time(NULL));

    FILE* f = fopen("teststring.bin", "rb");
    if (f) {
        puts("Vector file found");
        vector_read(&vec, readString, freeString, f);
        print();
        fclose(f);
    }
    else {
        puts("No vector file found");
        vector_init(&vec, sizeof(char*), freeString);
    }

    char buf[64] = { 0 };
    while (strcmp(buf, "q") != 0) {
        printf("Enter a string ('q' to quit) ");
        fflush(stdout);

        scanf("%63s", buf);
        char* str = malloc(strlen(buf) + 1);
        memcpy(str, buf, strlen(buf));
        str[strlen(buf)] = '\0';

        vector_push(&vec, &str);
    }

    f = fopen("teststring.bin", "wb+");
    vector_write(&vec, writeString, f);
    fclose(f);

    vector_free(&vec);

}