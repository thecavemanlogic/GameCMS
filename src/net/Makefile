CFLAGS = -Wall -Wextra -g

all: tcp_server.o echoserver

tcp_server.o: tcp_server.h tcp_server.c
	gcc -c $(CFLAGS) tcp_server.c -o tcp_server.o

echoserver: echoserver.c tcp_server.o ../dstruct/vector.o
	gcc $(CFLAGS) echoserver.c tcp_server.o ../dstruct/vector.o -lpthread -o echoserver