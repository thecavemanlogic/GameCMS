[Home](index.md) / Setting Up GameCMS

# Setting Up GameCMS on Your Own Machine

GameCMS was orginilly built on top of an Ubuntu distro for Window's Subsystem for Linux (WSL). So, this tutorial assumes that you have a similar environment at hand. There are some other prerequisites as well, which are listed below:
* OpenSSL library
* BSD string library
* GNU C Compiler

If you are unsure if you have these dependencies installed, execute the following commands in a shell:
```bash
sudo apt-get install libssl-dev
sudo apt-get install libbsd-dev
sudo apt-get install gcc
```

Next, clone the repository by typing the following into a shell:
```git clone https://github.com/thecavemanlogic/GameCMS.git```

Move into the ```src``` directory by executing ```mkdir GameCMS/src``` and then build the program by executing ```make```.

If the program built sucessfully, make will return no errors after ran. You can also perform a test contest by executing ```./test.sh``` in the shell. This script creates a temporary directory to host the contest and then runs the contest in that directory.
