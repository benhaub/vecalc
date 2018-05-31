/*
 *===============================================================================/
 * Author	:	Ben Haubrich						//
 * File		:	vectorOps.c						//
 * Synopsis	:	All the mathematical operations that can		//
 * 			be performed on a vector.				//
 *===============================================================================/
 */

/*Standard Headers*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*Local Headers*/
#include "vectorOps.h"

/*
 * Adds a chosen value to each element of the vector
 * param vector: the vector whose elements will be added on to
 * param Elem: The value to be added to each element
 * return: A vector that has all the input elements incremented by Elem
 * precond: vector is not null
 */
struct Vector *scalar_plus(struct Vector *vector, Elem addend) {

	if(vector == NULL) {

		fprintf(stderr, "Input vector is null!\n");
		return vector;
	}
	
	if(vector->size == 0) {
		
		printf("Using scalar plus on a zero size vector has no effect\n");
	}
	else {
		
		int i;
		for(i = 0; i < vector->size; i++) {

			vector->elements[i] += addend;
		}
	}

return vector;
}

/*
 * Subtracts a chosen value from each element of the vector
 * param vector: The vector whose elements will be subtracted by Elem
 * param Elem: The value that each element will be subtracted by
 * return: A vector that has had all it's elements subtracted Elem
 * precond: Input vector is not null
 */
struct Vector *scalar_minus(struct Vector *vector, Elem difference) {

	if(vector == NULL) {

		fprintf(stderr, "Input vector is null!\n");
		return vector;
	}
	
	if(vector->size == 0) {
								
		printf("Using scalar minus on a zero size vector has no effect\n");
	}
	else {

		int i;
		for(i = 0; i < vector->size; i++) {

			vector->elements[i] -= difference;
		}
	}	

return vector;
}

/*
 * multiplies a chosen value to each element of the vector
 * param vector: The vector whose elements will have Elem multiplied to
 * param Elem: The value that will be multiplied to each element
 * return: A vector that has had all it's elements multiplied by Elem
 * precond: Input vector is not null
 */
struct Vector *scalar_mult(struct Vector *vector, Elem factor) {

	if(vector == NULL) {

		fprintf(stderr, "Input vector is null!\n");
		return vector;
	}
	if(vector->size == 0) {
								
		printf("Using scalar multiply on a zero size vector has no effect\n");
	}
	else {
		
		int i;
		for(i = 0; i < vector->size; i++) {

			vector->elements[i] *= factor;
		}
	}

return vector;
}

/*
 * Divides a chosen value from each element of the vector
 * param vector: The vector whose elements will be divided by Elem
 * param Elem: The value that each element will be divided by
 * return: A vector that has had all it's elements divided by Elem
 * precond: Input vector is not null
 * precond: divisor is not zero
 */
struct Vector *scalar_div(struct Vector *vector, Elem divisor) {

	if(divisor == 0) {

		fprintf(stderr, "Bad argument to divide - Divide by zero error\n");
		return vector;
	}
	else if(vector->size == 0) {
								
		printf("Using scalar divide on a zero size vector has no effect\n");
	}
	else {

		int i;
		for(i = 0; i < vector->size; i++) {

			vector->elements[i] = (vector->elements[i]) / (divisor);
		}
	}

return vector;
}
/*
 * Sums up all the values in the vector and returns the magnitude
 * param struct Vector *: point to vector whose elements are to be summed
 * return: The magnitude of the vector
 * precond: Input vector is not null
 */
float magnitude(struct Vector *vector) {

	float magnitude = 0;

	int i;
	for(i = 0; i < vector->size; i++) {

		magnitude += vector->elements[i];
	}
	printf("Magnitude: %f\n", magnitude);

return magnitude;
}
