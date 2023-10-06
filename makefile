#------------------------------------------------------------------
# Montana ?, Romel Mendoza
# Lab 2
# Lab X01L
# CMPT 360 
# -----------------------------------------------------------------


CC = gcc
CFLAGS = -Wall -std=c11

dataStructures.o: dataStructures.c
		$(CC) $(CFLAGS) -c $^
dataStructures: dataStructures.o
		$(CC) $(CFLAGS) -o dataStructures $^
mrsh.o: mrsh.c
		$(CC) $(CFLAGS) -c $^
mrsh: mrsh.c
		$(CC) $(CFLAGS) -o mrsh $^

all:
	make mrsh

clean:
	-rm *.o 

.PHONY: tarball

tarball: dataStructures.c dataStructures.h mrsh.c mrsh.h makefile
		tar -czf cmpt360_lab_02_M?_RMM.tar.gz $^

