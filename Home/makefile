#------------------------------------------------------------------
# Montana K, Romel Mendoza
# Lab 2
# Lab X01L
# CMPT 360 
# -----------------------------------------------------------------


CC = gcc
CFLAGS = -Wall -std=c11

mrsh: mrsh.o dataStructures.o
		$(CC) $(CFLAGS) -o mrsh $^
test1: test1.o
		$(CC) $(CFLAGS) -o test1 $^
test2: test2.o
		$(CC) $(CFLAGS) -o test2 $^
mrsh.o: mrsh.c
		$(CC) $(CFLAGS) -c $^		
dataStructures.o: dataStructures.c
		$(CC) $(CFLAGS) -c $^
test1.o: test1.c
		$(CC) $(CFLAGS) -c $^
test2.o: test2.c
		$(CC) $(CFLAGS) -c $^

all:
	make mrsh
	make test1
	make test2

clean:
	-rm *.o mrsh test1 test2

.PHONY: tarball

tarball: dataStructures.c dataStructures.h mrsh.c mrsh.h makefile
		tar -czf cmpt360_lab_02_M?_RMM.tar.gz $^

