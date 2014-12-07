#CCArgs

![image](https://travis-ci.org/mattmaynes/CCArgs.svg?branch=master)

###Console Command with Arguments

This is a simple console command parser that reads commands from `stdin` and checks their validity against a command vector. The syntax to use `ccargs` is based off of `getopt` syntax and can be used in the same way.

Every console command has a *command identifier* and an argument count that is used for validating user input. To retrieve a command from the user use `get_cc` which takes in a command vector as well default prompt string and returns a single character indicating the user input.

There is an example program attached called `ccexp.c` which demonstrates the usage of ccargs. 

##Compiling
There is a make file attached to the project. To make just the object file output simply run `make` from the root directory. To make the given example then run `make example`.
