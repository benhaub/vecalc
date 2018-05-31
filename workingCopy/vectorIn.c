/*
 *==============================================================================//
 * Author	:	Ben Haubrich						//
 * File		:	vectorIn.c						//
 * Synopsis	:	Contains all the functions that deal with user input	//
 *==============================================================================//
 */

/*Standard Headers*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> /*For string manipulation in refreshArgv()*/
#include <ctype.h>  /*For isdigit()*/
#include <unistd.h> /*For adjusting input when stdin comes from
		     * a file, and not the terminal
		     */

/*Local Headers*/
#include "vectorMem.h" /*For checkAlloc()*/
#include "vectorIn.h" /*For userIn()*/

/* 
 * gets new options from standard in and places them back in argv for
 * processing. 
 * param argv: The argument vector that contains all the arguments
 * param int: The maximum number of arguments that has been used
 * param int: The initial amount of arguments the program was ran with
 * return: The new number of arguments
 * postcond: argv is refreshed with new arguments. The old ones are overwritten.
 * if there place needs to be taken
 */
int refreshArgv(char *argv[], int maxArgc, int initialArgc, int currentArgc) {

	char *newOptions = userIn();

	/*
	 * The first argument of argv is taken by then name of the program 
	 * (so we start at 1, not 0). j fullfills two purposes:
	 * The first is keeping track of which element of argv we should insert 
	 * into. The second is counting how many arguments were added, so that 
	 * we can use it to update argc.
	 */
	int j = 1;

	/*
	 * Nothing to evaluate if this is true. This can happen if we are given
	 * a blank line as input from redirected input.
	 */	
	if(strcmp(newOptions, "") == 0) {

		argv[j] = "";
		return j;
	}

	/*
	 * fgets (from the userIn function) processes the string when the user
	 * presses enter, but pressing enter also sends in a newline character.
	 * It is not needed. so we'll copy all but the last byte.
	 */ 
	char *temp = calloc(strlen(newOptions), sizeof(newOptions));
	strncpy(temp, newOptions, (strlen(newOptions)) - 1);
	memset(newOptions, 0, strlen(newOptions));
	memcpy(newOptions, temp, strlen(temp));
	free(temp);
	temp = NULL;
	
	/*
	 * If the user didn't specify any additional arguments, then we are
	 * left with just r, and we can return right away because we are just
	 * re-using the same argv
	 */
	if(strcmp(newOptions, "r") == 0) {

		free(newOptions);
		return currentArgc;
	}

	/*
	 * nextArg takes the next space delimited string and stores it in the
	 * next open spot in argv. Any string placed in nextArg will be no
	 * greater than the size of newOptions
	 */
	char *nextArg;

	/*get all the space delimited arguments and put them in argv*/
	char *delim = " ";
	nextArg = strtok(newOptions, delim);
	
	if(nextArg != NULL && strcmp(nextArg, "r") == 0) {

		/*
	 	* If the r option is given, we want j to be positioned such that
		* the the commands entered by the user in newOptions are
		* appended to the commands issued last time, so that they are
		* repeated as well as any additional commands specified.
		*/		
		j = currentArgc;
		
		/*Don't include the r option in argv*/
		nextArg = strtok(NULL, delim);
	}

	while(nextArg != NULL) {

		if(argv[j] == NULL) {

			argv[j] = calloc(strlen(nextArg), sizeof(char));
		}

		/*
		 * If j is less than the maximum args we've used and greater
		 * than the number of arguments vecalc was initialized with, 
		 * then we are dealing with memory that we've allocated
		 */
		else if(j >= initialArgc && j < maxArgc) {

			free(argv[j]);	
			argv[j] = calloc(strlen(nextArg), sizeof(char));
		}

		/*
		 * We don't own this memory, we should overwrite the location
		 * with one that we do own
		 */
		else if(j >= maxArgc) {

			argv[j] = calloc(strlen(nextArg), sizeof(char));
		}

		/*
		 * We can't realloc this memory, so we'll clear it's contents
		 * for new arguments
		 */
		else if(j < initialArgc) {
			
			argv[j] = memset(argv[j], 0, strlen(argv[j]));
		}
		strncpy(argv[j], nextArg, strlen(nextArg));
		j++;

		/*
		 * With the null input string, strtok continues on from the old
		 * string, and keeps copying more characters until the next 
		 * delimiter. If there are no more delimiters, strtok returns null
		 */
		nextArg = strtok(NULL, delim);
	}
	free(newOptions);

return j;
}

/*
 * Checks to see if the argument is a digit or not
 * param arg: The current argument that needs to be checked
 * return: true if it is a digit, false otherwise
 * precond: input parameter is not null
 */
bool ensureDigit(char *arg) {

	int decimalCount = 0;
	int subtractCount = 0;
	
	/*
	 * isdigit considers blank string numbers apparently. This is probably
	 * caused by using memset
	 */
	if(arg == NULL || strcmp(arg, "" ) == 0) {

		return false;
	}
	
	size_t i;
	for(i = 0; i < strlen(arg); i++) {
	
		/*
		* isdigit obviously doesn't count decimal poits or negative signs
		* as digits, so we'll allow at least one decimal or negative sign to
		* be there, but any more than that wouldn't be an actual floating 
		* point number or negative number.
		*/
		if(arg[i] == '-') {

			subtractCount++;
		
			if(decimalCount > 1 || subtractCount > 1) {

				return false;
			}
			continue;
		}	
		if(arg[i] == '.') {

			decimalCount++;
		
			if(decimalCount > 1 || subtractCount > 1) {

				return false;
			}
			
			continue;
		}
	
		if(isdigit(arg[i]) > 0) {

			continue;	
		}	
		else {

			return false;
		}
	}
return true;
}

/*
 * Accepts input from the user.
 * return: A dynamically allocated string that holds the users ne input
 * precond: char * is not null and is dynamically allocated
 * postcond: The returned string should be freed when it is no longer needed.
 * Returned strings will have newlines on the end of them, be sure to trim it
 * if necessary
 */
char *userIn() {

	char *newOptions = malloc(MAX_INPUT_LENGTH*sizeof(char));
	checkAlloc(newOptions);

	/*
	 * Check to see if stdin is coming from the terminal, and only print
	 * the vecalc prompt if it is.
	 */
	if(isatty(STDIN_FILENO) == 1) {
		
		printf("vecalc: ");
	}

	/*
	 * If the user enters in more than the max input length, fgets buffers
	 * the input and enters it on the next call for input from stdin.
	 */	
	fgets(newOptions, MAX_INPUT_LENGTH, stdin);

	/*
	 * EOF's on here-strings are always returned null by realloc.
	 */
	if(isatty(STDIN_FILENO) == 0 && strcmp(newOptions, "") == 0) {

		exit(EXIT_SUCCESS);	
	}
	
	/*
	 * Re-size newOptions to fit the input more optimally
	 */
	char *repositioned;
	repositioned = realloc(newOptions, strlen(newOptions)*sizeof(newOptions));
	checkAlloc(repositioned);

	/*
	 * realloc isn't always guarenteed to rellocate memory in place.
	 * If it had to move newOptions to a new location, we'll return that
	 * pointer instead. It is stated in realloc's documentation that:
	 * "If the new size of the memory object would require movement of the
	 * object, the space for the previous instantiation of the object is
	 * freed.". We will therefore not try to free newOptions since it's
	 * been done already.
	 */
	if(newOptions != repositioned) {

		newOptions = NULL;
		return repositioned;
	}
	else {

		return newOptions;
	}
}

/*
 * Removes values in argv from argv[argc] to argv[maxArgc] and
 * replaces them with null characters
 * param char[] *: the argument vector to be cleaned of unwanted values
 * param int: The current amount of arguments in argv
 * param int: The maximum amount of arguments in argv
 * precond: argv is not null
 * postcond: elements in argv greater that argc and less than the maximum
 * arguments used are cleared of existing values
 */
void cleanArgv(char *argv[], int argc, int maxArgc) {

	if(argv == NULL) {

		fprintf(stderr, "cleanArgv was given null argument vector\n");
	}

	int i;
	for(i = argc; i < maxArgc; i++) {

		argv[i] = memset(argv[i], 0, strlen(argv[i]));
	}
}
