
all: test container.o

test: test.c container.o prog
	gcc test.c container.o -lbsd -lcap -o test

container.o: container.h container.c
	gcc -c container.c -o container.o

prog: prog.c
	gcc prog.c -o prog