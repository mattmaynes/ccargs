#
# Make CConsole 
#
# Author: Matthew Maynes
# Version: 0.2
#
# 'make'        	builds the object files for the project
# 'make example'	builds an example program that runs the ccargs code
# 'make clean'  	removes all .o and executable files
#

# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -std=c99 -Wall

# define the input and output folders
IN = src
EXP = example
OUT = bin

# define the C source files
SRCS = ccargs.c

# define the C object files 
OBJS = $(SRCS:.c=.o)

# define the executable file
EXAMPLE = ccexp

default: init objs

objs:
	$(CC) $(CFLAGS) -c $(foreach src, $(SRCS), $(IN)/$(src)) -o $(foreach obj, $(OBJS), $(OUT)/$(obj))
	
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

example: init
	$(CC) $(CFLAGS) $(EXP)/$(EXAMPLE).c $(foreach src, $(SRCS), $(IN)/$(src)) -o $(OUT)/$(EXAMPLE)

init:
	mkdir -p bin
	
clean:
	rm $(OUT)/*.o $(OUT)/$(EXAMPLE) $(IN)/*~ 
