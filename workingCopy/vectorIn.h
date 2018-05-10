#ifndef _VECTORIN_H_
#define _VECTORIN_H

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
 * return: The new number of arguments
 * postcond: argv is refreshed with new arguments. The old ones are overwritten.
 * if there place needs to be taken
 */
int refreshArgv(char *[]);

#endif /*_VECTORIN_H_*/
