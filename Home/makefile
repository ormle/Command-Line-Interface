#------------------------------------------------------------------
# Montana K, Romel Mendoza
# Lab 5
# Lab X01L
# CMPT 360 
# -----------------------------------------------------------------


CC = gcc
CFLAGS = -Wall -std=c11
TARBALL_NAME =  cmpt360_lab_05_MK_RMM

all:
	make mrsh
	make test1
	make test2

mrsh: mrsh.o dataStructures.o
		$(CC) $(CFLAGS) -o mrsh $^
test1: test1.o
		$(CC) $(CFLAGS) -o bin/test1 $^
		
test2: test2.o
		$(CC) $(CFLAGS) -o bin/test2 $^
mrsh.o: mrsh.c
		$(CC) $(CFLAGS) -c $^		
dataStructures.o: dataStructures.c
		$(CC) $(CFLAGS) -c $^
test1.o: bin/test1.c
		$(CC) $(CFLAGS) -c $^
		mv test1.o bin/
test2.o: bin/test2.c
		$(CC) $(CFLAGS) -c $^
		mv test2.o bin/

clean:
	-rm *.o mrsh 
	-rm bin/test1 bin/test2

.PHONY: tarball move-file

tarball: dataStructures.c dataStructures.h mrsh.c mrsh.h makefile bin
		tar -czf Home/$(TARBALL_NAME).tar.gz $^

