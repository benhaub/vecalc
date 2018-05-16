/*
 *==============================================//
 * Author	:	Ben Haubrich		//
 * File		:	vecalc.h		//
 * Synopsis	:	Vector Calculator	//
 *==============================================//
 */

/*Standard Headers*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*Local Headers*/
#include "vecalc.h" /*For definition of Vector*/

/*
 * Extend an existing vecotr by 1 element
 * param vector: The vector to be extened
 * param Elem: The value placed in the new spot
 * return: A vector (separate from the original) that is one element larger
 * precond: input vector is not null.
 * postcond: The input vector still exists, and there is a new larger vector.
 * be sure to free memeory accordingly
 */
struct Vector *extend_vec(struct Vector *inputVector, Elem value) {

	/*Initialise the new vector*/
	struct Vector *biggerVector = calloc(1, sizeof(struct Vector));
	biggerVector->size = inputVector->size + 1;
	biggerVector->elements = calloc(biggerVector->size, sizeof(Elem));

	/*Copy the elements from the input vector into the new vector*/
	int i;
	for(i = 0; i < inputVector->size; i++) {

		biggerVector->elements[i] = inputVector->elements[i];
	}
	/*Add in the value for the additional element*/
	biggerVector->elements[(biggerVector->size) -1] = value;

	return biggerVector;
}
/*
 * Allocate memory for a new vector
 * return: A new vector with 0 size
 */
struct Vector *alloc_vec() {
	
	struct Vector *vector = calloc(1, sizeof(struct Vector));

	if(vector == NULL) {

		fprintf(stderr, "Error allocating memory");
	}

	vector->size = 0;

return vector;
}

void dealloc_vec(struct Vector *vector) {

	free(vector);
}

/*
 * Checks malloc calls to make sure the succeeded
 * param void *: The newly allocated pointer
 * return: true if void * is not null, false otherwise
 */
bool checkMalloc(void * ptr) {

	if(ptr == NULL) {

		fprintf(stderr, "Call to malloc failed\n");
		return false;
	}
	else {
		
		return true;
	}
}
