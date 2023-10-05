/*
#-----------------------------------------------------------------
# Montana , Romel Mendoza
# Lab 1
# Lab X01L
# CMPT 360 Jeffrey Davis
#----------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include "dataStructures.h"
#include "mrsh.h"

extern Library *lib;

int main(int argc, char *argv[])
{   
    // strage space for user input and tokenizing
    char cwd[PATH_MAX];
    char choice[100];
    char *calls[100];
    char* whitespace = " \t\n\f\r\v";
    //Initialize at startup
    //Library
    Library *lib = create_library();
    //Queue
    Queue *History = create_queue(HISTSIZE);
    //Environmental Variables
    init_evn_variables(lib);
    //Command History

    while (1)
    {
        int i = 0;

        getcwd(cwd, sizeof(cwd));
        printf("%s >>", cwd);
        fgets(choice, PATH_MAX,stdin);

        if (strlen(choice) > 1){ /* tokenizes user input */
            calls[i] = strtok(choice, whitespace);
            while (calls[i] != NULL){
                calls[++i] = strtok(NULL,whitespace);
            }
        if (strcmp(calls[0], "exit",10) == 0) /*change directory*/
        {break;
        
        }
    }
    
}
return 0;
}

void evn_variables(Library *lib)
{
    add_entry(lib, "CC", "");
    add_entry(lib, "EDITOR", "");
    add_entry(lib, "HOME", "");
    add_entry(lib, "OLDPWD", "");
    add_entry(lib, "HOST", "host");
    add_entry(lib, "PATH", "");
    add_entry(lib, "PWD", "");
    add_entry(lib, "SHELL", "");
    add_entry(lib, "USER", "user");
}
