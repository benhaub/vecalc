#ifndef _VECTOROUT_H_
#define _VECTOROUT_H_

/*Local Headers*/
#include "vecalc.h" /*For definition of a Vector*/

/*
 * Print the vector to stdout
 * Param vector: pointer to vector to be printed
 * return: True if it can be printed, false otherwise
 * precond: input vector is not null
 */
bool print_vec(struct Vector *);

/*
 * Prints a help page for usage of vecalc to a file in the CWD
 */
void getHelp();

/*
 * Sums up all the values in the vector; Returns the magnitude and prints it to
 * to stdout
 * param struct Vector *: point to vector whose elements are to be summed
 * return: The magnitude of the vector
 * precond: Input vector is not null
 */
float magnitude(struct Vector *);

#endif /*_VECTOROUT_H_*/

