#!/bin/bash
# Runs the game executable

LD_LIBRARY_PATH=.
export LD_LIBRARY_PATH

echo Passing $1 to the game...

./rockpaperscissors $1

# valgrind --vgdb-poll=0 --leak-check=full ./rockpaperscissors $1
