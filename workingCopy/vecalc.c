//===============================================================================/
//Author	:	Ben Haubrich						//
//File		:	vecalc.c						//
//Synopsis	:	A vector calculator that can be run from		//
//			the command line with a set of defined arguments	//
//===============================================================================/

//Standard Headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h> //For conditionally compiled testing. Use -DTESTING
#include <string.h> //For refreshArgv()
#include <ctype.h> //For isdigit()

//Local Headers
#include "vecalc.h"

/*
 * Allocate memory for a new vector
 * return: A new vector with 0 size
 */
bool print_vec(struct Vector *vector) {

	if(vector == NULL) {

		fprintf(stderr, "There is no vector to be print");
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
	printf("[a] [value] : append; extend the vector by one element and fill the element with the value \n");
	printf("[+] [value] : scalar plus;  add [value] to each element of the vector\n");
	printf("[-] [value] : scalar minus subtract [value] from each element of the vector\n");
	printf("[*] [value] : scalar multiply multiply [value] to each element of the vector\n");
	printf("[/] [value] : scalar divide divide [value] from each element of the vector\n");
	printf("[e] : end; terminate the vecalc program\n");
}

/* 
 * gets new options from standard in and places them back in argv for
 * processing
 * param argv: The argument vector that contains all the arguments
 * return: The new number of arguments
 * postcond: argv is refreshed with new arguments. The old ones are overwritten.
 * if there place needs to be taken
 */
int refreshArgv(char *argv[]) {

	//get new arguments from stdin (next 9 lines)
	char *newOptions = malloc(sizeof(char));
	
	//Using 20 as the maximum possible for no particular reason. Just
	//need enough to make sure we can handle any combination of args	
	printf("vecalc: ");
        fgets(newOptions, 20, stdin);
	
	//nextArg stores space delimited arguments from newOptions and is then 
	//used to insert into argv. Using 11 since the largest single argument possible
	//is the largest possible number - UINT_MAX. which is 10 characters long, plus
	//a newline.
	char *nextArg = malloc(11*sizeof(char));

	 //The first argument of argv is taken by then name of the program. j fullfills
	 //two purposes:
	 //The first is keeping track of which element of argv we should insert into.
	 //The second is counting how many arguments were added, so that we can use
	 //it to update argc. 
	int j = 1;

		//Pressing the enter key gives fgets a newline as input, so that's
	 	//when we stop. One the terribly unreasonable chance that we have naughty
	 	//users who enter more than 20 characters, fgets will add null to the end
	 	//of the string since there is no room for the newline
		//so we'll check for that too
		if(strcmp(newOptions, "") == 0) {

			return j;
		}
		
		//get all the space delimited arguments and put them in argv
		char *delim = " ";
		nextArg = strtok(newOptions, delim);

		while(nextArg != NULL) {

			//In case vecalc was called with no arguments (not even a space), or gets more
			//arguments than it had last time, we'll have to check for null elements.
			if(argv[j] == NULL) {

				argv[j] = calloc(1, sizeof(char));
			}
			//Clear and allocate anyway for copying
			else {
				
				argv[j] = NULL;
				argv[j] = calloc(1, sizeof(char));
			}

			strncpy(argv[j], nextArg, strlen(nextArg));
			j++;
			//With the null input string, strtok continues on from the old string, and
			//keeps copying more characters until the next delimiter. If there are no
			//more delimiters, strtok returns null
			nextArg = strtok(NULL, delim);
		}
return j;
}

/*
 * Checks to see if the argument is a digit or not
 * param arg: The current argument that needs to be checked
 * return: true if it is a digit, false otherwise
 * precond: input parameter is not null
 */
bool ensureDigit(char *arg) {

	if(arg == NULL) {

		return false;
	}

	if(isdigit(*arg) > 0) {

		return true;
	}
	else {

		return false;
	}
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
 * Extend an existing vecotr by 1 element
 * param vector: The vector to be extened
 * param Elem: The value placed in the new spot
 * return: A vector (separate from the original) that is one element larger
 * precond: input vector is not null.
 * postcond: The input vector still exists, and there is a new larger vector.
 * be sure to free memeory accordingly
 */
struct Vector *extend_vec(struct Vector *inputVector, Elem value) {

	//Initialise the new vector
	struct Vector *biggerVector = calloc(1, sizeof(struct Vector));
	biggerVector->size = inputVector->size + 1;
	biggerVector->elements = calloc(biggerVector->size, sizeof(Elem));

	//Copy the elements from the input vector into the new vector
	int i;
	for(i = 0; i < inputVector->size; i++) {

		biggerVector->elements[i] = inputVector->elements[i];
	}
	//Add in the value for the additional element
	biggerVector->elements[(biggerVector->size) -1] = value;

	return biggerVector;
}
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
	
	int i;
	for(i = 0; i < vector->size; i++) {

		vector->elements[i] += addend;
	}

return vector;
}

struct Vector *scalar_minus(struct Vector *vector, Elem difference) {

	//TODO Function stub	
	return NULL;
}

struct Vector *scalar_mult(struct Vector *vector, Elem factor) {

	//TODO Function stub	
	return NULL;
}

struct Vector *scalar_div(struct Vector *vector, Elem divisor) {

	//TODO Function stub	
	return NULL;
}
/*
 * Program main entry point.
 * Contains "main menu" for options to vecalc
 */
int main(int argc, char *argv[]) {

	//The main vector on which operation are performed
	struct Vector *vec = alloc_vec();
	//Temporary vector when the extend_vec function is called
	struct Vector *tempVec = vec;
	//Holds the current option being processed. Option never contains an
	//an argument to an option
	char *option;
	
	while(1) {	
		
		//Check vec in case the c option was given
		if(vec == NULL) {

			vec = alloc_vec();
		}

		int i;
		for(i = 1; i < argc; i++) {

			if(argv[i] == NULL) {

				printf("Invalid arguments. type 'h' for usage\n");
				argc = refreshArgv(argv);
			}

			option = argv[i];

			switch(*option) {
			
				case 'q':	dealloc_vec(vec);
						return EXIT_SUCCESS;
						break;
				
				case 'e':	dealloc_vec(vec);
						return EXIT_SUCCESS;
						break;
			
				case 'c':	dealloc_vec(vec);
						vec = NULL;
						break;

				case 'p':	print_vec(vec);
						break;

				case 'h':	getHelp();
						break;

				case 'a':	tempVec = vec;
						if(ensureDigit(argv[i + 1])) {

							//Notice that the Elem value increments i, and takes the next value.
							//this skips over the next iteration.
							vec = extend_vec(vec, atof(argv[++i]));
							dealloc_vec(tempVec);
						}
						else {

							fprintf(stderr, "Bad argument - Usage: [a] [value]\n");
							i++;
						}
						break;

				case '+':	if(ensureDigit(argv[i + 1])) {

							scalar_plus(vec, atof(argv[++i]));
						}
						else {

							fprintf(stderr, "Bad argument - Usage [+] [value]\n");
							//The argument is invalid, so skip over it.
							i++;
						}
						break;
				
				case '-':	if(ensureDigit(argv[i + 1])) {

							scalar_plus(vec, atof(argv[++i]));
						}
						else {

							fprintf(stderr, "Bad argument - Usage [-] [value]\n");
							i++;
						}
						break;

				case '*':	if(ensureDigit(argv[i + 1])) {

							scalar_mult(vec, atof(argv[++i]));
						}
						else {

							fprintf(stderr, "Bad argument - Usage [*] [value]\n");
							i++;
						}
						break;

				case '/':	if(ensureDigit(argv[i + 1])) {

							scalar_div(vec, atof(argv[++i]));
						}
						else {

							fprintf(stderr, "Bad argument - Usage [/] [value]\n");
							i++;
						}
						break;

				default:	fprintf(stderr, "Invalid option: %s", argv[i]);
						break;
			}
		}
	argc = refreshArgv(argv);
	} 
return 0;
} 
