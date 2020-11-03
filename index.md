## What is GameCMS?
GameCMS (short for Game Content Management System) is a framework for creating and managing programming competitions. Inspired by competions where programs compete in real time, GameCMS was made to be fast and flexible. GameCMS is heavily inspired by [CodinGame](https://www.codingame.com/) and MIT's [Battlecode](https://battlecode.org/).

As of right now, the whole library (except for a few Bash scripts) is written in C. Although this may ensure a fast application in some cases, this most likely makes it unsecure.

## Getting Started

### [Set up GameCMS](setup)
Set up a GameCMS environment on your own machine.

### [Tutorials](tutorials)
What's better than a tutorial? Nothing! So dive into a place of magic!

### [Documentation](reference)
Read about all the functions that you can use in contest scripts

## Futuer Plans for GameCMS
The GameCMS library is not even close to being finished! There are many fixes and additions that need to take place before it becomes a usable program. The entire program was written in C by an undergraduate who has no experience with writing secure applications... so the security in GameCMS is questionable. Giving this burden to other libraries that have stood the test of time will have dramatic improvements not only to the security, but also to development time.

* Replace the built-in "database" with [MySQL](https://www.mysql.com/) or [SQLite](https://www.sqlite.org/index.html) implementations.
* Rewrite the webserver in Python using the [Flask](https://flask.palletsprojects.com/en/1.1.x/) web framework.
