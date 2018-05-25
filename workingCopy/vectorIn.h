#ifndef _VECTORIN_H_
#define _VECTORIN_H

#define MAX_INPUT_LENGTH 185 /*Arbritray choice based on how many characters
			       fit the width of my screen*/
/*Standard Headers*/
#include <stdbool.h>

/*
 * Checks to see if the argument is a digit or not
 * param arg: The current argument that needs to be checked
 * return: true if it is a digit, false otherwise
 * precond: input parameter is not null
 */
bool ensureDigit(char *);

/* 
 * gets new options from standard in and places them back in argv for
 * processing.
 * param char*[]: The argument vector that contains all the arguments
 * param int: The maximum number of arguments that has been used
 * param int: The initial amount of arguments the program was ran with
 * param int: The number of arguments currently in the vector
 * return: The new number of arguments
 * postcond: argv is refreshed with new arguments. The old ones are overwritten.
 * if their place needs to be taken
 */
int refreshArgv(char *[], int, int, int);

/*
 * Accepts input from the user.
 * return: A dynamically allocated string that holds the users ne input
 * precond: char * is not null and is dynamically allocated
 * postcond: The returned string should be freed when it is no longer needed.
 * Returned strings will have newlines on the end of them, be sure to trim it
 * if necessary
 */
char * userIn();
/*
 * Clean argv removes values from argv greater than the current argc
 * param char[] *: the argument vector to be cleaned of unwanted values
 * param int: The current amount of arguments in argv
 * param int: The maximum amount of arguments in argv
 * precond: argv is not null
 * postcond: elements in argv greater that argc and less than the maximum
 * arguments used are cleared of existing values
 */
void cleanArgv(char *[], int, int);

#endif /*_VECTORIN_H_*/

