[Home](index.md) / [Tutorials](tutorials.md) / How to Create a Game Script

# How to Create a Game Script

## Introduction

In order to run a contest, we need to know what type of game we want our programs to compete in. If you have an idea for a game you want to implement, it would be a good idea to learn how to create a *game script*.

A game script is simply a file that runs the processes the game logic for the contest. If you want to have players compete in a racing game, then you define how you want the players to drive their vehicles in the game script. The game script is also responsible for assigning scores to players so that they be ranked at the end of the contest.

Thankfully, creating a game script is very simple. There are less than 20 functions that are used to create game scripts (those functions can be found [here](reference.md))

## The Tutorial

1. The first step, if you have not already, is to [set up your GameCMS environment](setup.md).
2. Next, you will need to create a new source file in your language of choice. The language you choose is important! It must be able to load a C library file and call those functions. This tutorial will write the game script in C, which is what GameCMS is written in. If you want to learn how to create a game script for another language, look at the list below:
    - Python ...
    - Java ...

3. In order to make the building process easier, let's create a make file called ```Makefile```. In that file, copy and paste in the text below:
```make

CFLAGS = -

file: script.c libgame_api.so
	gcc $(CFLAGS) script.c -L. -lgame_api -o script

```

This will hide the complex command for us. All we have to do is call ```make``` in a shell.

4. 
