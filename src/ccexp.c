/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Matthew Maynes
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
 
#include "ccargs.h"

#define __C_PROMPT__ "> "
#define __PROG_VERSION__ "v0.1"
#define __AUTHOR__ "Matthew Maynes"

int quit;

static ccmd commands[] ={
	{"about", 0, 0, 'a'},
	{"echo", 1, 0, 'e'},
	{"exit", 0, &quit, 'x'},
	{"help", 0, 0, 'h'}
};

void console(ccmd cmds[]);
void help(void);
void about(void);

int main(int argc, char* argv[]){
	quit = 0;	
	console(commands);
	return 0;	
}

void console(ccmd cmds[]){
	char cmd;
	about();
	printf("Type 'help' for instructions\n");
	while(!quit){
		cmd = get_cc(cmds, 4, __C_PROMPT__);
		switch(cmd){
		case 'a':
			about();
			break;
		case 'e':
			printf("%s\n", cmdarg);
			break;
		case 'h':
			help();
			break;
		case 'x':
			printf("Bye!\n");
			break;
		case ':':
			printf("Invalid argument count for function %s\n", cmdopt);
			break;
		case '?':
		default:
			printf("Unrecognized command %s\n", cmdopt);
			break;
		
		}
	}
}

void about(void){
	printf("CCExample %s. Created by: %s\n", __PROG_VERSION__, __AUTHOR__);
}

void help(void){
	char msg[] = "CCExample Help\n\
\n\
	This is a simple console input program to demonstrate the usage of the\n\
	ccargs code. There isn't much to do but you can try the following.\n\
\n\
	  [Commands]\n\
\n\
		about		Displays a version and author message about this example\n\
\n\
		echo [str] 	Echos the given string to demonstrate a single parameter\n\
				function\n\
\n\
		exit		Quits the program\n\
\n\
		help		Displays this message\n\
\n";
	printf("%s", msg);
}