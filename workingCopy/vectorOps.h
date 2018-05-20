/*
 *======================================================================//
 * Author	:	Ben Haubrich					//
 * File		:	vectorOps.h					//
 * Synopsis	:	Defnitions for mathematical operations that can //
 * 			be performed on a vector			//
 *======================================================================//
 */

#ifndef _VECTOROPS_H_
#define _VECTOROPS_H

/*Local Headers*/
#include "vecalc.h"

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
 * precond: divisor is not zero
 */
struct Vector *scalar_div(struct Vector *, Elem);

#endif /*_VECTOROPS_H_*/  

