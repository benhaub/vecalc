/*
 *===============================================================================/
 * Author	:	Ben Haubrich						//
 * File		:	vectorOut.c						//
 * Synopsis	:	All the functions whose primary purpose is to provide	//
 * 			output to the user					//
 *===============================================================================/
 */

/*Standard Headers*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/*Local Headers*/
#include "vectorOut.h"

/*
 * Allocate memory for a new vector
 * return: A new vector with 0 size
 */
bool print_vec(struct Vector *vector) {

	if(vector == NULL) {

		fprintf(stderr, "There is no vector to print\n");
		return EXIT_FAILURE;
	}
	else if(vector->size == 0) {

		printf("Nothing to print. Vector has zero size\n");
		return EXIT_SUCCESS;
	}
	else {

		int i;
		for(i = 0; i < vector->size; i++) {

			printf("%f\n", vector->elements[i]);
		}
		return EXIT_SUCCESS;
	}
}	

/*
 * Prints a help page for usage of vecalc to a file in the CWD
 */
void getHelp() {

	printf("Vecalc is a program that can perform operations on a vector (1 dimensional array)\n");
	printf("Usage: [Option] [Value]\n");
	printf("Options:\n");
	printf("[q] : quit; Same functionality as [e] (end)\n");
	printf("[c] : clear; Delete the current vector and start a new one\n");
	printf("[p] : print; Output the contents of the vector to the console \n");
	printf("[h] : help; Output the list of commands and the usage\n");
	printf("[m] : magnitude; Output the magnitude to the console\n");
	printf("[r] [option] [value] : repeat the last command given with a new set of commands.\n");
	printf("[a] [value] : append; extend the vector by one element and fill the element with the value \n");
	printf("[+] [value] : scalar plus;  add [value] to each element of the vector\n");
	printf("[-] [value] : scalar minus subtract [value] from each element of the vector\n");
	printf("[*] [value] : scalar multiply multiply [value] to each element of the vector\n");
	printf("[/] [value] : scalar divide divide [value] from each element of the vector\n");
	printf("[e] : end; terminate the vecalc program\n");
}
