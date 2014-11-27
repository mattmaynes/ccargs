#ifndef CCARGS_H
#define CCARGS_H

#include <stdio.h>

#define __MAX_CMD_LEN__ 128
#define __MAX_ARG_LEN__ 128
#define __MAX_ARGS__ 10
#define __TOTAL_ARG_LEN__ __MAX_ARG_LEN__ * __MAX_ARGS__

/**
 * Points to the last invoked console command
 */
extern char cmdopt[__MAX_CMD_LEN__];

/**
 * Points to the last invoked commands argument vector
 */
extern char cmdarg[__TOTAL_ARG_LEN__];

/**
 * The command structure for validating console commands
 *
 * cmd The command that is to be accepted as a valid command
 * argc The number of required arguments that this command takes
 * *flag A flag that can be set as a result of this command
 * scmd A short form of the command that will be returned from get_cc if this command is invoked
 */
typedef struct ccmd_t{
	char cmd[__MAX_CMD_LEN__];
	int argc;
	int* flag;
	char scmd;
}ccmd;

/**
 * Get Console Command
 *
 * Reads the next command that is input to the given in the standard input stdin and 
 * returns the matching short command from the command structure array. If an invalid 
 * command is entered then a special character is returned. The long version of the 
 * most recent command will always be stored in an external variable called 'cmdopt' 
 * and can be accessed using get_copt(). The most recent command argument vector will
 * also be stored in a global variable called 'cmdarg'. To break up the arguments of 
 * a command see get_carg().
 *
 * In the case that the flag field of the console command structure has a pointer,
 * it will be set if the command is deemed valid. A command is valid if it has the 
 * correct number of arguments.
 *
 * @param *cmds The valid commands that can be parsed from the input stream
 * @param argc The number of command options in the cmds array.
 *
 * @param *prompt 
 * An optional prompt to print before the user enters a command.
 * The prompt will be printed on the standard output stream stdout.
 * If 0 then nothing will be printed.
 * @end 
 *
 * @return 
 * A character that represents the short version of the entered command. If a command
 * is entered that is not in the command structure or a command that requires agruments
 * has none then there is a special return. 
 *
 * If the given command is not in the command structure then there is a return of '?'
 * If there is a command that requires parameters but has none then there is a return of ':'
 * If an error occurs or the stream cannot be read then there is a return of -1
 * @end
 * 
 */
char get_cc(ccmd* cmds, int argc, char* prompt);

/**
 * Get Console Argument
 *
 * Tokenizes the global 'cmdarg' value on spaces and returns the next available 
 * console argument for the last command. If there are no arguments left then the 
 * return is 0. The index parameter indiciates which argument is desired in the
 * argument vector. The return of the function is the number of characters in the 
 * argument name. **Note it is assumed that the carg parameter has enough space
 * allocated for the name of the entered argument up to the max size.
 *
 * @param *carg An array with enough space to hold the next argument
 * @param max The maximum size of carg to fill
 * @param index The index of the requested argument in the argument vector
 *
 * @return 
 * The number of characters in the carg array that have been returned. 
 * If the index requested is out of bounds of the argument vector then a
 * there is a return of 0 and cargs first index will be 0.
 * If carg is a NULL pointer or there is an error -1 is returned
 * @end
 *
 */
int get_carg(char* carg, int max, int index);

/**
 * Count Command Arguments
 *
 * Returns the count of the number of arguments entered by the last command.
 * This value is the number  of arguments in the global argument vector seperated
 * by spaces.
 *
 * @return The number of space seperated arguments in the last console argument
 */
int count_carg(void);

/**
 * Validate Console Command
 *
 * Checks if the given console command against the last entered console command. 
 * If this command option is valid then it returns with 1. If the command option
 * is not not valid then there is a return of 0. If the argument count does not
 * match the required number then -1 is returned
 *
 * @param opt A console command option to validate against the last entered command
 *
 * @return 
 * 1 If the option is valid
 * 0 If the option is invalid
 * -1 If the option matches but has the wrong number of arguments
 * @end 
 */
int valid_ccmd(ccmd opt);

#endif
