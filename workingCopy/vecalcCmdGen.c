#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
 * This program creates a text document that contains commands for the vecalc
 * execuatable according to the format specified in the fprintf statement. The
 * text document can then be redirected into vecalc to run vecalc.
 */
int main() {

	/*List of all the available commands in vecalc*/
	char cmdsList[8] = {'a', '+', 'r', '*', 'm', '/', '-', 'c'};

	/*Number of commands to print*/
	int c = 200;

	time_t t;

	srand(time(&t));

	/*File for commands to be stored in*/
	FILE *vecalcCmds;
	vecalcCmds = fopen("vecalcCmds.txt", "w+");

	/*
	 * rand() % x means generate random numbers between 1 and x
	 */
	int i;
	for(i = 0; i < c; i++) {

		fprintf(vecalcCmds, "%c %d\n", cmdsList[rand() % 7], rand() % 100);
	}
	for(i = 0; i < c; i++) {

		fprintf(vecalcCmds, "%c %d %c %d\n", cmdsList[rand() % 7], rand() % 100, cmdsList[rand() % 7], rand() % 100);
	}
	for(i = 0; i < c; i++) {

		fprintf(vecalcCmds, "%c %d %c %d %c %d\n", cmdsList[rand() % 7], rand() % 100, cmdsList[rand() % 7], rand() % 100, cmdsList[rand() % 7], rand() % 100);
	}
	for(i = 0; i < c; i++) {

		fprintf(vecalcCmds, "%c %d %c %d %c %d %c %d\n", cmdsList[rand() % 7], rand() % 100, cmdsList[rand() % 7], rand() % 100, cmdsList[rand() % 7], rand() % 100, cmdsList[rand() % 7], rand() % 100);
	}
	fprintf(vecalcCmds, "q");

return 0;
}
