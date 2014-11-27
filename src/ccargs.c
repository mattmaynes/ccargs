#include "ccargs.h"
#include <string.h>
#include <stdlib.h>

#define __UNKNOWN_CMD__ '?'
#define __INVALID_ARGC__ ':'

char* cmdopt;
char* cmdarg;
char* cmdcmd;

/**
 * Count Command Vector
 * @private
 *
 * Counts the number of command structs in the command vector. If the vector
 * is 0 then the return is 0.
 * 
 * @param cmds[] The command vector to count
 *
 * @return The number of command structures in the command vector
 */
int _ccmdvec(ccmd* cmds);

/**
 * Free Commands
 * @private
 *
 * Frees the global 'cmdopt', 'cmdcmd' and 'cmdarg' if they are allocated
 */
void _free_cmds(void);

/**
 * Initialize Commands
 * @private
 *
 * Allocates and initializes the cmdopt and cmdarg based on cmdcmd. 
 * 
 * @param The length of the command vector cmdcmd
 */
void _init_cmds(int clen);

/**
 * Trim
 * @private
 * 
 * Trims the white space off the start and end of the given string
 * 
 * @param *str The string to trim
 */
void _trim(char* str);

/**
 * Is Whitespace
 * @private
 *
 * Returns 1 if the given character is white space. White space includes
 * the following characters:
 *   ' '  space
 *   '\t' tab
 *   '\n' newline
 *   '\r' linefeed
 *
 * @param c The character to check
 * 	
 * @return 1 if the character is whitespace or else 0
 */
int _iswhitespace(char c);

/**
 * Command Token
 * @private
 *
 * Retrieves a token from the 'cmdcmd' global variable. The ccmd parameter
 * will be populated with the token at the given index. 
 * **Note it is assumed that the carg parameter has enough space
 * allocated for the name of the entered argument up to the max size.
 *
 * @param *ccmd The command token to be returned
 * @param max The maximum number of character to put in the ccmd buffer
 * @param index The index of the token to grab
 *
 * @return The size of the ccmd if successful or else -1 on error
 */
int _cmdtok(char* ccmd, int max, int index);

char get_cc(ccmd* cmds, char* prompt){
	char buffer[__MAX_ARG_LEN__ * __MAX_ARGS__];
	int argc = _ccmdvec(cmds);
	int clen = 0;
	int i = 0;
	_free_cmds();
	
	while(clen == 0){
		// If there is a prompt to print then print it
		if(prompt != 0) fprintf(stdout, "%s", prompt);	
		
		// Short circuit if the stream is closed
		if(fgets(buffer, __MAX_ARG_LEN__ * __MAX_ARGS__, stdin) <= 0) return -1;
		_trim(buffer);
		clen = strlen(buffer);
	}
	
	cmdcmd = (char*)malloc(sizeof(char) * clen);
	strncpy(cmdcmd, buffer, clen);
	_init_cmds(clen);
	
	while(i < argc && !valid_ccmd(cmds[i])) i++;
	
	if(cmds[i].scmd != __UNKNOWN_CMD__ && cmds[i].scmd != __INVALID_ARGC__)
		if(cmds[i].flag != 0) *(cmds[i].flag) = 1;
		
	return cmds[i].scmd;
	
}

int get_carg(char* carg, int max, int index){
	return _cmdtok(carg, max, ++index);
}

int count_carg(void){
	int count = 0;
	char *arg = (char*)malloc(sizeof(char) * __MAX_ARG_LEN__);
	while(get_carg(arg, __MAX_ARG_LEN__, count) > 0){
		count++;
	}
	
	free(arg);
	return count;
}


int valid_ccmd(ccmd opt){	 
	return strcmp(cmdopt, opt.cmd) == 0 && opt.argc == count_carg();
}

int _ccmdvec(ccmd* cmds){
	return cmds == 0 ? 0 : sizeof(cmds)/sizeof(ccmd);
}

void _free_cmds(void){
	if(cmdcmd > 0) free(cmdcmd);
	if(cmdopt > 0) free(cmdopt);
	if(cmdarg > 0) free(cmdarg);
}

void _init_cmds(int clen){
	int olen = 0; int alen = 0;
	cmdopt = (char*)malloc(sizeof(char) * __MAX_CMD_LEN__);
	_cmdtok(cmdopt, __MAX_CMD_LEN__, 0);
	
	olen = strlen(cmdopt);
	alen = clen - olen;
	cmdarg = (char*)malloc(sizeof(char) * alen);
	strncpy(cmdarg, cmdcmd + olen , alen);
	_trim(cmdarg);
}

int _cmdtok(char* ccmd, int max, int index){
	int cmdlen = 0; int tokc = 0; int ci = 0;
	
	// Short circuit if there is an issue
	if(cmdcmd == 0) return 0;
	if(ccmd == 0) return -1;
	
	// Get the length of the command vector	
	cmdlen = strlen(cmdcmd);
	
	// Loop over each character
	for(int i = 0; i < cmdlen; i++){
		if(ci < max){
			// If we hit a token delemeter
			if(cmdcmd[i] == ' ' || cmdcmd[i] == '\t'){
				tokc++;
			}
			// If this is the end of the string
			else if(cmdcmd[i] == '\n' ||  cmdcmd[i] == '\r' || cmdcmd[i] == '\0'){
				ccmd[ci] = '\0';
				return ci;
			}
			
			// If this is the correct token
			if(tokc == index){
				ccmd[ci] = cmdcmd[i];
				ci++;
			}
			else if(tokc > index){
				ccmd[ci] = '\0';
				return ci;
			}
		}
		else{
			ccmd[ci] = '\0';
			return ci;
		}
	}
	return 0;
}

int _iswhitespace(char c){
	return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

void _trim(char* str){
	if(str == 0) return;
	char* p = str;
	int len = strlen(str);

	// Trim front
	while(*p != 0 && _iswhitespace(*p)){
		p++; len--;
	}
	
	// Trim end
	int i = strlen(p);
	while(i >= 0 && _iswhitespace(p[i])) p[i--] = '\0';

	i = 0;
	while(i < len) str[i++] = *(p++);
	
}