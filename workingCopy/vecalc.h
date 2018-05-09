//===============================================/
//Author	:	Ben Haubrich		//
//File		:	vecalc.h		//
//Synopsis	:	Vector Calculator	//
//===============================================/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef _VECALC_H_
#define _VECALC_H_

#define ERROR 1E-6 //Error used to test floating point values of vectors

typedef float Elem;

struct Vector {

	int size;
	Elem *elements;
};

/*
 * Function Headers
 */

/*
 * Print the vector to stdout
 * Param vector: pointer to vector to be printed
 * return: True if it can be printed, false otherwise
 * precond: input vector is not null
 */
bool print_vec(struct Vector *);

/*
 * Allocate memory for a new vector
 * return: A new vector with 0 size
 */
struct Vector *alloc_vec();
/*
 * Extend an existing vecotr by 1 element
 * param vector: The vector to be extened
 * param Elem: The value placed in the new spot
 * return: A vector (separate from the original) that is one element larger
 * precond: input vector is not null
 * postcond: The input vector still exists, and there is a new larger vector.
 * be sure to free memeory accordingly
 */
struct Vector *extend_vec(struct Vector *, Elem);
/*
 * Adds a chosen value to each element of the vector
 * param vector: the vector whose elements will be added on to
 * param Elem: The value to be added to each element
 * return: A vector that has had all it's elements incremented by Elem
 * precond: vector is not null
 */
struct Vector *scalar_plus(struct Vector *, Elem);

/*
 * Subtracts a chosen value from each element of the vector
 * param vector: The vector whose elements will have Elem subtracted
 * param Elem: The value that will be subtracted from each element
 * return: A vector that has had all it's elements subtracted by Elem
 * precond: Input vector is not null
 */
struct Vector *scalar_minus(struct Vector *, Elem);

/*
 * multiplies a chosen value to each element of the vector
 * param vector: The vector whose elements will have Elem multiplied to
 * param Elem: The value that will be multiplied to each element
 * return: A vector that has had all it's elements multiplied by Elem
 * precond: Input vector is not null
 */
struct Vector *scalar_mult(struct Vector *, Elem);

/*
 * Divides a chosen value from each element of the vector
 * param vector: The vector whose elements will be divided by Elem
 * param Elem: The value that each element will be divided by
 * return: A vector that has had all it's elements divided by Elem
 * precond: Input vector is not null
 */
struct Vector *scalar_div(struct Vector *, Elem);

/*
 * Prints a help page for usage of vecalc to a file in the CWD
 */
void getHelp();

/*
 * Checks to see if the argument is a digit or not
 * param arg: The current argument that needs to be checked
 * return: true if it is a digit, false otherwise
 * precond: input parameter is not null
 */
bool ensureDigit(char *);

#endif /*_VECALC_H_*/
