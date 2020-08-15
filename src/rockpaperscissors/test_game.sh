#!/bin/bash
# Runs the game executable

LD_LIBRARY_PATH=.
export LD_LIBRARY_PATH

echo Passing $1 to the game...

valgrind --leak-check=full ./rockpaperscissors $1
