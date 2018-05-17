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

/*Local Headers*/
#include "vectorMem.h"
#include "vectorIn.h"

/* 
 * gets new options from standard in and places them back in argv for
 * processing
 * param argv: The argument vector that contains all the arguments
 * return: The new number of arguments
 * postcond: argv is refreshed with new arguments. The old ones are overwritten.
 * if there place needs to be taken
 */
int refreshArgv(char *argv[]) {

	char *newOptions = calloc(185 ,sizeof(char));
	
	userIn(newOptions);

	/*
	 * The first argument of argv is taken by then name of the program. 
	 * j fullfills two purposes:
	 * The first is keeping track of which element of argv we should insert 
	 * into. The second is counting how many arguments were added, so that 
	 * we can use it to update argc.
	 */ 
	int j = 1;

	/*Nothing to evaluate if this is true*/	
	if(strcmp(newOptions, "") == 0) {

		/*strcpy(argv[j], newOptions);*/
		argv[j] = "";
		return j;

	}

	/*
	 * fgets processes the string when the user presses enter, but
	 * pressing enter also sends in a newline character. It is not needed.
	 * so we'll copy all but the last byte. 
	 */
	char *temp = calloc(strlen(newOptions), sizeof(newOptions));
	strncpy(temp, newOptions, (strlen(newOptions)) - 1);
	newOptions = temp;

	/*
	 * nextArg takes the next space delimited string and stores it in the
	 * next open spot in argv
	 */
	char *nextArg = calloc(185, sizeof(char));

	/*get all the space delimited arguments and put them in argv*/
	char *delim = " ";
	nextArg = strtok(newOptions, delim);

	while(nextArg != NULL) {

		/*
		 * In case vecalc was called with no arguments 
		 * (not even a space), or gets more arguments than it had last 
		 * time, we'll have to check for null elements.
		 *
		 * main() only takes one character options, so that's all we'll
		 * give space for
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
		 * With the null input string, strtok continues on from the old
		 * string, and keeps copying more characters until the next 
		 * delimiter. If there are no more delimiters, strtok returns null
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

/*
 * Handles input from the user to make sure that it is safe to be further
 * processed by vecalc and that it's take up as little memory as possible.
 * param char *: string to hold the users new input
 * precond: char * is not null
 */
void userIn(char *newOptions) {

	/*
	 * Check to see if stdin is coming from the terminal. If it's not, then
	 * it will cause printf to buffer up, then dump when the file is out of
	 * lines.
	 *
	 * It's also not very useful to print the vecalc prompt if recieving 
	 * input from a file. Printing vecalc helps the user know that the
	 * program is still running and awaiting input
	 */
	if(isatty(STDIN_FILENO) == 0) {
		
		fflush(stdout);
	}
	else {

		printf("vecalc: ");
	}

	
	fgets(newOptions, 185, stdin);
	newOptions = realloc(newOptions, strlen(newOptions)*sizeof(newOptions));
}
