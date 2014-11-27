#include "ccargs.h"
#include <string.h>
#include <stdlib.h>

#define __UNKNOWN_CMD__ '?'
#define __INVALID_ARGC__ ':'

char* cmdopt;
char* cmdarg;

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
 * @param *cmdcmd The complete console command vector
 * @param The length of the command vector cmdcmd
 */
void _init_cmds(char* cmdcmd,  int len);

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
 * The ccmd parameter will be populated with the token at the given index. 
 * **Note it is assumed that the carg parameter has enough space
 * allocated for the name of the entered argument up to the max size.
 *
 * @param *vec The vector to tokenize
 * @param *ccmd The command token to be returned
 * @param max The maximum number of character to put in the ccmd buffer
 * @param index The index of the token to grab
 *
 * @return The size of the ccmd if successful or else -1 on error
 */
int _cmdtok(char* vec, char* ccmd, int max, int index);

char get_cc(ccmd* cmds, int argc, char* prompt){
	char buffer[__MAX_ARG_LEN__ * __MAX_ARGS__];
	char* cmdcmd;
	int clen = 0; char valid = 0;
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
	_init_cmds(cmdcmd, clen);
	free(cmdcmd);
	
	while(i < argc){
		valid = valid_ccmd(cmds[i]);
		if(valid == 1){
			if(cmds[i].flag != 0) *(cmds[i].flag) = 1;
			return cmds[i].scmd;
		}
		else if(valid == -1){
			return __INVALID_ARGC__;
		}
		i++;
	}
	return __UNKNOWN_CMD__;
	
}

int get_carg(char* carg, int max, int index){
	return _cmdtok(cmdarg, carg, max, index);
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
	if(strcmp(cmdopt, opt.cmd) == 0)
	 	return opt.argc == count_carg() ? 1 : -1;
	return 0;
}

void _free_cmds(void){
	if(cmdopt > 0) free(cmdopt);
	if(cmdarg > 0) free(cmdarg);
}

void _init_cmds(char* cmdcmd, int clen){
	int olen = 0; int alen = 0;
	cmdopt = (char*)malloc(sizeof(char) * __MAX_CMD_LEN__);
	_cmdtok(cmdcmd, cmdopt, __MAX_CMD_LEN__, 0);
	
	olen = strlen(cmdopt);
	alen = clen - olen;
	cmdarg = (char*)malloc(sizeof(char) * alen);
	strncpy(cmdarg, cmdcmd + olen , alen);
	_trim(cmdarg);
}

int _cmdtok(char* vec, char* ccmd, int max, int index){
	int cmdlen = 0; int tokc = 0; int ci = 0;
	int quot = 0; int apos = 0;
	// Short circuit if there is an issue
	if(vec == 0) return 0;
	if(ccmd == 0) return -1;
	
	// Get the length of the command vector	
	cmdlen = strlen(vec);
	
	// Loop over each character
	for(int i = 0; i < cmdlen && ci < max; i++){
		ccmd[ci] = 0;
		if(vec[i] == '"') quot = !quot;
		if(vec[i] == '\'') apos = !apos;
		
		// If we hit a token delemeter
		if((vec[i] == ' ' || vec[i] == '\t') && !quot && !apos){
			tokc++;
		}
		// If this is the end of the string
		else if(vec[i] == '\n' ||  vec[i] == '\r' || vec[i] == 0){
			ccmd[ci] = 0;
			return ci;
		}
		
		// If this is the correct token
		if(tokc == index){
			ccmd[ci] = vec[i];
			ci++;
		}
		else if(tokc > index){
			ccmd[ci] = 0;
			return ci;
		}
	}
	return ci;
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
		p++;
	}
	
	// Trim end
	int i = strlen(p) - 1;
	while(i > 0 && _iswhitespace(p[i])){
		p[i] = '\0';
		i--;
	}

	i = 0;
	while(i < len && p <= (str + len)){
		str[i] = *p;
		i++;
		p++;
	}
}