#include <stdio.h>

#include "container.h"

int main() {

    container_t* c = createContainer("/prog");

    freeContainer(c);
}