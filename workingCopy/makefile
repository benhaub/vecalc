#################################################
#Author		:	Ben Haubrich		#
#File		:	makefile		#
#Synopsis	:	makefile for vecalc	#
#################################################

.PHONY: clean debug

VECALC_OBJ = vecalc.o vectorOps.o vectorOut.o vectorIn.o vectorMem.o
CFLAGS = -Wall -Wextra -std=c89

vecalc:	vecalc.o vectorOps.o vectorOut.o vectorIn.o vectorMem.o
	gcc $(CFLAGS) $(VECALC_OBJ) -o vecalc 

debug: 	vecalc.o vectorOps.o vectorOut.o vectorIn.o vectorMem.o
	gcc $(CFLAGS) $(VECALC_OBJ) -o vecalc -g

clean: 
	rm *.o	
		
vecalc.o: vecalc.c vecalc.h
	gcc -Wall -c -std=c89 vecalc.c

vectorOps.o: vectorOps.c vectorOps.h
	gcc -Wall -c -std=c89 vectorOps.c

vectorIn.o: vectorIn.c vectorIn.h
	gcc -Wall -c -std=c89 vectorIn.c

vectorMem.o: vectorMem.c vectorMem.h
	gcc -Wall -c -std=c89 vectorMem.c

