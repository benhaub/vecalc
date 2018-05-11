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
	  * contains an argument to an option
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

							/*
							 * Notice that the Elem value increments i, and takes the next value.
							 * this skips over the next iteration.
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
							/*The argument is invalid, so skip over it. This same logic
							 * is repeated in other cases*/
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

	#ifdef TESTING
		
		const Elem ERROR = 1E-6;

		/*
		 * The testing file is generated from the makefile. Refer to
		 * is for the sequence of input
		 */
		
		/*Test extend_vec*/
		if(loopCount == 0) {
		
			if(vec[0] - 5 > ERROR) {
				
				printf("Element 0 should have value 5, but has value: %f\n", vec[0]);	
				assert(vec[0] - 5 < ERROR);
			}
		}

		if(loopCount == 1) {
			
			if(vec[1] - 55 > ERROR) {
			
				printf("Element 1 should have value 55, but has value: %f\n", vec[1]);	
				assert(vec[1] - 55 < ERROR);
			}
		}
		
		if(loopCount == 2) {
			
			if(vec[2] - 4 > ERROR) {
			
				printf("Element 2 should have value 4, but has value: %f\n", vec[2]);	
				assert(vec[2] - 4 < ERROR);
			}
		}

		if(loopCount == 3) {
			
			if(vec[3] - 7 > ERROR) {
			
				printf("Element 1 should have value 7, but has value: %f\n", vec[3]);	
				assert(vec[1] - 7 < ERROR);
			}
		}

		if(loopCount == 4) { 

			if(vec[2] != NULL) {
				
				printf("Element 2 should not have a value, but has value: %f\n", vec[2]);	
				assert(vec[2] != NULL);
			}
		}

		if(loopCount == 5) { 

			if(vec[2] != NULL) {
				
				printf("Element 2 should not have a value, but has value: %f\n", vec[2]);	
				assert(vec[2] != NULL);
			}
		}

		if(loopCount == 6) { 

			if(vec[2] != NULL) {
				
				printf("Element 2 should not have a value, but has value: %f\n", vec[2]);	
				assert(vec[2] != NULL);
			}
		}

		if(loopCount == 7) { 

			if(vec[2] != NULL) {
				
				printf("Element 2 should not have a value, but has value: %f\n", vec[2]);	
				assert(vec[2] != NULL);
			}
		}

		if(loopCount == 8) { 

			if(vec[2] != NULL) {
				
				printf("Element 2 should not have a value, but has value: %f\n", vec[2]);	
				assert(vec[2] != NULL);
			}
		}
	#endif /*TESTING*/

	argc = refreshArgv(argv);
	}

return 0;
} 
