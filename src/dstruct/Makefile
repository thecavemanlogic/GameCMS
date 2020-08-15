CFLAGS = -Wall -Wextra -Wwrite-strings -g

all: lib tests

# core

lib: vector.o hashmap.o

vector.o: vector.h vector.c
	gcc -c $(CFLAGS) vector.c -o vector.o

hashmap.o: hashmap.h hashmap.c
	gcc -c $(CFLAGS) hashmap.c -o hashmap.o

key_value.o: key_value.h key_value.c
	gcc -c $(CFLAGS) key_value.c -o key_value.o

# tests

tests: testvector teststringvector testhashmap

testvector: vector.o testvector.c
	gcc $(CFLAGS) testvector.c vector.o -o testvector

teststringvector: vector.o teststringvector.c
	gcc $(CFLAGS) teststringvector.c vector.o -o teststringvector

testhashmap: hashmap.o testhashmap.c
	gcc $(CFLAGS) testhashmap.c hashmap.o -o testhashmap