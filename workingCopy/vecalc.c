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
	int initialArgc;
	initialArgc = argc;
	
	int maxArgc;
	maxArgc = argc;

	/*
	 * float to hold the value of magnitude. m is only used in this program
	 * for testing, so warnings about it being unused are generated if we
	 * aren't compiling the test code. We can safely ignore it.
	 */
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-parameter"
	#pragma GCC diagnostic ignored "-Wunused-but-set-variable"	
	float m;
	#pragma GCC diagnostic pop
	
	#ifdef TESTING

	int loopCount = 0;
	
	#endif

	while(1) {

		/*Check if this is the most space we've needed so far*/
		if(argc > maxArgc) {

			maxArgc = argc;
		}

		/*
		 * This can sometimes happen with redirected input, known for
		 * certain that this happens with here-strings, or when the
		 * initital arguments aren't given. The while makes sure to
		 * take care of blank input if it's given
		 */
		while(argv[1] == NULL) {

			argc = refreshArgv(argv, maxArgc, initialArgc, argc);

			/*Check if this is the most space we've needed so far*/
			if(argc > maxArgc) {

				maxArgc = argc;
			}
			
			/*
			 * The only problematic values are the values
			 * that follow the current command left over from a 
			 * previously larger command, if it exists. They are
			 * problematic because sometimes strings will only
			 * partially copy over an old string, so they need to
			 * be cleared out fully to ensure safe copying. The
			 * reason we don't clear memory beyond maxArgc is
			 * because that memory may not belong to us.
			 */
			if(argc < maxArgc) {
			
				cleanArgv(argv, argc, argc + 1);
			}
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
		
       			/*Check vec in case the c option was given*/
			if(vec == NULL) {

				vec = alloc_vec();
			}

			option = argv[i];

			/*Any option should only be one character in length*/
			if(strlen(option) > 1) {
			
				fprintf(stderr, "Invalid option: %s Type 'h' for usage\n", argv[i]);
				continue;
				
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

				case 'a':	if(ensureDigit(argv[i + 1])) {

							tempVec = vec;

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

				case 'm':	m = magnitude(vec);

						break;

				case 'r':	if(i != 1) {

							fprintf(stderr, "The r option can not follow any other option.\n");
						}
						break;

				default:	fprintf(stderr, "Invalid option: %s\n", argv[i]);
						
						break;
			}/*delimits case*/
		}/*delimits for*/
		
		/*TODO:
		 * add option to repeat last option given, with or without addition arguments
		 *
		 * Make sure it is known what happens for entries that are near
		 * max length
		 *
		 * Assignment spec says the vector has a max length of 65 thousand something
		 */

	#ifdef TESTING
		
		const Elem ERROR = 1E-5;

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
		/* / 3 */
		else if(loopCount == 27) {

			if(vec->elements[0] + 1.5 > ERROR) {

				printf("Element 0 should have a value of -1.5, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] + 1.5) < ERROR);
			}
			if(vec->elements[1] - 23.5 > ERROR) {

				printf("Element 1 should have a value of 23.5, but has value: %f\n", vec->elements[1]);
				assert(vec->elements[1] - 23.5 < ERROR);
			}
			if((vec->elements[2] + 2) > ERROR) {

				printf("Element 0 should have a value of -2, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 2) < ERROR);
			}
			if(vec->elements[3] + 0.5 > ERROR) {

				printf("Element 3 should have a value of -0.5, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] + 0.5 < ERROR);
			}
		}
		/* / 6 */
		else if(loopCount == 28) {

			if((vec->elements[0] - 0.25) > ERROR) {

				printf("Element 0 should have a value of 0.25, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] -0.25) < ERROR);
			}
			if((vec->elements[1] - 3.916666) > ERROR) {

				printf("Element 1 should have a value of 3.916666, but has value: %f\n", vec->elements[1]);
				assert((vec->elements[1] - 3.916666) < ERROR);
			}
			if((vec->elements[2] + 0.333333) > ERROR) {

				printf("Element 0 should have a value of -0.333333, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 0.333333) < ERROR);
			}
			if((vec->elements[3] + 0.083333) > ERROR) {

				printf("Element 3 should have a value of -0.083333, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == -0.0083333);
			}
		}
		/* / 0.5 */
		else if(loopCount == 29) {

			if((vec->elements[0] - 0.5) > ERROR) {

				printf("Element 0 should have a value of 0.5, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] - 0.5) < ERROR);
			}
			if((vec->elements[1] - 7.833332) > ERROR) {

				printf("Element 1 should have a value of 7.833332, but has value: %f\n", vec->elements[1]);
				assert((vec->elements[1] - 7.833332) < ERROR);
			}
			if((vec->elements[2] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 0.666666) < ERROR);
			}
			if((vec->elements[3] + 0.1666666) > ERROR) {

				printf("Element 3 should have a value of -0.166666, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 0.166666);
			}
		}
		/* // 1 */
		else if(loopCount == 30) {

			if((vec->elements[0] - 0.5) > ERROR) {

				printf("Element 0 should have a value of 0.5, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] - 0.5) < ERROR);
			}
			if((vec->elements[1] - 7.833332) > ERROR) {

				printf("Element 1 should have a value of 7.833332, but has value: %f\n", vec->elements[1]);
				assert((vec->elements[1] - 7.833332) < ERROR);
			}
			if((vec->elements[2] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 0.666666) < ERROR);
			}
			if((vec->elements[3] + 0.1666666) > ERROR) {

				printf("Element 3 should have a value of -0.166666, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 0.166666);
			}
		}
		/* +* 6/ */
		else if(loopCount == 31) {

			if((vec->elements[0] - 0.5) > ERROR) {

				printf("Element 0 should have a value of 0.5, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] - 0.5) < ERROR);
			}
			if((vec->elements[1] - 7.833332) > ERROR) {

				printf("Element 1 should have a value of 7.833332, but has value: %f\n", vec->elements[1]);
				assert((vec->elements[1] - 7.833332) < ERROR);
			}
			if((vec->elements[2] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 0.666666) < ERROR);
			}
			if((vec->elements[3] + 0.1666666) > ERROR) {

				printf("Element 3 should have a value of -0.166666, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 0.166666);
			}
		}
		/* 7 / */
		else if(loopCount == 32) {
		
			if((vec->elements[0] - 0.5) > ERROR) {

				printf("Element 0 should have a value of 0.5, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] - 0.5) < ERROR);
			}
			if((vec->elements[1] - 7.833332) > ERROR) {

				printf("Element 1 should have a value of 7.833332, but has value: %f\n", vec->elements[1]);
				assert((vec->elements[1] - 7.833332) < ERROR);
			}
			if((vec->elements[2] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 0.666666) < ERROR);
			}
			if((vec->elements[3] + 0.1666666) > ERROR) {

				printf("Element 3 should have a value of -0.166666, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 0.166666);
			}
		}
		/* / 0 */
		else if(loopCount == 33) {

			if((vec->elements[0] - 0.5) > ERROR) {

				printf("Element 0 should have a value of 0.5, but has value: %f\n", vec->elements[0]);
				assert((vec->elements[0] - 0.5) < ERROR);
			}
			if((vec->elements[1] - 7.833332) > ERROR) {

				printf("Element 1 should have a value of 7.833332, but has value: %f\n", vec->elements[1]);
				assert((vec->elements[1] - 7.833332) < ERROR);
			}
			if((vec->elements[2] + 0.666666) > ERROR) {

				printf("Element 0 should have a value of -0.666666, but has value: %f\n", vec->elements[2]);
				assert((vec->elements[2] + 0.666666) < ERROR);
			}
			if((vec->elements[3] + 0.1666666) > ERROR) {

				printf("Element 3 should have a value of -0.166666, but has value: %f\n", vec->elements[3]);
				assert(vec->elements[3] == 0.166666);
			}
			if(magnitude(vec) - 7.5 > ERROR) {

				printf("The magnitude of the vector should be 7.5, but it is %f\n", magnitude(vec));
				assert(magnitude(vec) - 7.5 < ERROR);
			}
		}
		else if(loopCount == 35) {
			
			if(vec->elements[0] != 1) {

				printf("Element 0 should have a value of 1, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
			if(vec->size != 1) {

				printf("Size of vector should be 1, but is: %d", vec->size);
				assert(vec->size == 1);
			}
		}
		else if(loopCount == 36) {

			if(vec->elements[1] != 2) {

				printf("Element 1 should have a value of 2, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
			if(vec->size != 2) {

				printf("Size of vector should be 2, but is: %d", vec->size);
				assert(vec->size == 2);	
			}
		}
		else if(loopCount == 37) {

			if(vec->elements[2] != 3) {
			
				printf("Element 2 should have a value of 3, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
			if(vec->size != 3) {

				printf("Size of vector should be 3, but is: %d", vec->size);
				assert(vec->size == 3);
			}
		}
		else if(loopCount == 38) {

			if(vec->elements[3] != 4) {
			
				printf("Element 3 should have a value of 4, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
			if(vec->size != 4) {

				printf("Size of vector should be 4, but is: %d", vec->size);
				assert(vec->size == 4);
			}
		}
		else if(loopCount == 39) {

			if(vec->elements[4] != 5) {
			
				printf("Element 4 should have a value of 5, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
			if(vec->size != 5) {

				printf("Size of vector should be 5, but is: %d", vec->size);
				assert(vec->size == 5);
			}
		}
		else if(loopCount == 40 || loopCount == 41 || loopCount == 42 || loopCount == 43\
					|| loopCount == 44 || loopCount == 45) {

			if(vec->elements[0] != 1) {

				printf("Element 0 should have a value of 1, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 1);
			}
			if(vec->elements[1] != 2) {

				printf("Element 0 should have a value of 2, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 2);
			}
			if(vec->elements[2] != 3) {
			
				printf("Element 0 should have a value of 2, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 2);
			}
			if(vec->elements[3] != 4) {
			
				printf("Element 0 should have a value of 4, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 4);
			}
			if(vec->elements[4] != 5) {
			
				printf("Element 0 should have a value of 5, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 5);
			}
			if(magnitude(vec) != 15) {

				printf("The magnitude of the vector should be 15, but it is %f\n", magnitude(vec));
				assert(magnitude(vec) == 15);
			}
		}
		/* / -0.25 */
		else if(loopCount == 46) {

			if(vec->elements[0] != 3712) {

				printf("Element 0 should have a value of 3712, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3712);
			}
			if(vec->elements[1] != 3676) {

				printf("Element 1 should have a value of 3676, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3676);
			}
			if(vec->elements[2] != 3640) {
			
				printf("Element 2 should have a value of 3640, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3640);
			}
			if(vec->elements[3] != 3604) {
			
				printf("Element 3 should have a value of 3604, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3604);
			}
			if(vec->elements[4] != 3568) {
			
				printf("Element 4 should have a value of 3568, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3568);
			}
		}
		/* - -1 */
		else if(loopCount == 47) {

			if(vec->elements[0] != 3713) {

				printf("Element 0 should have a value of 3713, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3713);
			}
			if(vec->elements[1] != 3677) {

				printf("Element 1 should have a value of 3677, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3677);
			}
			if(vec->elements[2] != 3641) {
			
				printf("Element 2 should have a value of 3641, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3641);
			}
			if(vec->elements[3] != 3605) {
			
				printf("Element 3 should have a value of 3605, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3605);
			}
			if(vec->elements[4] != 3569) {
			
				printf("Element 4 should have a value of 3569, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3569);
			}
		}
		/* + -2 */
		else if(loopCount == 48) {
		
			if(vec->elements[0] != 3711) {

				printf("Element 0 should have a value of 3711, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3711);
			}
			if(vec->elements[1] != 3675) {

				printf("Element 1 should have a value of 3675, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3675);
			}
			if(vec->elements[2] != 3639) {
			
				printf("Element 2 should have a value of 3639, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3639);
			}
			if(vec->elements[3] != 3603) {
			
				printf("Element 3 should have a value of 3603, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3603);
			}
			if(vec->elements[4] != 3567) {
			
				printf("Element 4 should have a value of 3567, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3567);
			}
		}
		else if(loopCount == 49 || loopCount == 50) {

			if(vec->elements[0] != 3711) {

				printf("Element 0 should have a value of 3711, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3711);
			}
			if(vec->elements[1] != 3675) {

				printf("Element 1 should have a value of 3675, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3675);
			}
			if(vec->elements[2] != 3639) {
			
				printf("Element 2 should have a value of 3639, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3639);
			}
			if(vec->elements[3] != 3603) {
			
				printf("Element 3 should have a value of 3603, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3603);
			}
			if(vec->elements[4] != 3567) {
			
				printf("Element 4 should have a value of 3567, but has value: %f\n", vec->elements[0]);
				assert(vec->elements[0] == 3567);
			}
			if(magnitude(vec) != 18195) {

				printf("The magnitude of the vector should be 18195, but it is %f\n", magnitude(vec));
				assert(magnitude(vec) == 18195);
			}
		}
		else if(loopCount == 52) {

			if(m != 5) {

				printf("The magnitude shoud be 5, but it is %f\n", m);
				assert(m == 5);
			}
		}
		else if(loopCount == 53) {

			if(m != 12) {

				printf("The magnitude should be 12, but it is %f\n", m);
				assert(m == 12);
			}
		}
		else if(loopCount == 54) {
		  	
			if(m != 27) {

				printf("The magnitude should be 27, but it is %f\n", m);
				assert(m == 27);
			}
		}
		else if(loopCount == 55 || loopCount == 56 || loopCount == 57 || loopCount == 58\
			       		|| loopCount == 59 || loopCount == 60) {
			
			if(m != 27) {

				printf("The magnitude should be 27, but it is %f\n", m);
				assert(m == 27);
			}
		}
		/*Test repeat*/
		/*r + 3*/
		else if(loopCount == 63) {

			if(vec->elements[0] != 8) {

				printf("Element 0 should have a value of 8, but has value %f\n", vec->elements[0]);
				assert(vec->elements[0] == 8);
			}
			if(vec->elements[1] != 8) {

				printf("Element 1 should have a value of 8, but has value %f\n", vec->elements[1]);
				assert(vec->elements[1] == 8);
			}
			if(vec->size != 2) {

				printf("The size of the vector should be 2, but it is %d\n", vec->size);
				assert(vec->size == 2);
			}
		}
		/* r */
		else if(loopCount == 64) {

			if(vec->elements[0] != 11) {

				printf("Element 0 should have a value of 11, but has value %f\n", vec->elements[0]);
				assert(vec->elements[0] == 11);
			}
			if(vec->elements[1] != 11) {

				printf("Element 1 should have a value of 11, but has value %f\n", vec->elements[1]);
				assert(vec->elements[1] == 11);
			}
			if(vec->elements[2] != 8) {
				
				printf("Element 2 should have a value of 8, but has value %f\n", vec->elements[2]);
				assert(vec->elements[2] == 8);
			}
			if(vec->size != 3) {

				printf("The size of the vector should be 3, but it is %d\n", vec->size);
				assert(vec->size == 3);
			}
		}
		else if(loopCount == 66) {

			if(vec->elements[0] - 445.5 > ERROR) {

				printf("Element 0 should have a value of 445.1, but has value %f\n", vec->elements[0]);
				assert(vec->elements[0] - 445.5 < ERROR);
			}
			if(vec->elements[1] - 445.5 > ERROR) {

				printf("Element 1 should have a value of 445.5, but has value %f\n", vec->elements[1]);
				assert(vec->elements[1] - 445.5 < ERROR);
			}
			if(vec->elements[2] != 324) {
				
				printf("Element 2 should have a value of 324, but has value %f\n", vec->elements[2]);
				assert(vec->elements[2] == 324);
			}
			if(vec->elements[3] - 31.5 > ERROR) {

				printf("Element 3 should have a value of 31.5, but has value %f\n", vec->elements[3]);
				assert(vec->elements[3] - 31.5 < ERROR);
			}
			if(vec->elements[4] - 3.5 > ERROR) {

				printf("Element 4 should have a value of 3.5, but has value %f\n", vec->elements[4]);
				assert(vec->elements[4] - 3.5 < ERROR);
			}
			if(vec->elements[5] != 2) {

				printf("Element 5 should have a value of 2, but has a value %f\n", vec->elements[5]);
				assert(vec->elements[5] == 2);
			}
			if(vec->size != 6) {

				printf("The size of the vector should be 6, but it is %d\n", vec->size);
				assert(vec->size == 6);
			}
		}
		else if(loopCount == 67 || loopCount == 68) {

			if(vec->elements[0] - 896 > ERROR) {

				printf("Element 0 should have a value of 891, but has value %f\n", vec->elements[0]);
				assert(vec->elements[0] - 896 < ERROR);
			}
			if(vec->elements[1] - 896 > ERROR) {

				printf("Element 1 should have a value of 896, but has value %f\n", vec->elements[1]);
				assert(vec->elements[1] - 896 < ERROR);
			}
			if(vec->elements[2] != 653) {
				
				printf("Element 2 should have a value of 653, but has value %f\n", vec->elements[2]);
				assert(vec->elements[2] == 653);
			}
			if(vec->elements[3] - 69 > ERROR) {

				printf("Element 3 should have a value of 69, but has value %f\n", vec->elements[3]);
				assert(vec->elements[3] - 69 < ERROR);
			}
			if(vec->elements[4] - 13 > ERROR) {

				printf("Element 4 should have a value of 13, but has value %f\n", vec->elements[4]);
				assert(vec->elements[4] - 13 < ERROR);
			}
			if(vec->elements[5] != 9) {

				printf("Element 5 should have a value of 9, but has a value %f\n", vec->elements[5]);
				assert(vec->elements[5] == 9);
			}
			if(vec->size != 6) {

				printf("The size of the vector should be 6, but it is %d\n", vec->size);
				assert(vec->size == 6);
			}
		}
		else if(loopCount == 71) {

			if(vec->elements[0] != 5) {

				printf("Element 0 should have a value of 5, but has value %f\n", vec->elements[0]);
				assert(vec->elements[0] == 5);
			}
			if(vec->size != 1) {

				printf("The size of the vector should be 1, but it is %d\n", vec->size);
				assert(vec->size == 1);
			}
		}
		else if(loopCount == 72) {

			if(vec->elements[0] != 5) {

				printf("Element 0 should have a value of 5, but has a value %f\n", vec->elements[0]);
				assert(vec->elements[0] == 5);
			}
			if(vec->elements[1] != 5) {

				printf("Element 0 should have a value of 5, but has a value %f\n", vec->elements[1]);
				assert(vec->elements[1] == 5);
			}	
			if(vec->size != 2) {

				printf("The size of the vector should be 2, but has size %d\n", vec->size);
				assert(vec->size == 2);
			}
		}
	loopCount++;
	#endif /*TESTING*/

		argc = refreshArgv(argv, maxArgc, initialArgc, argc);
	
		if(argc < maxArgc) {

			cleanArgv(argv, argc, argc+1);
		}
	}/*delimits while(1)*/

	dealloc_vec(vec);
	free(option);

return 0;
} 
