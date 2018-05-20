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
 * processing. Also ensures as little memory allocation for new args as
 * possible.
 * param argv: The argument vector that contains all the arguments
 * param int *: The maximum number of arguments that has been used
 * param int *: The initial amount of arguments the program was ran with
 * return: The new number of arguments
 * postcond: argv is refreshed with new arguments. The old ones are overwritten.
 * if there place needs to be taken
 */
int refreshArgv(char *argv[], int maxArgc, int initialArgc) {

	char *newOptions = calloc(MAX_INPUT_LENGTH ,sizeof(char));
	checkAlloc(newOptions);

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
	 * next open spot in argv. Any string placed in nextArg will be no
	 * greater than the size of newOptions
	 */
	char *nextArg = calloc(strlen(newOptions), sizeof(char));

	/*get all the space delimited arguments and put them in argv*/
	char *delim = " ";
	nextArg = strtok(newOptions, delim);

	/*Holds memory locations from argv to be freed before overwriting them*/
	char *holdMem;

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

			holdMem = malloc(strlen(argv[j])*sizeof(char));
			holdMem = argv[j];	
			argv[j] = calloc(strlen(nextArg), sizeof(char));
			free(holdMem);
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

	int decimalCount = 0;
	int subtractCount = 0;
	
	/*
	 * isdigit considers blank string numbers apparently. This is probably
	 * caused by using memset
	 * 
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
 * Handles input from the user to make sure that it is safe to be further
 * processed by vecalc and that it's take up as little memory as possible.
 * param char *: string to hold the users new input
 * precond: char * is not null
 */
void userIn(char *newOptions) {

	/*
	 * Check to see if stdin is coming from the terminal, and only print
	 * the vecalc prompt if it is.
	 */
	if(isatty(STDIN_FILENO) == 1) {
		
		printf("vecalc: ");
	}

	/*
	 * TODO:
	 * What happens for input greater than or equal to MAX_INPUT_LENGTH
	 */	
	fgets(newOptions, MAX_INPUT_LENGTH, stdin);
	/*Re-size newOptions to fit the input more optimally*/
	newOptions = realloc(newOptions, strlen(newOptions)*sizeof(newOptions));
	checkAlloc(newOptions);
}

/*
 * Clean argv removes values from argv from argv[arc] to argv[maxArgc]
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
