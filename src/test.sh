#!/bin/bash

cp libgame_api.so ../environment/libgame_api.so
cp rps ../environment/rps

cd ../environment

valgrind --leak-check=full --show-leak-kinds=all ../development/GameCMS