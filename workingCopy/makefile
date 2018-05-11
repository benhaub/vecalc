#################################################
#Author		:	Ben Haubrich		#
#File		:	makefile		#
#Synopsis	:	makefile for vecalc	#
#################################################

.PHONY: clean debug test

VECALC_OBJ = vecalc.o vectorOps.o vectorOut.o vectorIn.o vectorMem.o
CFLAGS = -Wall -Wextra -std=c89
#The number of added tests for extend
EXTTENDTESTS = 0

vecalc:	vecalc.o vectorOps.o vectorOut.o vectorIn.o vectorMem.o
	gcc $(CFLAGS) $(VECALC_OBJ) -o vecalc 

debug: 	vecalc.o vectorOps.o vectorOut.o vectorIn.o vectorMem.o
	gcc $(CFLAGS) $(VECALC_OBJ) -o vecalc -g

clean: 
	rm *.o	
		
vecalc.o: vecalc.c vecalc.h
	gcc $(CFLAGS) -c vecalc.c

vectorOps.o: vectorOps.c vectorOps.h
	gcc $(CFLAGS) -c vectorOps.c

vectorIn.o: vectorIn.c vectorIn.h
	gcc $(CFLAGS) -c  vectorIn.c

vectorMem.o: vectorMem.c vectorMem.h
	gcc $(CFLAGS) -c vectorMem.c

test: 
	#To get good test results, send stderr off to /dev/null
	gcc $(CFLAGS) $(VECALC_OBJ) -o vecalc -DTESTING
	#make input file
	touch vecalcTestInput.txt
	#Test extend_vec
	printf "%s\n" "a 5" "a 55" "a 4" "a 7" "a5" "a55" "5a 5" "a 5_5" "a$%$#" > vecalcTestInput.txt
	#Test scalar_plus
	printf "%s\n" "+ 0" "+ 1" "+ 2" "2 + 1" "+ +" >> vecalcTestInput.txt
