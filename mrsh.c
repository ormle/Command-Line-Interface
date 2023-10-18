/*
#-----------------------------------------------------------------
# Montana K, Romel Mendoza
# Lab 1
# Lab X01L
# CMPT 360 
#----------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "dataStructures.h"
#include "mrsh.h"

extern Library *lib;
extern ssize_t getline(char **lineptr, size_t *n, FILE *stream);

void evn_variables(Library *lib)
{
    //Set variables
    char *cc = getenv("CC");
    char *editor = getenv("EDITOR");
    char *home = getenv("HOME");
    char *oldpwd = getenv("OLDPWD");
    char *host = getenv("HOST");
    char *path = getenv("PATH");
    char *pwd = getenv("PWD");
    char *shell = getenv("SHELL");
    char *user = getenv("USER");

    //If Null set to empty string
    if (cc == NULL){ cc = "";}
    if (editor == NULL){ editor = "";}
    if (home == NULL){ home = "";}
    if (oldpwd == NULL){ oldpwd = "";}
    if (host == NULL){host = "";}
    if (path == NULL) {path = "";}
    if (pwd == NULL){pwd = "";}
    if (shell == NULL){shell = "";}
    if (user == NULL){user = "";}

    //Add to environment
    add_entry(lib, "CC", cc);
    add_entry(lib, "EDITOR", editor);
    add_entry(lib, "HOME", home);
    add_entry(lib, "OLDPWD", oldpwd);
    add_entry(lib, "HOST", host);
    add_entry(lib, "PATH", path);
    add_entry(lib, "PWD", pwd);
    add_entry(lib, "SHELL", shell);
    add_entry(lib, "USER", user);
}

int main(int argc, char *argv[])
{   
    // storage space for user input and tokenizing
    //char cwd[PATH_MAX]; 
    char *cwd;
    char *choice = NULL;
    size_t choiceSize = 32;
    char *calls[100]; /*this will hold the user input so we can compare the string */
    char* whitespace = " =\t\n\f\r\v";/* possible charaters to recognize for tokenizing input */
    int cmdCount = 0;//To count number of commands
    //Initialize at startup
    //Library
    Library *lib = create_library();
    //Queue (Command history)
    Queue *History = create_queue(HISTSIZE);
    //Environmental Variables
    evn_variables(lib);

    

    while (1)
    {
        int i = 0;
        char* user = get_entry(lib, "USER");
        char* host = get_entry(lib, "HOST");
        cwd = get_entry(lib, "PWD");
        /*
        Lab02 specs say currentDir is taken from 
        the environemntal variables, commented out for possible future use
        */
        //getcwd(cwd, sizeof(cwd)); 
        printf("%s@%s:%s>>", user, host, cwd);
        //Read user input of any length
        getline(&choice, &choiceSize, stdin);
        
        cmdCount++;
        if (cmdCount > HISTSIZE){
            dequeue(History);
            cmdCount = 0;
        }
        enqueue(History, choice);

        if (strlen(choice) > 1){ /* tokenizes user input */
            calls[i] = strtok(choice, whitespace);
            while (calls[i] != NULL){
                calls[++i] = strtok(NULL,whitespace);
            }
        }

        if (strcmp(calls[0], "exit") == 0 || strcmp(calls[0], "quit") == 0) 
        /*return from application*/
        {break;}
        else if (strcmp(calls[0], "export") == 0) {
            /* Set an environmental variable  */
            //Need to capitialize environmental variables
            char *name = capitalize(calls[1]);
            add_entry(lib, name, calls[2]);//Set environmental variable
            free(name);
        }
        else if (strcmp(calls[0], "env") == 0) {
            /*Displays all environmental variables*/
            print_all_entries(lib);
        }
        else if (strcmp(calls[0], "history") == 0) {
            /*Displays command history*/
            print_queue(History);
        }
    }
    //Free memory
    destroy_library(lib);
    destroy_queue(History);
    return 0;
}


