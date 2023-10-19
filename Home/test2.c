/*
#-----------------------------------------------------------------
# Montana K, Romel M
# Lab 2
# Lab X01L
# CMPT 360 
#----------------------------------------------------------------
# This is a test file to test forking 
# Displays a message every 2 seconds 3 times (6 seconds overall?)
*/

#include<stdio.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
    int i;

    for (i = 0; i < 3; i++){
        printf("Hello!\n");
        sleep(2);
    }
    printf("DONE!\n");
}