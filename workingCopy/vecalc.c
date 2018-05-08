//===============================================================================/
//Author	:	Ben Haubrich						//
//File		:	vecalc.c						//
//Synopsis	:	A vector calculator that can be run from		//
//			the command line with a set of defined arguments	//
//===============================================================================/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h> //For conditionally compiled testing. Use -DTESTING
#include <string.h> //For refreshArgv()

//Local Headers
#include "vecalc.h"

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

		int i = 0;
		for(i = 0; i < vector->size; i++) {

			printf("%f\n", vector->elements[i]);
		}
		return EXIT_SUCCESS;
	}
}	

void getHelp() {

	//FILE *helpDoc
	//helpDoc = fopen("vecalcHelp", "w");
}

 /* 
  * gets new options from standard in and places them back in argv for
  * processing
  * param argv: The argument vector that contains all the arguments
  * return: The new number of arguments
  * postcond: argv is refreshed with new arguments. The old ones are overwritten.
  *
  * Note that there is some change in behaviour with the way that the input args
  * work and subsequent refreshed args. In the input, kkkkkk is all one argument,
  * but for a refreshed arg that is 6 arguments.
  */
int refreshArgv(char *argv[]) {

	//get new arguments from stdin (next 9 lines)
	char *newOptions = malloc(sizeof(char));
	
	//Using 8 as the maximum possible for no particular reason. Just
	//need enough to make sure we can handle any combination of args	
	printf("vecalc: ");
        fgets(newOptions, 8, stdin);
	
	//newOptionByte stores one character at a time from newOptions and is used
	//to place into argv one iteration at a time.	
	char *newOptionByte = malloc(sizeof(char));

	 //Insert the new arguments into argv (while loop)
	 //The first argument of argv is taken by then name of the program. j fullfills
	 //two purposes:
	 //The first is keeping track of which element of argv we should insert into.
	 //The second is counting how many arguments were added, so that we can use
	 //it to update argc. 
	int j = 1;

	while(newOptions[0] != '\n') {

		//TODO: Entire structure of vecalc needs to be changed. Needs to be able
		//to split single arguments not by character, but by whitespace.

		//Pressing the enter key gives fgets a newline as input, so that's
	 	//when we stop. One the terribly unreasonable chance that we have naughty
	 	//users who enter more than 8 characters, fgets will add null to the end
	 	//of the string since there is no room for the newline
		//so we'll check for that too
		if(strcmp(newOptions, "") == 0) {

			return j;
		}
		//Eliminate the spaces from the argument list.
		if(newOptions[0] == ' ') {

			newOptions += 1;
			//Might not even need the continue statement, but it also might help
			//make this statements purpose more clear.
			continue;
		}
		else {

			//using strncpy to only copy one byte at a time makes sure we don't get the
			//new line character
			strncpy(newOptionByte, newOptions, 1);
			//It's really important not to just use equals here to put newOptionByte into
			//the array. If we say argv[j] = newOptionByte, we give it newOptionByte's 
			//address, then any subsequent changes to newOptionByte also change argv[j-1];
			//strncpy give unique adresses.

			//In case vecalc was called with no arguments (not even a space), or gets more
			//arguments than it had last time
			
			if(argv[j] == NULL) {

				argv[j] = calloc(1, sizeof(char));
			}
			//Clear and allocate anyway for copying
			else {
				
				argv[j] = NULL;
				argv[j] = calloc(1, sizeof(char));
			}

			strncpy(argv[j], newOptionByte, 1);
			newOptions += 1;
			j++;
		}
	}
	return j;
}

struct Vector *alloc_vec() {
	
	struct Vector *vector = calloc(1, sizeof(struct Vector));
	vector->size = 0;

	return vector;
}

void dealloc_vec(struct Vector *vector) {

	free(vector);
}

struct Vector *extend_vec(struct Vector *inputVector, Elem value) {

	//Initialise the new vector
	struct Vector *biggerVector = calloc(1, sizeof(struct Vector));
	biggerVector->size = inputVector->size + 1;
	biggerVector->elements = calloc(biggerVector->size, sizeof(Elem));

	//Copy the elements from the input vector into the new vector
	int i = 0;
	for(i = 0; i < inputVector->size; i++) {

		biggerVector->elements[i] = inputVector->elements[i];
	}
	//Add in the value for the additional element
	biggerVector->elements[biggerVector->size] = value;

	return biggerVector;
}

struct Vector *scalar_plus(struct Vector *vector, Elem addend) {

	//TODO Function stub	
	return NULL;
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
	//temporary vector when the extend_vec function is called
	struct Vector *tempVec = vec;
	//Holds the current option being processed
	char *option;
	
	while(1) {	
		
		int i = 0;
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
						break;

				case 'p':	print_vec(vec);
						break;

				case 'h':	getHelp();
						break;

				case 'a':	tempVec = vec;
						vec = extend_vec(vec, atof(argv[++i]));
						dealloc_vec(tempVec);
						//Notice that this skips over the next
						//iteration, because it will just be 
						//the value argument.
						break;

				case '+':	scalar_plus(vec, atof(argv[++i]));
						break;
				
				case '-':	scalar_plus(vec, atof(argv[++i]));
						break;

				case '*':	scalar_mult(vec, atof(argv[++i]));
						break;

				case '/':	scalar_div(vec, atof(argv[++i]));
						break;

				default:	printf("Not a valid option\n");
						break;
						//TODO: add if's for common misuse, and suggest input	

			}
		}
		argc = refreshArgv(argv);
	}

#ifdef TESTING

	//Test print_vec
	struct Vector *vector = calloc(1, sizeof(struct Vector));
	vector->size = 5;
	vector->elements = calloc(vector->size, sizeof(Elem));
	vector->elements[0] = 1;
	vector->elements[1] = 2;
	vector->elements[2] = 3;
	vector->elements[3] = 4;
	vector->elements[4] = 5;

	print_vec(vector);

	//Test extend_vec
	//Extend vector by 5
	for(int i = 0; i < 5; i++) {
	
		struct Vector *savedAddress = vector;

		//The vectors are exteneded in this fashion to stop memory leaks.
		vector = extend_vec(vector, 0);
		dealloc_vec(savedAddress);
	}
	//Make sure the size increased
	if(vector->size != 10) {
		
		fprintf(stderr, "The size of the vector is :%d. It should be: 10\n", vector->size); 
		assert(vector->size == 10);
	}
	//Make sure that all the values from the previous vetctor carried over
	for(int i = 0; i < 5; i++) {
		
		if( ( (vector->elements[i]) - (i+1) ) > ERROR) {
					
			fprintf(stderr, "The value of element %d is: %f, but should be %d\n", i, vector->elements[i], i+1);
			assert( ( (vector->elements[i]) - (i+1) ) < ERROR);
		}
	}
	//Test scalar_plus
	//add 1 to all elements
	vector = scalar_plus(vector, 1);
	
	for(int i = 0; i < vector->size; i++) {
	
		if( ( (vector->elements[i]) -  (i+2) )  > ERROR){

			fprintf(stderr, "The value of element %d should be %d, but it is %f",i, i+2, vector->elements[i]);
			assert( ( (vector->elements[i]) -  (i+2) )  < ERROR);

		}	
	}

#endif //TESTING

	return 0;

}
