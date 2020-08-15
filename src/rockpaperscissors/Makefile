
all: lib rockpaperscissors contest

contest: contest.c libcontest_api.so
	gcc -g contest.c -L. -lcontest_api -lbsd -o contest

rockpaperscissors: rockpaperscissors.c libgame_api.so game_api.h
	gcc -g rockpaperscissors.c -L. -lgame_api -o rockpaperscissors

# Move libraries to current directory
lib: libgame_api.so libcontest_api.so

libgame_api.so: ../libgame_api.so game_api.h
	cp ../libgame_api.so .

game_api.h: ../game/game_api.h
	cp ../game/game_api.h .