GAMEAPI_OFLAGS = -Wall -Wextra -Wwrite-strings -g -fPIC
GAMEAPI_OFILES = game_api.o timer.o gamelog.o replay.o config.o

all: ../libgame_api.so testreplay

../libgame_api.so: $(GAMEAPI_OFILES)
	gcc -shared $(GAMEAPI_OFILES) -lpthread -lbsd -o ../libgame_api.so

game_api.o: game_api.h game_api.c
	gcc $(GAMEAPI_OFLAGS) -c game_api.c -o game_api.o

timer.o: timer.h timer.c
	gcc $(GAMEAPI_OFLAGS) -c timer.c -o timer.o

gamelog.o: gamelog.h gamelog.c
	gcc $(GAMEAPI_OFLAGS) -c gamelog.c -o gamelog.o

replay.o: replay.h replay.c
	gcc $(GAMEAPI_OFLAGS) -c replay.c -o replay.o

config.o: ../utility/config.h ../utility/config.c
	gcc $(GAMEAPI_OFLAGS) -c ../utility/config.c -o config.o

testreplay: replay.o testreplay.c
	gcc testreplay.c replay.o -o testreplay