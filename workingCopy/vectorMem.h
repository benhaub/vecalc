/*
 *==============================================================================//
 * Author	:	Ben Haubrich						//
 * File		:	vectorMem.h						//
 * Synopsis	:	Contains all functions that manage and modify memory	//
 *==============================================================================//
 */

#ifndef _VECTORMEM_H_
#define _VECTORMEM_H_

/*Local Headers*/
#include "vecalc.h" /*For definition of Vector*/

/*
 * Allocate memory for a new vector
 * return: A new vector with 0 size
 */
struct Vector *alloc_vec();

/*
 * De-allocate the vector
 */
void dealloc_vec(struct Vector *);

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
 * Checks malloc calls to make sure the succeeded
 * param void *: The newly allocated pointer
 * return: true if void * is not null, false otherwise
 */
bool checkAlloc(void * ptr);

#endif /*_VECTORMEM_H_*/
