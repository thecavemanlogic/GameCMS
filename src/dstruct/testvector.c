#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vector.h"

vector_t vec;

void print() {
    printf("Printing %ld ints...\n", vec.size);
    for (size_t i = 0; i < vec.size; ++i) {
        int a;
        vector_get(&vec, i, &a);
        printf("%d ", a);
    }
    putchar('\n');
}

int main() {

    srand(time(NULL));

    FILE* f = fopen("test.bin", "r");
    if (f) {
        puts("Vector file found");
        vector_read(&vec, NULL, NULL, f);
        print();
        fclose(f);
    }
    else {
        puts("No vector file found");
        vector_init(&vec, sizeof(int), NULL);
    }

    int a = -2;
    while (a != -1) {
        printf("Enter a number (-1 to quit) ");
        fflush(stdout);

        scanf("%d", &a);

        vector_push(&vec, &a);

        printf("vs = %ld\n", vec.size);
    }

    f = fopen("test.bin", "w+");
    vector_write(&vec, NULL, f);
    fclose(f);

    vector_free(&vec);

}