#include "ccargs.h"

#define __NULL__ '\0'

#define __UNKNOWN_CMD__ '?'
#define __INVALID_ARGC__ ':'

#define __ESCAPE_CHAR__ '\\'
#define __QUOT__ '"'
#define __APOS__ '\''

#define __LINE_TERM__ '\n'
#define __REG_SIZE__ 3


char cmdopt[__MAX_CMD_LEN__];
char cmdarg[__TOTAL_ARG_LEN__];

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
 * Clean
 * @private
 *
 * Removes any escape characters and quotes from the given string
 *
 * @param *str The string to clean
 */
void _clean(char* str);

/**
 * Erase
 * @private
 *
 * Cleans a string by filling it with 0's
 *
 * @param *str The string to fill
 * @param n The number of characters to change to 0
 */
void _erase(char* str, int n);

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

/**
 * String Copy
 * @private
 *
 * Copys a string from the source to the destination up to the maximum
 * number of characters. The return is the number of characters that were
 * copied
 *
 * @param *dest The destination character array
 * @param *src The source character array
 * @param max The maximum number of characters to copy
 *
 * @return The number of copied characters or -1 on error
 */
int _strncpy(char* dest, char* src, int max);

/**
 * String Length
 * @private
 *
 * Counts the number of characters in a string until it finds the 
 * null terminator '\0'
 *
 * @param *str The string to count valid characters
 *
 * @return The number of chracters or -1 on error
 */
int _strlen(char* str);

/**
 * String Equals
 * @private
 *
 * Checks if the two strings are equal. If the string match then there
 * is a return of 1. If they are not equal then there is a return of 0.
 * If there is an error such as null pointers then there is a return of -1
 *
 * @param *base The base string to compare
 * @param *cmp The string to compare to
 * 
 * @return 1 if equal, 0 if not equal and -1 on error
 */
int _strequ(char* base, char* cmp);

/**
 * Shift Right
 * @private
 *
 * Shifts the leftmost character of the register into the index 
 * slot in the buffer. The register is the shifted to the left one.
 * The void created by the shift is filled by the fill character.
 * The registers are shifted n times. If n is greater then 1 then 
 * for each subsequent shift, index is incremented.
 * 
 * **Note: It is assumed that buffer has enough space to have n
 * characters put into it.
 *
 * @param *buffer The space to put the shifted character into
 * @param *reg The register to shift out of
 * @param size The size of the register
 * @param fill The default character to shift into the right of the register
 * @param n The number of shifts to perform
 * @param index The index of the buffer to insert the new character from the register
 *
 * @return 0 If the shift was successful or -1 on error
 */
int _shiftl(char* buffer, char* reg, int size, char fill, int n, int index);

char get_cc(ccmd* cmds, int argc, char* prompt){
	char buffer[__MAX_CMD_LEN__ + __TOTAL_ARG_LEN__];
	char reg[__REG_SIZE__]; char c;
	int clen = 0; char valid = 0;
	int i = -__REG_SIZE__;
	*cmdopt = __NULL__;
	*cmdarg = __NULL__;

	_erase(buffer, __MAX_CMD_LEN__ + __TOTAL_ARG_LEN__);
	while(clen == 0){
		// If there is a prompt to print then print it
		if(prompt != 0) fprintf(stdout, "%s", prompt);	
	
		 _erase(reg, __REG_SIZE__); 
		c = fgetc(stdin);
		while(i < __MAX_CMD_LEN__ + __TOTAL_ARG_LEN__ && c != __LINE_TERM__){
			_shiftl(buffer, reg, __REG_SIZE__, c, 1, i);
			c = fgetc(stdin); 
			i++;
		}
		_shiftl(buffer, reg, __REG_SIZE__, __NULL__, __REG_SIZE__, i);	
		_trim(buffer);
		clen = _strlen(buffer);
	}
	_init_cmds(buffer, clen);
	
	i = 0;
	while(i < argc){
		valid = valid_ccmd(cmds[i]);
		if(valid == 1){
			_clean(cmdarg);
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
	char arg[__MAX_ARG_LEN__];
	while(get_carg(arg, __MAX_ARG_LEN__, count) > 0){
		count++;
	}
	
	return count;
}


int valid_ccmd(ccmd opt){	 
	if(_strequ(cmdopt, opt.cmd))
	 	return opt.argc == count_carg() ? 1 : -1;
	return 0;
}

void _init_cmds(char* cmdcmd, int clen){
	int olen = 0; int alen = 0;
	olen = _cmdtok(cmdcmd, cmdopt, __MAX_CMD_LEN__, 0);
	alen = 1 + clen - olen;
	_strncpy(cmdarg, cmdcmd + olen, alen);
	_trim(cmdarg);
}

int _cmdtok(char* vec, char* ccmd, int max, int index){
	int cmdlen = 0; int tokc = 0; int ci = 0;
	int quot = 0; int apos = 0; int escp = 0;
	
	// Short circuit if there is an issue
	if(ccmd == 0) return -1;
	if(vec == 0 || *vec == __NULL__) return 0;
	
	// Get the length of the command vector	
	cmdlen = _strlen(vec);
	
	// Loop over each character
	for(int i = 0; i < cmdlen && ci < max - 1; i++){
		ccmd[ci] = 0; escp = 0;
		if(vec[i] == __ESCAPE_CHAR__){
			escp = 1;
			i++;
		}

		if(!escp && vec[i] == __QUOT__) quot = !quot & !apos;  
		if(!escp && vec[i] == __APOS__) apos = !apos & !quot;  
		// If we hit a token delemeter
		if(!escp && (vec[i] == ' ' || vec[i] == '\t') && !quot && !apos){
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
	ccmd[ci] = 0;
	return ci;
}

int _iswhitespace(char c){
	return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

void _trim(char* str){
	if(str == 0) return;
	char* p = str;
	int len = _strlen(str);

	// Trim front
	while(*p != 0 && _iswhitespace(*p)){
		p++;
	}
	
	// Trim end
	int i = _strlen(p) - 1;
	while(i > 0 && _iswhitespace(p[i])){
		p[i] = __NULL__;
		i--;
	}

	_strncpy(str, p, len);
}

void _clean(char* str){
	char* p = str;
	int len = _strlen(str);
	while(*p != __NULL__){
		if(*p == __ESCAPE_CHAR__){
			_shiftl(str, p, len, __NULL__, 1, p - str);
		}
		p++;
	}
}

int _shiftl(char* buffer, char* reg, int size, char fill, int n, int index){
	char* r = reg;
	char* p = buffer + (index > 0 ? index : 0);
	if(buffer == 0 || reg == 0) return -1;
	if(n == 0) return 0;
	*p = *r;
	r++;
	while(r < reg + size){
		*(r - 1) = *r;
		r++;
	}
	*(--r) = fill;
	return _shiftl(buffer, reg, size, fill, n - 1, index + 1);
}

int _strncpy(char* dest, char* src, int max){
	int i = 0;
	if(dest == 0 || src == 0) return -1;
	while(i < max  && src[i] != __NULL__){
		dest[i] = src[i];
		i++;
	}
	dest[i] = __NULL__;
	return 0;

}

int _strlen(char* str){
	int i = 0;
	if(str == 0) return -1;
	while(str[i] != '\0') i++;
	return i;
}

int _strequ(char* base, char* cmp){
	int i = 0; 
	if(base == 0 || cmp == 0) return -1;
	while(base[i] != '\0' && cmp[i] != '\0' && base[i] == cmp[i]) i++;
	return base[i] == '\0' && cmp[i] == '\0';
}

void _erase(char* str, int n){
	while(n > 0){
		n--;
		str[n] = '\0';
	}
}
