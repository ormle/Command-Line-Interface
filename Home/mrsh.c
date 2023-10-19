/*
#-----------------------------------------------------------------
# Montana Kampe, Romel Mendoza
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
#include <sys/wait.h>
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
/* this function witll take in the calls array and the pid of a child
        process then call execute*/
void action_foreground(char **calls)
{
    pid_t cpid;
    cpid = fork();

    if(cpid < 0){ /* if cpid is less than 0 the fork failed*/
        fprintf(stderr,"Failed to execute");
        exit(1);
    }
    if (cpid > 0){//
        wait(NULL);//Zombie prevention
    }
    if (cpid == 0){ /* when cpid equals 0 then fork worked then can execute*/
        execvp(calls[0], calls);
        exit(0);
    }
}

/*
Runs the executable command in the background
*/
void action_background(char **calls){
    pid_t child, grandchild;
                
    child = fork();

    if (child < 0){
        //Fork failed
        fprintf(stderr, "Fork failed");
        exit(1);
    }
    if (child > 0){
        wait(NULL); //Zombie prevention
    }else{
        grandchild = fork();
        if (grandchild < 0){
            //Fork failed
            fprintf(stderr, "Fork failed");
            exit(1);
        }
        if (grandchild > 0){
            exit(0);//Orphan status
        }
        else{
            execvp(calls[0], calls);
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            exit(0);
        }
    }
}

/*
Reruns last matching application in the command history
*/
void run_last(Queue *History, char **calls)
{
    if (History->amount > 0){
        //Copy History to cycle through
        Queue *Hcopy = copy_queue(History);
        int a = Hcopy->amount;
        int j;
        //App to look for
        //Take out ! from string so we only match executable name
        char *app = malloc(strlen(&calls[0][1])+1);
        strcpy(app, &calls[0][1]);
        //printf("App: %s\n", app);
        for (j = 0; j < a; j++){
            //Get front item from queue
            char *f = front(Hcopy);
            //printf("Front: %s\n", f);
            //Compare
            if (strncmp(app, f, strlen(app)) == 0){
                //Execute
                calls[0] = app;
                action_foreground(calls);
                return;
            }
            else {dequeue(Hcopy);}
        }
        //destroy_queue(Hcopy);
    }
}

int main(int argc, char *argv[])
{   
    // storage space for user input and tokenizing
    //char cwd[PATH_MAX]; 
    char *cwd;
    char *choice = NULL;
    size_t choiceSize = 32;
    char *calls[100]; /*this will hold the user input so we can compare the string */
    char* whitespace = " =\t\n\f\r\v";/* possible characters to recognize for tokenizing input */
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
        
        if (History->amount == HISTSIZE){
            dequeue(History);
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
            if (strcmp(calls[1], "") == 0 || calls[1] == 0){
                continue;
            }
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
        else if (strncmp(&calls[0][0], "!", 1) == 0){
            /*Run last matching application in history*/
            run_last(History, calls);
        }
        else{
            /*Execute an executable*/
            if (strcmp(calls[i-1], "&") == 0){
                /*Run in background*/
                action_background(calls);
            }else{
                /*Run in foreground*/
                action_foreground(calls);
            }
        }
    }
    //Free memory
    destroy_library(lib);
    destroy_queue(History);
    return 0;
}


