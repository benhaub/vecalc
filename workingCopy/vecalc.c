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
#include <unistd.h> /*For returning stdin to the terminal*/

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
	
	/*
	 * In order to manage memory usage properly, we need to know how
	 * much of argv has been dynamically allocated and how much hasn't, so 
	 * this will keep track of the max number of elements that were 
	 * dynamically allocated to our program.
	 *
	 * If argv was given any arguments as it started, those are not
	 * dynamically allocated elements.
	 */

	int *initialArgc = malloc(sizeof(argc));
	*initialArgc = argc;
	
	int *maxArgc = malloc(sizeof(argc));
	*maxArgc = argc;
	
	#ifdef TESTING

	int loopCount = 0;
	
	#endif

	while(1) {

		/*Check vec in case the c option was given*/
		if(vec == NULL) {

			vec = alloc_vec();
		}

		/*Check if this is the most space we've needed so far*/
		if(*maxArgc < argc) {

			*maxArgc = argc;
		}

		/*
		 * This can sometimes happen with redirected input, known for
		 * certain that this happens with here-strings.
		 */
		if(argv[1] == NULL) {

			argc = refreshArgv(argv, maxArgc, initialArgc);
			cleanArgv(argv, argc, *maxArgc);
		}

		/*
		 * If the user forgets to add a q to the end of their
		 * redirected input, then fgets will recieve the empty
		 * string and send it back here infinitely. The program
		 * will close if this happens iff the input has been
		 * redirected
		 */
		if(isatty(STDIN_FILENO) == 0 && strcmp(argv[1], "") == 0) {

			return EXIT_SUCCESS;	
		}
		
		int i;
		for(i = 1; i < argc; i++) {
		
       			option = argv[i];

			/*Any option should only be one character in length*/
			while(strlen(option) > 1) {
			
				fprintf(stderr, "Invalid option. Type 'h' for usage\n");
				argc = refreshArgv(argv, maxArgc, initialArgc);
				cleanArgv(argv, argc, *maxArgc);

				#ifdef TESTING

				loopCount++;

				#endif
			
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
		 *
		 * Make sure it is known what happens for entries that are near
		 * max length
		 */

	#ifdef TESTING
		
		const Elem ERROR = 1E-6;

		/*
		 * The testing file is generated from the makefile. Refer to
		 * it for the sequence of input, and adding more testing
		 */
		
		/*Test extend_vec*/
		if(loopCount == 0) {
		
			if(vec->elements[0] - 5 > ERROR) {
				
				print_vec(vec);
				printf("Element 0 should have value 5, but has value: %f\n", vec->elements[0]);	
				assert(vec->elements[0] - 5 < ERROR);
			}
		}

		else if(loopCount == 1) {
			
			if(vec->elements[1] - 55 > ERROR) {
			
				printf("Element 1 should have value 55, but has value: %f\n", vec->elements[1]);	
				assert(vec->elements[1] - 55 < ERROR);
			}
		}
		
		else if(loopCount == 2) {
			
			if(vec->elements[2] - 4 > ERROR) {
			
				printf("Element 2 should have value 4, but has value: %f\n", vec->elements[2]);	
				assert(vec->elements[2] - 4 < ERROR);
			}
		}

		else if(loopCount == 3) {
			
			if(vec->elements[3] - 7 > ERROR) {
			
				printf("Element 3 should have value 7, but has value: %f\n", vec->elements[3]);	
				assert(vec->elements[3] - 7 < ERROR);
			}
		}

		else if(loopCount == 4 || loopCount == 5 || loopCount == 6 || loopCount == 7 || loopCount == 8) { 

			if(vec->size != 4) {
				
				printf("Size of the vector should be 4, but is: %d\n", vec->size);	
				assert(vec->size == 4);
			}
		}
		/*Test scalar_plus*/
		else if(loopCount == 9) {

			if(vec->elements[0] != 5) {

				printf("Element 0 should have a value of 5, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 5);
			}
			if(vec->elements[1] != 55) {

				printf("Element 1 should have a value of 55, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 55);
			}
			if(vec->elements[2] != 4) {

				printf("Element 2 should have a value of 4, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == 4);
			}
			if(vec->elements[3] != 7) {

				printf("Element 3 should have a value of 7, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 7);
			}
		}
		else if(loopCount == 10) {
			
			if(vec->elements[0] != 6) {

				printf("Element 0 should have a value of 6, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 6);
			}
			if(vec->elements[1] != 56) {

				printf("Element 1 should have a value of 56, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 56);
			}
			if(vec->elements[2] != 5) {

				printf("Element 2 should have a value of 5, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == 5);
			}
			if(vec->elements[3] != 8) {

				printf("Element 3 should have a value of 8, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 8);
			}
		}
		else if(loopCount == 11) {
			
			if(vec->elements[0] != 8) {

				printf("Element 0 should have a value of 8, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 8);
			}
			if(vec->elements[1] != 58) {

				printf("Element 1 should have a value of 58, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 58);
			}
			if(vec->elements[2] != 7) {

				printf("Element 2 should have a value of 7, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == 7);
			}
			if(vec->elements[3] != 10) {

				printf("Element 3 should have a value of 10, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 10);
			}
		}
		else if(loopCount == 12 || loopCount == 13) {
		
			if(vec->elements[0] != 9) {

				printf("Element 0 should have a value of 9, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 9);
			}
			if(vec->elements[1] != 59) {

				printf("Element 1 should have a value of 59, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 59);
			}
			if(vec->elements[2] != 8) {

				printf("Element 2 should have a value of 8, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == 8);
			}
			if(vec->elements[3] != 11) {

				printf("Element 3 should have a value of 11, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 11);
			}
		}
		/*Test scalar_minus*/
		else if(loopCount == 14) {
			
			if(vec->elements[0] != 7) {

				printf("Element 0 should have a value of 7, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 7);
			}
			if(vec->elements[1] != 57) {

				printf("Element 1 should have a value of 57, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 57);
			}
			if(vec->elements[2] != 6) {

				printf("Element 2 should have a value of 6, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == 6);
			}
			if(vec->elements[3] != 9) {

				printf("Element 3 should have a value of 9, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 9);
			}
		}
		else if(loopCount == 15) {
			
			if(vec->elements[0] != 3) {

				printf("Element 0 should have a value of 3, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3);
			}
			if(vec->elements[1] != 53) {

				printf("Element 1 should have a value of 53, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 53);
			}
			if(vec->elements[2] != 2) {

				printf("Element 2 should have a value of 2, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == 2);
			}
			if(vec->elements[3] != 5) {

				printf("Element 3 should have a value of 5, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 5);
			}
		}
		/* - 16 */
		else if(loopCount == 16) {
			
			if(vec->elements[0] != -3) {

				printf("Element 0 should have a value of -3, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == -3);
			}
			if(vec->elements[1] != 47) {

				printf("Element 1 should have a value of 47, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 47);
			}
			if(vec->elements[2] != -4) {

				printf("Element 2 should have a value of -4, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == -4);
			}
			if(vec->elements[3] != -1) {

				printf("Element 3 should have a value of -1, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -1);
			}
		}
		/* -6 */
		else if(loopCount == 17) {

			if(vec->elements[0] != -3) {

				printf("Element 0 should have a value of -3, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == -3);
			}
			if(vec->elements[1] != 47) {

				printf("Element 1 should have a value of 47, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 47);
			}
			if(vec->elements[2] != -4) {

				printf("Element 2 should have a value of -4, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == -4);
			}
			if(vec->elements[3] != -1) {

				printf("Element 3 should have a value of -1, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -1);
			}
		}
		/* 6 - */
		else if(loopCount == 18) {

			if(vec->elements[0] != -3) {

				printf("Element 0 should have a value of -3, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == -3);
			}
			if(vec->elements[1] != 47) {

				printf("Element 1 should have a value of 47, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 47);
			}
			if(vec->elements[2] != -4) {

				printf("Element 2 should have a value of -4, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == -4);
			}
			if(vec->elements[3] != -1) {

				printf("Element 3 should have a value of -1, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -1);
			}
		}
		/* - - */
		else if(loopCount == 19) {

			if(vec->elements[0] != -3) {

				printf("Element 0 should have a value of -3, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == -3);
			}
			if(vec->elements[1] != 47) {

				printf("Element 1 should have a value of 47, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 47);
			}
			if(vec->elements[2] != -4) {

				printf("Element 2 should have a value of -4, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == -4);
			}
			if(vec->elements[3] != -1) {

				printf("Element 3 should have a value of -1, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -1);
			}
		}
		/* - + */
		else if(loopCount == 20) {

			if(vec->elements[0] != -3) {

				printf("Element 0 should have a value of -3, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == -3);
			}
			if(vec->elements[1] != 47) {

				printf("Element 1 should have a value of 47, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 47);
			}
			if(vec->elements[2] != -4) {

				printf("Element 2 should have a value of -4, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == -4);
			}
			if(vec->elements[3] != -1) {

				printf("Element 3 should have a value of -1, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -1);
			}
		}
		/*Test scalar_mult*/
		/* * 1 */
		else if(loopCount == 21) {
			
			if(vec->elements[0] != -3) {

				printf("Element 0 should have a value of -3, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == -3);
			}
			if(vec->elements[1] != 47) {

				printf("Element 1 should have a value of 47, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 47);
			}
			if(vec->elements[2] != -4) {

				printf("Element 2 should have a value of -4, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == -4);
			}
			if(vec->elements[3] != -1) {

				printf("Element 3 should have a value of -1, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -1);
			}
		}
		/* * 2 */
		else if(loopCount == 22) {
			
			if(vec->elements[0] != -6) {

				printf("Element 0 should have a value of -6, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == -6);
			}
			if(vec->elements[1] != 94) {

				printf("Element 1 should have a value of 94, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 94);
			}
			if(vec->elements[2] != -8) {

				printf("Element 2 should have a value of -8, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == -8);
			}
			if(vec->elements[3] != -2) {

				printf("Element 3 should have a value of -2, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -2);
			}
		}
		/* * 0.5 */
		else if(loopCount == 23) {
			
			if(vec->elements[0] != -3) {

				printf("Element 0 should have a value of -3, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == -3);
			}
			if(vec->elements[1] != 47) {

				printf("Element 1 should have a value of 47, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 47);
			}
			if(vec->elements[2] != -4) {

				printf("Element 2 should have a value of -4, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == -4);
			}
			if(vec->elements[3] != -1) {

				printf("Element 3 should have a value of -1, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -1);
			}
		}
		/* ** 6 */
		else if(loopCount == 24) {
		       
			if(vec->elements[0] != -3) {

				printf("Element 0 should have a value of -3, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == -3);
			}
			if(vec->elements[1] != 47) {

				printf("Element 1 should have a value of 47, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 47);
			}
			if(vec->elements[2] != -4) {

				printf("Element 2 should have a value of -4, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == -4);
			}
			if(vec->elements[3] != -1) {

				printf("Element 3 should have a value of -1, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -1);
			}
		}
		/* * * */
		else if(loopCount == 25) {
		       
			if(vec->elements[0] != -3) {

				printf("Element 0 should have a value of -3, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == -3);
			}
			if(vec->elements[1] != 47) {

				printf("Element 1 should have a value of 47, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 47);
			}
			if(vec->elements[2] != -4) {

				printf("Element 2 should have a value of -4, but has value: %f\n", vec->elements[2]);
				assert(vec->elements[2] == -4);
			}
			if(vec->elements[3] != -1) {

				printf("Element 3 should have a value of -1, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -1);
			}
		}
		/* * 1.5 */
		else if(loopCount == 26) {
			
			if(vec->elements[0] + 4.5 > ERROR) {

				printf("Element 0 should have a value of -4.5, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] + 4.5 < ERROR);
			}
			if(vec->elements[1] - 70.5 > ERROR) {

				printf("Element 1 should have a value of 70.5, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[0] - 70.5 < ERROR);
			}
			if((vec->elements[2] + 6) > ERROR) {

				printf("Element 0 should have a value of -6, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 6) < ERROR);
			}
			if(vec->elements[3] + 1.5 > ERROR) {

				printf("Element 3 should have a value of -1.5, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] + 1.5 < ERROR);
			}
		}
		/*Test scalar_div*/
		else if(loopCount == 27) {

			if(vec->elements[0] != -2) {

				printf("Element 0 should have a value of -2, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == -2);
			}
			if(vec->elements[1] != 23) {

				printf("Element 1 should have a value of 23, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] == 23);
			}
			if((vec->elements[2] + 2.5) > ERROR) {

				printf("Element 0 should have a value of -2.5, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 2.5) < ERROR);
			}
			if(vec->elements[3] != -1) {

				printf("Element 3 should have a value of -1, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -1);
			}
		}
		else if(loopCount == 28) {

			if((vec->elements[0] + 0.333333) > ERROR) {

				printf("Element 0 should have a value of -0.333333, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] + 0.333333) < ERROR);
			}
			if((vec->elements[1] - 3.833333) > ERROR) {

				printf("Element 1 should have a value of 3.833333, but has value: %f\n", vec->elements[1]);
				assert((vec->elements[1] - 3.833333) < ERROR);
			}
			if((vec->elements[2] + 0.416666) > ERROR) {

				printf("Element 0 should have a value of -0.416666, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 0.416666) < ERROR);
			}
			if((vec->elements[3] + 0.166666) > ERROR) {

				printf("Element 3 should have a value of -0.166666, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -0.166666);
			}
		}
		else if(loopCount == 29) {

			if((vec->elements[0] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] + 0.666666) < ERROR);
			}
			if((vec->elements[1] - 7.666666) > ERROR) {

				printf("Element 1 should have a value of 7.666666, but has value: %f\n", vec->elements[1]);
				assert((vec->elements[1] - 7.666666) < ERROR);
			}
			if((vec->elements[2] + 0.833332) > ERROR) {

				printf("Element 0 should have a value of -0.833332, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 0.833332) < ERROR);
			}
			if((vec->elements[3] + 0.333332) > ERROR) {

				printf("Element 3 should have a value of -0.333332, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 0.333332);
			}
		}
		else if(loopCount == 30) {

			if((vec->elements[0] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] + 0.666666) < ERROR);
			}
			if((vec->elements[1] - 7.666666) > ERROR) {

				printf("Element 1 should have a value of 7.666666, but has value: %f\n", vec->elements[1]);
				assert((vec->elements[1] - 7.666666) < ERROR);
			}
			if((vec->elements[2] + 0.833332) > ERROR) {

				printf("Element 0 should have a value of -0.833332, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 0.833332) < ERROR);
			}
			if((vec->elements[3] + 0.333332) > ERROR) {

				printf("Element 3 should have a value of -0.333332, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 0.333332);
			}
		}
		else if(loopCount == 31) {

			if((vec->elements[0] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] + 0.666666) < ERROR);
			}
			if((vec->elements[1] - 7.666666) > ERROR) {

				printf("Element 1 should have a value of 7.666666, but has value: %f\n", vec->elements[1]);
				assert((vec->elements[1] - 7.666666) < ERROR);
			}
			if((vec->elements[2] + 0.833332) > ERROR) {

				printf("Element 0 should have a value of -0.833332, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 0.833332) < ERROR);
			}
			if((vec->elements[3] + 0.333332) > ERROR) {

				printf("Element 3 should have a value of -0.333332, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 0.333332);
			}
		}
		else if(loopCount == 32) {
		
			if((vec->elements[0] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] + 0.666666) < ERROR);
			}
			if((vec->elements[1] - 7.666666) > ERROR) {
				
				printf("Element 1 should have a value of 7.666666, but has value: %f\n", vec->elements[1]);
				assert((vec->elements[1] - 7.666666) < ERROR);
			}
			if((vec->elements[2] + 0.833332) > ERROR) {

				printf("Element 0 should have a value of -0.833332, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 0.833332) < ERROR);
			}
			if((vec->elements[3] + 0.333332) > ERROR) {

				printf("Element 3 should have a value of -0.333332, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 0.333332);
			}
		}
		else if(loopCount == 33) {

			if((vec->elements[0] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] + 0.666666) < ERROR);
			}
			if((vec->elements[1] - 7.666666) > ERROR) {
				
				printf("Element 1 should have a value of 7.666666, but has value: %f\n", vec->elements[1]);
				assert((vec->elements[1] - 7.666666) < ERROR);
			}
			if((vec->elements[2] + 0.833332) > ERROR) {

				printf("Element 0 should have a value of -0.833332, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 0.833332) < ERROR);
			}
			if((vec->elements[3] + 0.333332) > ERROR) {

				printf("Element 3 should have a value of -0.333332, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 0.333332);
			}
		}
		else if(loopCount == 34 || loopCount == 35 || loopCount == 36||\
			loopCount == 37 || loopCount == 36 || loopCount == 37||\
			loopCount == 38 || loopCount == 39 || loopCount == 40) {
			
			if(vec->elements[0] != 1) {

				printf("Element 0 should have a value of 1, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}

			if(vec->elements[1] != 2) {

				printf("Element 0 should have a value of 1, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
			if(vec->elements[2] != 3) {
			
				printf("Element 0 should have a value of 1, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
			if(vec->elements[3] != 4) {
			
				printf("Element 0 should have a value of 1, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
			if(vec->elements[4] != 5) {
			
				printf("Element 0 should have a value of 1, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
		}
		else if(loopCount == 41) {

			if(vec->elements[0] != 1) {

				printf("Element 0 should have a value of 1, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}

			if(vec->elements[1] != 2) {

				printf("Element 0 should have a value of 1, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
			if(vec->elements[2] != 3) {
			
				printf("Element 0 should have a value of 1, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
			if(vec->elements[3] != 4) {
			
				printf("Element 0 should have a value of 1, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
			if(vec->elements[4] != 5) {
			
				printf("Element 0 should have a value of 1, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
		}
	loopCount++;
	#endif /*TESTING*/

	argc = refreshArgv(argv, maxArgc, initialArgc);
	cleanArgv(argv, argc, *maxArgc);
	}

return 0;
} 
