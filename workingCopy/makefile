#################################################
#Author		:	Ben Haubrich		#
#File		:	makefile		#
#Synopsis	:	makefile for vecalc	#
#################################################

.PHONY: clean debug test

VECALC_OBJ = vecalc.o vectorOps.o vectorOut.o vectorIn.o vectorMem.o
VECALC_C = vecalc.c vectorOps.c vectorOut.c vectorIn.c vectorMem.c
CFLAGS = -Wall -Wextra -std=c89

vecalc:	vecalc.o vectorOps.o vectorOut.o vectorIn.o vectorMem.o
	gcc $(CFLAGS) $(VECALC_OBJ) -o vecalc 

debug:  
	gcc $(CFLAGS) $(VECALC_C) -o vecalc -g
	#In order to use gdb, the program has to be totally recompiled for the
	#symbol table to properly generate

clean: 
	rm *.o
	rm core.*
		
vecalc.o: vecalc.c vecalc.h
	gcc $(CFLAGS) -c vecalc.c

vectorOps.o: vectorOps.c vectorOps.h
	gcc $(CFLAGS) -c vectorOps.c

vectorIn.o: vectorIn.c vectorIn.h
	gcc $(CFLAGS) -c  vectorIn.c

vectorMem.o: vectorMem.c vectorMem.h
	gcc $(CFLAGS) -c vectorMem.c

test: 
	#To add additional testing, give vecalc the clear option, and add start
	#additional testing onto the text file, continuing off from the 
	#loopCount. The loop count makes it too difficult to add more
	#testing in between existing tests.
	#You may want to consider discarding stderr when running tests
	gcc $(CFLAGS) $(VECALC_C) -o vecalc -DTESTING -g
	#make input file
	touch vecalcTestInput.txt
	#Test extend_vec. "a 5" is for loopCount 0, "a 55" is for loopCount 1, etc. [0, 8]
	printf "%s\n" "a 5" "a 55" "a 4" "a 7" "a5" "a55" "5a 5" "a 5_5" "a$%$#" > vecalcTestInput.txt
	#Test scalar_plus. "+ 0" is for loopCount 9, "+ 1" is for loopCount 10, etc. [9, 13]
	printf "%s\n" "+ 0" "+ 1" "+ 2" "2 + 1" "+ +" >> vecalcTestInput.txt
	#Test scalar_minus. [14, 20]
	printf "%s\n" "- 2" "- 4" "- 6" "-6" "6 -" "- -" "- +" >> vecalcTestInput.txt
	#Test scalar_mult. [21, 26]
	printf "%s\n" "* 1" "* 2" "* 0.5" "** 6" "* *" "* 1.5" >> vecalcTestInput.txt
	#Test scalar_div. [27, 33]
	printf "%s\n" "/ 3" "/ 6" "/ 0.5" "// 1" "/+* 6/" "7 /" "/ 0" >> vecalcTestInput.txt
	#Clear the current vector and start a new one
	printf "%s\n" "c" >> vecalcTestInput.txt
	printf "%s\n" "a 1" "a 2" "a 3" "a 4" "a 5" >> vecalcTestInput.txt
	#Test various illegal input [34, 40]. None of these should change the vector in any way
	printf "%s\n" " " "\0" "\"						" "./vecalc" ".*" "[ ]" >> vecalcTestInput.txt
	#501 character input [41, 41] - Test is 6 empty chars long.
	printf "%s\n" "     " >> vecalcTestInput.txt
	printf "%s\n" "q" >> vecalcTestInput.txt
	./vecalc < vecalcTestInput.txt 
	#rm -f vecalcTestInput.txt
