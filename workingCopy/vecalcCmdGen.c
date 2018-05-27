#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
 * This function prints out commands fro the vecalc execuatable according to
 * the format specified in the fprintf statement
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

return 0;
}
