#!/bin/bash
# Runs the contest executable

LD_LIBRARY_PATH=.
export LD_LIBRARY_PATH

valgrind -v --leak-check=full ./contest