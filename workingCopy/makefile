#################################################
#Author		:	Ben Haubrich		#
#File		:	makefile		#
#Synopsis	:	makefile for vecalc	#
#################################################

# targets that don't produce a file of the same name
.PHONY: clean debug profile

VECALC_OBJ = vecalc.o vectorOps.o vectorOut.o vectorIn.o vectorMem.o
VECALC_C = vecalc.c vectorOps.c vectorOut.c vectorIn.c vectorMem.c
# flags for the C compiler
CFLAGS = -Wall -Wextra -std=c89 -I$(PWD)/include
# Stores the current working directory
PWD = $(shell env | egrep -i '^pwd' | tr -d "PWD=")
# LD_PATH stores the directory that programs search by for dynamically
# loaded libraries. The dynamic rule adds the lib directory to this
# environment variable. This variables varies on some flavours of linux. it may
# need to be changed if dynamically loading libraries are not working.
LD_PATH = $(shell env | egrep -i ld_library_path | tr -d "LD_LIBRARY_PATH=")
# VPATH is a pre-defined variable that tells make where to look for header files
VPATH = ./:$(PWD)/include

vecalc:	vecalc.o vectorOps.o vectorOut.o vectorIn.o vectorMem.o
	gcc $(CFLAGS) $(VECALC_OBJ) -o vecalc

dynamic:
	gcc $(CFLAGS) $(VECALC_C) -fPIC -c
	gcc $(CFLAGS) -shared -o libvector.so *.o
	mv libvector.so ./lib
	export LD_LIBRARY_PATH=$(LD_PATH):$(PWD)/lib 
	gcc $(CFLAGS) $(VECALC_C) -L$(PWD)/lib -o vecalc -lvector
	
profile:
	gcc $(CFLAGS) $(VECALC_C) -o vecalc -pg

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
