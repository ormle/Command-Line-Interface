#------------------------------------------------------------------
# Montana K, Romel Mendoza
# Lab 2
# Lab X01L
# CMPT 360 
# -----------------------------------------------------------------


CC = gcc
CFLAGS = -Wall -std=c11

dataStructures.o: dataStructures.c
		$(CC) $(CFLAGS) -c $^
mrsh.o: mrsh.c
		$(CC) $(CFLAGS) -c $^		
dataStructures: dataStructures.o
		$(CC) $(CFLAGS) -o dataStructures $^
mrsh: mrsh.o dataStructures.o
		$(CC) $(CFLAGS) -o mrsh $^

all:
	make mrsh

clean:
	-rm *.o mrsh

.PHONY: tarball

tarball: dataStructures.c dataStructures.h mrsh.c mrsh.h makefile
		tar -czf cmpt360_lab_02_M?_RMM.tar.gz $^

