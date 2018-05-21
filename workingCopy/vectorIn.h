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
 * processing
 * param char*[]: The argument vector that contains all the arguments
 * param int *: The maximum number of arguments that has been used
 * param int *: The initial amount of arguments the program was ran with
 * return: The new number of arguments
 * postcond: argv is refreshed with new arguments. The old ones are overwritten.
 * if their place needs to be taken
 */
int refreshArgv(char *[], int, int);

/*
 * Handles input from the user to make sure that it is safe to be further
 * processed by vecalc and that it's take up as little memory as possible.
 * param char *: string to hold the users new input
 * precond: char * is not null
 */
void userIn(char *);
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
/*
 * add more arguments onto an argument vector
 * param char *[]: The argument vector currently in use
 * param char *: string that hold the previous set of commands that is being
 * re-used
 * precond: char *[] is not null
 * char * is not null
 * postcond: The old arguments from char * will be placed at the beginning of
 * the argument vector in use, and the new ones will follow it
 */
void appendArgv(char *[], int, int); 

#endif /*_VECTORIN_H_*/
