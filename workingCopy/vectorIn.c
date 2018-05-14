/*
 *===============================================================================/
 * Author	:	Ben Haubrich						//
 * File		:	vectorIn.c						//
 * Synopsis	:	Contains all the functions that deal with user input	//
 *===============================================================================/
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

/*Local headers*/

/* 
 * gets new options from standard in and places them back in argv for
 * processing
 * param argv: The argument vector that contains all the arguments
 * return: The new number of arguments
 * postcond: argv is refreshed with new arguments. The old ones are overwritten.
 * if there place needs to be taken
 */
int refreshArgv(char *argv[]) {

	/*get new arguments from stdin (next 9 lines)*/
	char *newOptions = malloc(sizeof(char));
	
	/*
	 * Check to see if stdin is coming from the terminal. If it's not, then
	 * it will cause printf to buffer up, then dump when the file is out of
	 * lines.
	 */
	if(isatty(STDIN_FILENO)) {
		
		printf("vecalc: ");
	}
	
	/*
	 * Using 100 as the maximum possible for no particular reason. Just
	 * need enough to make sure we can handle any combination of args
	 */
	fgets(newOptions, 100, stdin);

	/*
	 * fgets processes the string when the user presses enter, but
	 * pressing enter also sends in a newline character. It is not needed.
	 * so we'll copy all but the last byte. 
	 */

	char *temp = malloc(sizeof(newOptions));
	strncpy(temp, newOptions, (strlen(newOptions)) - 1);
	newOptions = temp;

	/*
	 * nextArg stores space delimited arguments from newOptions and is then 
	 * used to insert into argv. Using 11 since the largest single argument possible
	 * is the largest possible number - UINT_MAX. which is 10 characters long, plus
	 * a newline.
	 */
	char *nextArg = malloc(11*sizeof(char));

	 /*
	  * The first argument of argv is taken by then name of the program. j fullfills
	  * two purposes:
	  * The first is keeping track of which element of argv we should insert into.
	  * The second is counting how many arguments were added, so that we can use
	  * it to update argc.
	  */ 
	int j = 1;

		/*Nothing to evaluate if this is true*/	
		if(strcmp(newOptions, "") == 0) {

			return j;
		}
		
		/*get all the space delimited arguments and put them in argv*/
		char *delim = " ";
		nextArg = strtok(newOptions, delim);

		while(nextArg != NULL) {

			/*
			 * In case vecalc was called with no arguments (not even a space), or gets more
			 * arguments than it had last time, we'll have to check for null elements.
			 */
			if(argv[j] == NULL) {

				argv[j] = calloc(1, sizeof(char));
			}
			/*Clear and allocate anyway for copying*/
			else {
				
				argv[j] = NULL;
				argv[j] = calloc(1, sizeof(char));
			}

			strncpy(argv[j], nextArg, strlen(nextArg));
			j++;
			/*
			 * With the null input string, strtok continues on from the old string, and
			 * keeps copying more characters until the next delimiter. If there are no
			 * more delimiters, strtok returns null
			 */
			nextArg = strtok(NULL, delim);
		}
	free(nextArg);
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

	if(arg == NULL) {

		return false;
	}

	if(isdigit(*arg) > 0) {

		return true;
	}
	else {

		return false;
	}
}
