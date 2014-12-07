#CCArgs

![image](https://travis-ci.org/mattmaynes/CCArgs.svg?branch=master)

###Console Command with Arguments

This is a simple console command parser that reads commands from `stdin` and checks their validity against a command vector. The syntax to use `ccargs` is based off of `getopt` syntax and can be used in the same way.

Every console command has a *command identifier* and an argument count that is used for validating user input. To retrieve a command from the user use `get_cc` which takes in a command vector as well as a default prompt string and returns a single character indicating the user input.

There is an example program attached called `ccexp.c` which demonstrates the usage of ccargs. 

##Downloading
There currently are no release versions of the code but there will be soon. For now you can download or clone this repository and use the code in the `src` folder.

##Compiling
There is a make file attached to the project. To make just the object file output simply run `make` from the root directory. To make the given example then run `make example`.

##Usage
The usage of `ccargs` is very similar to `getopt`. First you need an argument vector that will define all of the accepted console inputs a user can have. For example, a program that needs to print its status, help or exit may have the following.

```
int quit;

static ccmd commands[] ={
	{"exit", 0, &quit, 'x'},
	{"help", 0, 0, 'h'},
	{"status", 0, 0, 's'}
};
```

The `ccmd` structure has 4 fields:
- `char cmd[__MAX_CMD_LEN__]` This is the long name of the command the user will type
- `int argc` This is the bumber of arguments the given command requires. For a variable number of commands use a negative number
- `int* flag` If set to a valid pointer and the given command is used, this flag will be toggled
- `char scmd` This is the short version of the command that will be returned from `get_cc` if the command was entered correctly

To use this command array for user input you can call `get_cc` in a loop until the user wishes to exit.

```
  char buffer[128];
	char cmd;
	
	while(!quit){
		cmd = get_cc(cmds, 5, "> ");
		switch(cmd){
		case 'h':
			help();
			break;
		case 's':
			status();
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
```

For more examples on how to use `ccargs` look in the `example` directory for `ccexp.c`.


