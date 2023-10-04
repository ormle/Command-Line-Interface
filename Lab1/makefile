#------------------------------------------------------------------
# Lab 1
# Lab X01L
# CMPT 360 
# -----------------------------------------------------------------


CC = gcc
CFLAGS = -Wall -std=c11

dataStructures.o: dataStructures.c
		$(CC) $(CFLAGS) -c $^
dataStructures: dataStructures.o
		$(CC) $(CFLAGS) -o dataStructures $^
testLibrary.o: testLibrary.c
		$(CC) $(CFLAGS) -c $^
testLibrary: testLibrary.o dataStructures.o
		$(CC) $(CFLAGS) -o testLibrary $^

all:
	make testLibrary
.PHONY:

tarball: dataStructures.c dataStructures.h testLibrary.c testInput.txt Q3.pdf makefile
		tar -czf cmpt360_lab_01_RMM.tar.gz $^

testdata:
		./testLibrary < testInput.txt > testOutput.txt



