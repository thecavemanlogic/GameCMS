CFLAGS = -Wall -Wextra -Wwrite-strings -g

all: configreader.o compileconfig config.o shutdown.o registry.o log.o

configreader.o: configreader.h configreader.c
	gcc -fPIC -c $(CFLAGS) configreader.c -o configreader.o

compileconfig: compileconfig.c configreader.o config.h config.o
	gcc $(CFLAGS) configreader.o compileconfig.c config.o -lbsd -o compileconfig

config.o: config.h config.c
	gcc -c $(CFLAGS) config.c -o config.o

shutdown.o: shutdown.h shutdown.c
	gcc -c $(CFLAGS) shutdown.c -o shutdown.o

# To use OpenSSL, link with -lssl -lcrypto
# https://stackoverflow.com/questions/4352573/linking-openssl-libraries-to-a-program

registry.o: registry.h registry.c
	gcc -c $(CFLAGS) registry.c -o registry.o

log.o: log.h log.c
	gcc -c $(CFLAGS) log.c -o log.o

clean:
	rm *.o