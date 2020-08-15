CFLAGS = -Wall -Wextra -g
CLIBS  = -lssl -lcrypto -lpthread -lrt -lbsd

OFILES = net/http.o utility/shutdown.o dstruct/vector.o builder.o net/tcp_server.o utility/config.o utility/registry.o game_scheduler.o server2.o game_runner.o init.o admin.o scheduler.o dstruct/key_value.o

all: GameCMS rps testscheduler

GameCMS: GameCMS.c $(OFILES)
	gcc $(CFLAGS) GameCMS.c $(OFILES) $(CLIBS) -o GameCMS

admin.o: admin.h admin.c
	gcc -c $(CFLAGS) admin.c -o admin.o

builder.o: builder.c builder.h utility/config.o utility/config.h
	gcc -c $(CFLAGS) builder.c -o builder.o

game_runner.o: game_runner.h game_runner.c
	gcc -c $(CFLAGS) game_runner.c -o game_runner.o

game_scheduler.o: game_scheduler.h game_scheduler.c
	gcc -c $(CFLAGS) game_scheduler.c -o game_scheduler.o

server2.o: server2.c utility/shutdown.o net/http.o utility/registry.o utility/config.o dstruct/vector.o net/tcp_server.o builder.o builder.h
	gcc -c $(CFLAGS) server2.c $(CLIBS) -o server2.o

game1: game.c game/game_api.h libgame_api.so
	gcc $(CFLAGS) game.c -L. -lgame_api -o game1

rps: rockpaperscissors/rockpaperscissors.c game/game_api.h libgame_api.so
	gcc $(CFLAGS) rockpaperscissors/rockpaperscissors.c -L. -lgame_api -o rps

init.o: init.h init.c
	gcc -c $(CFLAGS) init.c -o init.o

scheduler.o: scheduler.h scheduler.c
	gcc -c $(CFLAGS) scheduler.c -o scheduler.o

testscheduler: scheduler.o testscheduler.c
	gcc $(CFLAGS) testscheduler.c scheduler.o -lpthread -o testscheduler