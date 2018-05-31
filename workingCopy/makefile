#################################################
#Author		:	Ben Haubrich		#
#File		:	makefile		#
#Synopsis	:	makefile for vecalc	#
#################################################

.PHONY: clean debug profile

VECALC_OBJ = vecalc.o vectorOps.o vectorOut.o vectorIn.o vectorMem.o
VECALC_C = vecalc.c vectorOps.c vectorOut.c vectorIn.c vectorMem.c
CFLAGS = -Wall -Wextra -std=c89 -I$(PWD)/include
PWD = $(shell env | egrep -i '^pwd' | tr -d "PWD=")
LD_PATH = $(shell env | egrep -i ld_library_path | tr -d "LD_LIBRARY_PATH=")
VPATH = ./:$(PWD)/include

vecalc:	vecalc.o vectorOps.o vectorOut.o vectorIn.o vectorMem.o
	gcc $(CFLAGS) $(VECALC_OBJ) -o vecalc

dynamic:
	gcc $(CFLAGS) $(VECALC_C) -fPIC -c
	gcc $(CFLAGS) -shared -olibvector.so *.o
	mv libvector.so ./lib
	gcc $(CFLAGS) $(VECALC_C) -L$(PWD)/lib -o vecalc -lvector
	export LD_LIBRARY_PATH=$(LD_PATH):$(PWD)/lib 

profile:
	gcc $(CFLAGS) $(VECALC_C) -o vecalc -pg

clean:
	rm *.o
	rm core.*

pwd:
	echo $(PWD)

cflags:
	echo $(CFLAGS)
		
vecalc.o: vecalc.c vecalc.h
	gcc $(CFLAGS) -c vecalc.c

vectorOps.o: vectorOps.c vectorOps.h
	gcc $(CFLAGS) -c vectorOps.c

vectorIn.o: vectorIn.c vectorIn.h
	gcc $(CFLAGS) -c  vectorIn.c

vectorMem.o: vectorMem.c vectorMem.h
	gcc $(CFLAGS) -c vectorMem.c
