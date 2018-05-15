/*
 *===============================================================================/
 * Author	:	Ben Haubrich						//
 * File		:	vecalc.c						//
 * Synopsis	:	A vector calculator that can be run from		//
 * 			the command line with a set of defined arguments	//
 *===============================================================================/
 */

/*Standard Headers*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> /*To check length of option*/
#include <assert.h> /*For conidtionally compiled testing*/

/*Local Headers*/
#include "vecalc.h"
#include "vectorOps.h"
#include "vectorOut.h"
#include "vectorIn.h"
#include "vectorMem.h"

/*
 * Program main entry point.
 * Contains "main menu" for options to vecalc
 */
int main(int argc, char *argv[]) {

	/*The main vector on which operation are performed*/
	  struct Vector *vec = alloc_vec();

	 /*Temporary vector when the extend_vec function is called*/
	  struct Vector *tempVec = vec;

	 /*
	  * option holds the current option being processed. Option never 
	  * contains an argument to an option for all valid input.
	  */
	char *option;
	
	while(1) {

		#ifdef TESTING

		/*
		 * Testing is sort of tricky for this program given the
		 * constant need for input from the user. If we did a regular
		 * defined block of testing at the bottom, we would need to
		 * be able to keep track of the current state of the program,
		 * and what should change in the next state depending on what 
		 * the user entered. This testing is going to rely on a 
		 * pre-defined testing pattern that will come from a file. The 
		 * testing will be broken up into if's that depend on the loop 
		 * count, so we know what should change after each line of input
		 */
			int loopCount = 0;
		#endif	
		
		/*Check vec in case the c option was given*/
		if(vec == NULL) {

			vec = alloc_vec();
		}

		int i;
		for(i = 1; i < argc; i++) {
		
       			option = argv[i];

			/*Any option should only be one character in length*/
			if(strlen(option) > 1) {
			
				fprintf(stderr, "Invalid option or argument. Type 'h' for usage\n");
				argc = refreshArgv(argv);
				option = argv[i];
			}

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

							/*
							 * Notice the pre-inrement. extend_vec takes the next value
							 * of i. This also skips over the next i value iteration in
							 * the for loop.
							 */
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
							/*
							 * The argument is invalid, so skip over it. This same logic
							 * is repeated in other cases
							 */
							i++;
						}
						break;
				
				case '-':	if(ensureDigit(argv[i + 1])) {

							scalar_minus(vec, atof(argv[++i]));
	
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

				default:	fprintf(stderr, "Invalid option: %s\n", argv[i]);
						break;
			}
		}
		/*TODO:
		 * add magnitude option
		 * add option to repeat last option given, with or without addition arguments
		 *
		 * Clean up any memory leaks!
		 */

	#ifdef TESTING
		
		const Elem ERROR = 1E-6;

		/*
		 * The testing file is generated from the makefile. Refer to
		 * it for the sequence of input, and adding more testing
		 */
		
		/*Test extend_vec*/
		if(loopCount == 0) {
		
			if(vec[0] - 5 > ERROR) {
				
				printf("Element 0 should have value 5, but has value: %f\n", vec[0]);	
				assert(vec[0] - 5 < ERROR);
			}
		}

		else if(loopCount == 1) {
			
			if(vec[1] - 55 > ERROR) {
			
				printf("Element 1 should have value 55, but has value: %f\n", vec[1]);	
				assert(vec[1] - 55 < ERROR);
			}
		}
		
		else if(loopCount == 2) {
			
			if(vec[2] - 4 > ERROR) {
			
				printf("Element 2 should have value 4, but has value: %f\n", vec[2]);	
				assert(vec[2] - 4 < ERROR);
			}
		}

		else if(loopCount == 3) {
			
			if(vec[3] - 7 > ERROR) {
			
				printf("Element 3 should have value 7, but has value: %f\n", vec[3]);	
				assert(vec[3] - 7 < ERROR);
			}
		}

		else if(loopCount == 4 || loopCount == 5 || loopCount == 6 || loopCount == 7 || loopCount == 8) { 

			if(vec[4] != NULL) {
				
				printf("Element 4 should not have a value, but has value: %f\n", vec[4]);	
				assert(vec[4] != NULL);
				assert(vec->size == 5);
			}
		}
		/*Test scalar_plus*/
		else if(loopCount == 9) {

			if(vec[0] != 5) {

				printf("Element 0 should have a value of 5, but has value: %f\n", vec[0]);
				assert(vec[0] == 5);
			}
			if(vec[1] != 55) {

				printf("Element 1 should have a value of 55, but has value: %f\n", vec[1]);
				assert(vec[1] == 55);
			}
			if(vec[2] != 4) {

				printf("Element 2 should have a value of 4, but has value: %f\n", vec[2]);
				assert(vec[2] == 4);
			}
			if(vec[3] != 7) {

				printf("Element 3 should have a value of 7, but has value: %f\n", vec[3]);
				assert(vec[3] == 7);
			}
			if(vec[4] != NULL) {

				printf("scalar_plus added onto more elements than the size of the vector\n");
			}
		}
		else if(loopCount == 10) {
			
			if(vec[0] != 6) {

				printf("Element 0 should have a value of 6, but has value: %f\n", vec[0]);
				assert(vec[0] == 6);
			}
			if(vec[1] != 56) {

				printf("Element 1 should have a value of 56, but has value: %f\n", vec[1]);
				assert(vec[1] == 56);
			}
			if(vec[2] != 5) {

				printf("Element 2 should have a value of 5, but has value: %f\n", vec[2]);
				assert(vec[2] == 5);
			}
			if(vec[3] != 8) {

				printf("Element 3 should have a value of 8, but has value: %f\n", vec[3]);
				assert(vec[3] == 8);
			}
			if(vec[4] != NULL) {

				printf("scalar_plus added onto more elements than the size of the vector\n");
			}
		}
		else if(loopCount == 11 || loopCount == 12 || loopCount == 13) {
			
			if(vec[0] != 8) {

				printf("Element 0 should have a value of 8, but has value: %f\n", vec[0]);
				assert(vec[0] == 8);
			}
			if(vec[1] != 58) {

				printf("Element 1 should have a value of 58, but has value: %f\n", vec[1]);
				assert(vec[1] == 58);
			}
			if(vec[2] != 7) {

				printf("Element 2 should have a value of 7, but has value: %f\n", vec[2]);
				assert(vec[2] == 7);
			}
			if(vec[3] != 10) {

				printf("Element 3 should have a value of 10, but has value: %f\n", vec[3]);
				assert(vec[3] == 10);
			}
			if(vec[4] != NULL) {

				printf("scalar_plus added onto more elements than the size of the vector\n");
			}
		}
		/*Test scalar_minus*/
		else if(loopCount == 14) {
			
			if(vec[0] != 6) {

				printf("Element 0 should have a value of 6, but has value: %f\n", vec[0]);
				assert(vec[0] == 6);
			}
			if(vec[1] != 56) {

				printf("Element 1 should have a value of 56, but has value: %f\n", vec[1]);
				assert(vec[1] == 56);
			}
			if(vec[2] != 5) {

				printf("Element 2 should have a value of 5, but has value: %f\n", vec[2]);
				assert(vec[2] == 5);
			}
			if(vec[3] != 8) {

				printf("Element 3 should have a value of 8, but has value: %f\n", vec[3]);
				assert(vec[3] == 8);
			}
			if(vec[4] != NULL) {

				printf("scalar_minus subtracted from more elements than the size of the vector\n");
				assert(vec[4] == NULL);
			}
		}
		else if(loopCount == 15) {
			
			if(vec[0] != 2) {

				printf("Element 0 should have a value of 2, but has value: %f\n", vec[0]);
				assert(vec[0] == 2);
			}
			if(vec[1] != 52) {

				printf("Element 1 should have a value of 52, but has value: %f\n", vec[1]);
				assert(vec[1] == 52);
			}
			if(vec[2] != 1) {

				printf("Element 2 should have a value of 1, but has value: %f\n", vec[2]);
				assert(vec[2] == 1);
			}
			if(vec[3] != 4) {

				printf("Element 3 should have a value of 4, but has value: %f\n", vec[3]);
				assert(vec[3] == 4);
			}
			if(vec[4] != NULL) {

				printf("scalar_minus subtracted from more elements than the size of the vector\n");
			}
		}
		else if(loopCount == 16) {
			
			if(vec[0] != -4) {

				printf("Element 0 should have a value of -4, but has value: %f\n", vec[0]);
				assert(vec[0] == -4);
			}
			if(vec[1] != 46) {

				printf("Element 1 should have a value of 46, but has value: %f\n", vec[1]);
				assert(vec[1] == 46);
			}
			if(vec[2] != -5) {

				printf("Element 2 should have a value of -5, but has value: %f\n", vec[2]);
				assert(vec[2] == -5);
			}
			if(vec[3] != -2) {

				printf("Element 3 should have a value of -2, but has value: %f\n", vec[3]);
				assert(vec[3] == -2);
			}
			if(vec[4] != NULL) {

				printf("scalar_minus subtracted from more elements than the size of the vector\n");
			}
		}
		else if(loopCount == 17) {

			if(vec[0] != -4) {

				printf("Element 0 should have a value of -4, but has value: %f\n", vec[0]);
				assert(vec[0] == -4);
			}
			if(vec[1] != 46) {

				printf("Element 1 should have a value of 46, but has value: %f\n", vec[1]);
				assert(vec[1] == 46);
			}
			if(vec[2] != -5) {

				printf("Element 2 should have a value of -5, but has value: %f\n", vec[2]);
				assert(vec[2] == -5);
			}
			if(vec[3] != -2) {

				printf("Element 3 should have a value of -2, but has value: %f\n", vec[3]);
				assert(vec[3] == -2);
			}
			if(vec[4] != NULL) {

				printf("scalar_minus subtracted from more elements than the size of the vector\n");
			}
		
		}
		else if(loopCount == 18) {

			if(vec[0] != -4) {

				printf("Element 0 should have a value of -4, but has value: %f\n", vec[0]);
				assert(vec[0] == -4);
			}
			if(vec[1] != 46) {

				printf("Element 1 should have a value of 46, but has value: %f\n", vec[1]);
				assert(vec[1] == 46);
			}
			if(vec[2] != -5) {

				printf("Element 2 should have a value of -5, but has value: %f\n", vec[2]);
				assert(vec[2] == -5);
			}
			if(vec[3] != -2) {

				printf("Element 3 should have a value of -2, but has value: %f\n", vec[3]);
				assert(vec[3] == -2);
			}
			if(vec[4] != NULL) {

				printf("scalar_minus subtracted from more elements than the size of the vector\n");
			}
		
		}
		else if(loopCount == 19) {

			if(vec[0] != -4) {

				printf("Element 0 should have a value of -4, but has value: %f\n", vec[0]);
				assert(vec[0] == -4);
			}
			if(vec[1] != 46) {

				printf("Element 1 should have a value of 46, but has value: %f\n", vec[1]);
				assert(vec[1] == 46);
			}
			if(vec[2] != -5) {

				printf("Element 2 should have a value of -5, but has value: %f\n", vec[2]);
				assert(vec[2] == -5);
			}
			if(vec[3] != -2) {

				printf("Element 3 should have a value of -2, but has value: %f\n", vec[3]);
				assert(vec[3] == -2);
			}
			if(vec[4] != NULL) {

				printf("scalar_minus subtracted from more elements than the size of the vector\n");
			}
		
		}
		else if(loopCount == 20) {

			if(vec[0] != -4) {

			printf("Element 0 should have a value of -4, but has value: %f\n", vec[0]);
			assert(vec[0] == -4);
			}
			if(vec[1] != 46) {

				printf("Element 1 should have a value of 46, but has value: %f\n", vec[1]);
				assert(vec[1] == 46);
			}
			if(vec[2] != -5) {

				printf("Element 2 should have a value of -5, but has value: %f\n", vec[2]);
				assert(vec[2] == -5);
			}
			if(vec[3] != -2) {

				printf("Element 3 should have a value of -2, but has value: %f\n", vec[3]);
				assert(vec[3] == -2);
			}
			if(vec[4] != NULL) {

				printf("scalar_minus subtracted from more elements than the size of the vector\n");
			}
		}
		/*Test scalar_mult*/
		else if(loopCount == 21) {
			
			if(vec[0] != -4) {

				printf("Element 0 should have a value of -4, but has value: %f\n", vec[0]);
				assert(vec[0] == -4);
			}
			if(vec[1] != 46) {

				printf("Element 1 should have a value of 46, but has value: %f\n", vec[1]);
				assert(vec[1] == 46);
			}
			if(vec[2] != -5) {

				printf("Element 2 should have a value of -5, but has value: %f\n", vec[2]);
				assert(vec[2] == -5);
			}
			if(vec[3] != -2) {

				printf("Element 3 should have a value of -2, but has value: %f\n", vec[3]);
				assert(vec[3] == -2);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multiplied to more elements than the size of the vector\n");
			}
		}
		else if(loopCount == 22) {
			
			if(vec[0] != -8) {

				printf("Element 0 should have a value of -8, but has value: %f\n", vec[0]);
				assert(vec[0] == -8);
			}
			if(vec[1] != 92) {

				printf("Element 1 should have a value of 92, but has value: %f\n", vec[1]);
				assert(vec[1] == 92);
			}
			if(vec[2] != -10) {

				printf("Element 2 should have a value of -10, but has value: %f\n", vec[2]);
				assert(vec[2] == -10);
			}
			if(vec[3] != -4) {

				printf("Element 3 should have a value of -4, but has value: %f\n", vec[3]);
				assert(vec[3] == -4);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multiplied to more elements than the size of the vector\n");
			}
		}
		else if(loopCount == 23) {
			
			if(vec[0] != -4) {

				printf("Element 0 should have a value of -4, but has value: %f\n", vec[0]);
				assert(vec[0] == -4);
			}
			if(vec[1] != 46) {

				printf("Element 1 should have a value of 46, but has value: %f\n", vec[1]);
				assert(vec[1] == 46);
			}
			if(vec[2] != -5) {

				printf("Element 2 should have a value of -5, but has value: %f\n", vec[2]);
				assert(vec[2] == -5);
			}
			if(vec[3] != -2) {

				printf("Element 3 should have a value of -2, but has value: %f\n", vec[3]);
				assert(vec[3] == -2);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multiplied to more elements than the size of the vector\n");
			}
		}
		else if(loopCount == 24) {
		       
			if(vec[0] != -4) {

				printf("Element 0 should have a value of -4, but has value: %f\n", vec[0]);
				assert(vec[0] == -4);
			}
			if(vec[1] != 46) {

				printf("Element 1 should have a value of 46, but has value: %f\n", vec[1]);
				assert(vec[1] == 46);
			}
			if(vec[2] != -5) {

				printf("Element 2 should have a value of -5, but has value: %f\n", vec[2]);
				assert(vec[2] == -5);
			}
			if(vec[3] != -2) {

				printf("Element 3 should have a value of -2, but has value: %f\n", vec[3]);
				assert(vec[3] == -2);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multiplied to more elements than the size of the vector\n");
			}		
		}	
		else if(loopCount == 25) {
		       
			if(vec[0] != -4) {

				printf("Element 0 should have a value of -4, but has value: %f\n", vec[0]);
				assert(vec[0] == -4);
			}
			if(vec[1] != 46) {

				printf("Element 1 should have a value of 46, but has value: %f\n", vec[1]);
				assert(vec[1] == 46);
			}
			if(vec[2] != -5) {

				printf("Element 2 should have a value of -5, but has value: %f\n", vec[2]);
				assert(vec[2] == -5);
			}
			if(vec[3] != -2) {

				printf("Element 3 should have a value of -2, but has value: %f\n", vec[3]);
				assert(vec[3] == -2);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multiplied to more elements than the size of the vector\n");
			}
		}
		else if(loopCount == 26) {
			
			if(vec[0] != -6) {

				printf("Element 0 should have a value of -6, but has value: %f\n", vec[0]);
				assert(vec[0] == -6);
			}
			if(vec[1] != 69) {

				printf("Element 1 should have a value of 69, but has value: %f\n", vec[1]);
				assert(vec[0] == 69);
			}
			if((vec[2] + 7.5) > ERROR) {

				printf("Element 0 should have a value of -7.5, but has value: %f\n", vec[2]);
				assert((vec[2] + 7.5) < ERROR);
			}
			if(vec[3] != -3) {

				printf("Element 3 should have a value of -3, but has value: %f\n", vec[3]);
				assert(vec[3] == -3);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multplied to more elements than the size of the vector%f\n", vec[4]);
			}		
		}
		printf("%d", loopCount);
		/*Test scalar_div*/
		else if(loopCount == 27) {

			if(vec[0] != -2) {

				printf("Element 0 should have a value of -2, but has value: %f\n", vec[0]);
				assert(vec[0] == -2);
			}
			if(vec[1] != 23) {

				printf("Element 1 should have a value of 23, but has value: %f\n", vec[1]);
				assert(vec[1] == 23);
			}
			if((vec[2] + 2.5) > ERROR) {

				printf("Element 0 should have a value of -2.5, but has value: %f\n", vec[2]);
				assert((vec[2] + 2.5) < ERROR);
			}
			if(vec[3] != -1) {

				printf("Element 3 should have a value of -1, but has value: %f\n", vec[3]);
				assert(vec[3] == -1);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multplied to more elements than the size of the vector%f\n", vec[4]);
			}		
		}
		else if(loopCount == 28) {

			if((vec[0] + 0.333333) > ERROR) {

				printf("Element 0 should have a value of -0.333333, but has value: %f\n", vec[0]);
				assert((vec[0] + 0.333333) < ERROR);
			}
			if((vec[1] - 3.833333) > ERROR) {

				printf("Element 1 should have a value of 3.833333, but has value: %f\n", vec[1]);
				assert((vec[1] - 3.833333) < ERROR);
			}
			if((vec[2] + 0.416666) > ERROR) {

				printf("Element 0 should have a value of -0.416666, but has value: %f\n", vec[2]);
				assert((vec[2] + 0.416666) < ERROR);
			}
			if((vec[3] + 0.166666) > ERROR) {

				printf("Element 3 should have a value of -0.166666, but has value: %f\n", vec[3]);
				assert(vec[3] == -0.166666);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multplied to more elements than the size of the vector%f\n", vec[4]);
			}		
		}
		else if(loopCount == 29) {

			if((vec[0] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec[0]);
				assert((vec[0] + 0.666666) < ERROR);
			}
			if((vec[1] - 7.666666) > ERROR) {

				printf("Element 1 should have a value of 7.666666, but has value: %f\n", vec[1]);
				assert((vec[1] - 7.666666) < ERROR);
			}
			if((vec[2] + 0.833332) > ERROR) {

				printf("Element 0 should have a value of -0.833332, but has value: %f\n", vec[2]);
				assert((vec[2] + 0.833332) < ERROR);
			}
			if((vec[3] + 0.333332) > ERROR) {

				printf("Element 3 should have a value of -0.333332, but has value: %f\n", vec[3]);
				assert(vec[3] == 0.333332);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multplied to more elements than the size of the vector%f\n", vec[4]);
			}		
		}
		else if(loopCount == 30) {

			if((vec[0] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec[0]);
				assert((vec[0] + 0.666666) < ERROR);
			}
			if((vec[1] - 7.666666) > ERROR) {

				printf("Element 1 should have a value of 7.666666, but has value: %f\n", vec[1]);
				assert((vec[1] - 7.666666) < ERROR);
			}
			if((vec[2] + 0.833332) > ERROR) {

				printf("Element 0 should have a value of -0.833332, but has value: %f\n", vec[2]);
				assert((vec[2] + 0.833332) < ERROR);
			}
			if((vec[3] + 0.333332) > ERROR) {

				printf("Element 3 should have a value of -0.333332, but has value: %f\n", vec[3]);
				assert(vec[3] == 0.333332);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multplied to more elements than the size of the vector%f\n", vec[4]);
			}	

		}
		else if(loopCount == 31) {

			if((vec[0] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec[0]);
				assert((vec[0] + 0.666666) < ERROR);
			}
			if((vec[1] - 7.666666) > ERROR) {

				printf("Element 1 should have a value of 7.666666, but has value: %f\n", vec[1]);
				assert((vec[1] - 7.666666) < ERROR);
			}
			if((vec[2] + 0.833332) > ERROR) {

				printf("Element 0 should have a value of -0.833332, but has value: %f\n", vec[2]);
				assert((vec[2] + 0.833332) < ERROR);
			}
			if((vec[3] + 0.333332) > ERROR) {

				printf("Element 3 should have a value of -0.333332, but has value: %f\n", vec[3]);
				assert(vec[3] == 0.333332);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multplied to more elements than the size of the vector%f\n", vec[4]);
			}	

		}
		else if(loopCount == 32) {
		
			if((vec[0] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec[0]);
				assert((vec[0] + 0.666666) < ERROR);
			}
			if((vec[1] - 7.666666) > ERROR) {
				
				print_vec(vec);

				printf("Element 1 should have a value of 7.666666, but has value: %f\n", vec[1]);
				assert((vec[1] - 7.666666) < ERROR);
			}
			if((vec[2] + 0.833332) > ERROR) {

				printf("Element 0 should have a value of -0.833332, but has value: %f\n", vec[2]);
				assert((vec[2] + 0.833332) < ERROR);
			}
			if((vec[3] + 0.333332) > ERROR) {

				printf("Element 3 should have a value of -0.333332, but has value: %f\n", vec[3]);
				assert(vec[3] == 0.333332);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multplied to more elements than the size of the vector%f\n", vec[4]);
			}
		}
		else if(loopCount == 33) {

			if((vec[0] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec[0]);
				assert((vec[0] + 0.666666) < ERROR);
			}
			if((vec[1] - 7.666666) > ERROR) {
				
				print_vec(vec);

				printf("Element 1 should have a value of 7.666666, but has value: %f\n", vec[1]);
				assert((vec[1] - 7.666666) < ERROR);
			}
			if((vec[2] + 0.833332) > ERROR) {

				printf("Element 0 should have a value of -0.833332, but has value: %f\n", vec[2]);
				assert((vec[2] + 0.833332) < ERROR);
			}
			if((vec[3] + 0.333332) > ERROR) {

				printf("Element 3 should have a value of -0.333332, but has value: %f\n", vec[3]);
				assert(vec[3] == 0.333332);
			}
			if(vec[4] != NULL) {

				printf("scalar_mult multplied to more elements than the size of the vector%f\n", vec[4]);
			}
		}
		else if(loopCount == 34 || loopCount == 35 || loopCount == 36||\
			loopCount == 37 || loopCount == 36 || loopCount == 37||\
			loopCount == 38 || loopCount == 39 || loopCount == 40) {
			
			if(vec[0] != 1) {

				printf("Element 0 should have a value of 1, but has value: %f\n", vec[0]);
				assert(vec[0] == 1);
			}

			if(vec[1] != 2) {

				printf("Element 0 should have a value of 1, but has value: %f\n", vec[0]);
				assert(vec[0] == 1);
			}
			if(vec[2] != 3) {
			
				printf("Element 0 should have a value of 1, but has value: %f\n", vec[0]);
				assert(vec[0] == 1);
			}
			if(vec[3] != 4) {
			
				printf("Element 0 should have a value of 1, but has value: %f\n", vec[0]);
				assert(vec[0] == 1);
			}
			if(vec[4] != 5) {
			
				printf("Element 0 should have a value of 1, but has value: %f\n", vec[0]);
				assert(vec[0] == 1);
			}
			if(vec[5] != NULL) {

				printf("Element 5 should not have a value, but has value %f\n", vec[5]);
			}
		}

	#endif /*TESTING*/

	argc = refreshArgv(argv);
	}

return 0;
} 
