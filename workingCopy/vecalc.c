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
	  * option holds the current option being processed. Option never contains an
	  * an argument to an option
	  */
	char *option;
	
	while(1) {	
		
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
	argc = refreshArgv(argv);
	} 
return 0;
} 
