#------------------------------------------------------------------
# Montana K, Romel Mendoza
# Lab 6
# Lab X01L
# CMPT 360 
# -----------------------------------------------------------------


CC = gcc
CFLAGS = -Wall -std=c11
TARBALL_NAME = cmpt360_lab_06_MK_RMM

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
	-rm *.o mrsh test1 test2
	-rm test2.o bin/
	-rm test1.o bin/

.PHONY: tarball move-file

tarball: dataStructures.c dataStructures.h mrsh.c mrsh.h makefile bin
		mkdir $(TARBALL_NAME)
		mkdir -p $(TARBALL_NAME)/home
		mv $^ $(TARBALL_NAME)/home/
		tar -czf $(TARBALL_NAME).tar.gz $(TARBALL_NAME)/

