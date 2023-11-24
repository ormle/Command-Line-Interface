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
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include "dataStructures.h"
#include "mrsh.h"

extern Library *lib;
extern ssize_t getline(char **lineptr, size_t *n, FILE *stream);

void evn_variables(Library *lib)
{
    //Get UID
    int uid = geteuid();
    //Get password
    struct passwd *pw = getpwuid(uid);
    
    char *user = pw->pw_name;
    char *home = pw->pw_dir;
    char *shell = pw->pw_shell;
    char host[256];
    char pwd[256];

    //These next 4 im not sure what I should set them to
    char *cc = NULL;
    char *editor = NULL;
    char *oldpwd = NULL;
    char *path = NULL;

    //Set variables
    gethostname(host, sizeof(host));
    //Change directory to home directory
    chdir(home);
    getcwd(pwd, sizeof(pwd));

    //If Null set to empty string
    if (user == NULL){user = "";}
    if (home == NULL){ home = "";}
    if (shell == NULL){shell = "";}
    
    if (cc == NULL){ cc = "";}//For now these are empty
    if (editor == NULL){ editor = "";}
    if (oldpwd == NULL){ oldpwd = "";}
    if (path == NULL) {path = "";}

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

/*
Takes the user input(choice) and splits it into tokens
Tokens are stored in order in calls
*/
void tokenize(char *choice, char **calls)
{
    char* whitespace = " =\t\n\f\r\v"; /* possible characters to recognize for tokenizing input */
    int i = 0;

    if (strlen(choice) > 1){ 
        calls[i] = strtok(choice, whitespace);
        while (calls[i] != NULL){
            calls[++i] = strtok(NULL,whitespace);
        }
    }
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
    if (child > 0){//Parent
        wait(NULL); //Zombie prevention
    }else{//Child
        grandchild = fork();
        if (grandchild < 0){
            //Fork failed
            fprintf(stderr, "Fork failed");
            exit(1);
        }
        if (grandchild > 0){//Child
            exit(0);//Orphan status
        }
        else{//Grandchild
            execvp(calls[0], calls);
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            exit(0);
        }
    }
}

/*
Redirects STDOUT to a file or a file to a command
Flag indicates whether this is being done in the background or not
*/
void redirection(char **calls, int flag){
    int rIn, rOut, inOriginal, outOriginal;
    int i = 0;

    outOriginal = dup(STDOUT_FILENO);//Keep original output
    inOriginal = dup(STDIN_FILENO);//Keep original input

    while (calls[i] != NULL){
        if (strcmp(calls[i],">") == 0){
            rOut = open(calls[i + 1], O_WRONLY | O_CREAT | O_TRUNC);
            if (rOut == -1){
                perror("open");
                exit(1);
            }
            dup2(rOut, STDOUT_FILENO);
            close(rOut);
            chmod(calls[i+1], S_IRUSR | S_IWUSR);//File permissions
            calls[i] = NULL;
            break;
        } else if (strcmp(calls[i], "<") == 0) {
            rIn = open(calls[i + 1], O_RDONLY);
            if (rIn == -1){
                perror("open");
                exit(1);
            }
            dup2(rIn, STDIN_FILENO);
            close(rIn);
            chmod(calls[i+1], S_IRUSR | S_IWUSR);//File permissions
            calls[i] = NULL;
            break;
        }
        i++;
    }

    if (flag){
        action_background(calls);
    }else { action_foreground(calls); }

    dup2(outOriginal, STDOUT_FILENO);//Make output back to original
    dup2(inOriginal, STDIN_FILENO);//Make input back to original
}

/*
Splits a piping command at the | into left and right command arrays
Left command contains the commands until the | 
Right command contains the commands after |
(Inconclusive, | is not added to either array)

llast and rlast become the spot in the array after all commands in array
to become NULL or to make adding extra commands easy
*/
void split_pipe(char **calls, char **leftCommand, char **rightCommand, int *llast, int *rlast)
{
    int i = 0; //To parse calls
    int j = 0; //To parse left/right command arrays
    //Fill left command first
    while (strcmp(calls[i], "|") != 0){
        leftCommand[j] = calls[i];
        i++;
        j++;
    }
    leftCommand[j] = NULL;
    i++;
    *llast = j;
    j = 0;
    //Fill right command 
    while (calls[i] != NULL){
        rightCommand[j] = calls[i];
        i++;
        j++;
    }
    rightCommand[j] = NULL;
    rightCommand[j+1] = NULL;
    
    *rlast = j;
}

/*
Deletes a file
*/
int deleteFile(char *filename)
{
    if (remove(filename) == 0) {
        //printf("%s deleted successfully.\n", filename);
        return 0; // Success
    } else {
        perror("Error deleting file");
        return 1; // Error
    }
}

/*
Runs a command with piping in the foreground
*/
void pipe_foreground(char **calls)
{
    char *leftCommand[30];
    char *rightCommand[30];
    int llast, rlast;
    split_pipe(calls, leftCommand, rightCommand, &llast, &rlast);
    char *fileName = "tmp.txt";

    int fd[2];
    pid_t pid;

    pipe(fd);
    pid = fork();

    if (pid != 0){ //Parent
        close(fd[1]);  //Close write end
        wait(NULL);//Wait for child process to exit
        chmod(fileName, S_IRUSR | S_IWUSR);//File permissions

        rightCommand[rlast] = fileName; 
        action_foreground(rightCommand);
        deleteFile(fileName); //Deletes the temporary file, Comment out if want to see it
    }
    else{ //Child
        close(fd[0]); //Close the reading end
        //Redirect stdout to a file
        leftCommand[llast] = ">";
        leftCommand[llast + 1] = fileName; 
        redirection(leftCommand, 0);
        exit(EXIT_SUCCESS);
    }
}
/*
Runs a command with piping in the background
*/
void pipe_background(char **calls)
{
    char *leftCommand[30];
    char *rightCommand[30];
    char *fileName = "tmp.txt";
    int llast, rlast;

    split_pipe(calls, leftCommand, rightCommand, &llast, &rlast);
    int fd[2];
    pid_t pidc, pidgc;

    pidc = fork();
    pipe(fd);

    if (pidc > 0){//Parent
        close(fd[1]); //Close write end
        wait(NULL);//Zombie prevention
        chmod(fileName, S_IRUSR | S_IWUSR);//Edit file permissions

        rightCommand[rlast-1] = fileName;
        action_background(rightCommand);
        deleteFile(fileName);//Deletes the temporary file, Comment out if want to see it
    }else{//Child
        pidgc = fork();
        if (pidgc > 0){//Child
            exit(0);//To make Grandchild orphan
        }
        else{//Grandchild
            close(fd[0]);//Close reading end
            //Redirect stdout to a file
            leftCommand[llast] = ">";
            leftCommand[llast + 1] = fileName;
            redirection(leftCommand, 1);
            exit(0);
        }
    }
}

/*
Raises a flag if piping character is found
*/
int pipe_flag(char *string)
{
    int pflag = 0;
    if (strstr(string, "|") != NULL){
        pflag = 1;//Set flag
    }
    return pflag;
}
/*
Raises a flag if redirection character is found
*/
int redirection_flag(char *string)
{
    int rflag = 0;
    if(strstr(string, ">") || strstr(string, "<")){
        rflag = 1;//Set flag
    }
    return rflag;
}
/*
Raises a flag if run-in-background character is found
*/
int background_flag(char *string)
{
    int bflag = 0;
    if (strstr(string, "&") != NULL){
        bflag = 1;//Set flag
    }
    return bflag;
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
                //Retokenize command line arguments
                int pflag = pipe_flag(f);//pipe flag
                int rflag = redirection_flag(f);//redirection flag
                int bflag = background_flag(f);//run in background flag
                char *last_call[100];
                
                tokenize(f, last_call); //Changes f and last_call
                //Execute
                if (pflag){
                    if (bflag){
                        pipe_background(last_call);
                    }else{ pipe_foreground(last_call); }
                }else if (rflag){
                    if (bflag){
                        redirection(last_call, 1);
                    }else{ redirection(last_call, 0); }
                }else{
                    if (bflag){
                        action_background(last_call);
                    }else{ action_foreground(last_call);}
                }
                return;
            }
            else { dequeue(Hcopy); }
        }//End for
        //destroy_queue(Hcopy);
    }//End if
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
    Library *lib = create_library();//Library
    Queue *History = create_queue(HISTSIZE);//Queue (Command history)
    evn_variables(lib);//Environmental Variables

    while (1)
    {
        int i = 0;
        //Flags -> 1=TRUE, 0=FALSE
        int bflag = 0; //Flag to indicate if execute command in background
        int rflag = 0; //Flag to indicate if redirection is being requested
        int pflag = 0; //Flag to indicate if piping in command line
        char* user = get_entry(lib, "USER");
        char* host = get_entry(lib, "HOST");
        cwd = get_entry(lib, "PWD");
        
        //getcwd(cwd, sizeof(cwd)); 
        printf("%s@%s:%s>>", user, host, cwd);
        //Read user input of any length
        getline(&choice, &choiceSize, stdin);

        /*Add to the command history,
          Dequeue last command if full
        */
        if (History->amount == HISTSIZE){
            dequeue(History);
        }
        enqueue(History, choice);

        //Check flags
        /*Check if pipe is being done*/
        pflag = pipe_flag(choice);//Set flag
        /*Check if redirection is being done*/
        rflag = redirection_flag(choice);//Set flag
        /*Check if background execution*/
        bflag = background_flag(choice);//Set flag

        /* tokenizes user input */ //segfaults if replaced with tokenize() fxn
        if (strlen(choice) > 1){ 
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
        else if (strcmp(calls[0], "cd") == 0){
            char* new_dir = NULL;
            int change_result;
            if (strcmp(calls[1], "~") == 0){
                new_dir = calls[1];
                change_result = chdir(get_entry(lib, "HOME"));
            }
            else{
                new_dir = calls[1];
                change_result = chdir(new_dir);
            }

            if(change_result == 0){

                char* old_pwd = get_entry(lib, "PWD");
                add_entry(lib, "OLDPWD", old_pwd);
                char cwd[256];
                getcwd(cwd, sizeof(cwd));
                add_entry(lib, "PWD", cwd);
            
            } else {
                perror("cd");
            }
        }
        else if (strcmp(calls[0],"pwd") == 0){
            char* pwd = get_entry(lib, "PWD");
            printf("%s\n",pwd);
        }
        
        else{
            /*Execute an executable*/
            if (bflag){
                /*Run in background*/
                if (pflag){//Pipe command
                    pipe_background(calls);
                }else if (rflag){//Redirection command
                    redirection(calls, 1);
                }
                else{ action_background(calls); }
            }else{
                /*Run in foreground*/
                if (pflag){//Pipe command
                    pipe_foreground(calls);
                }
                else if (rflag){//Redirection command
                    redirection(calls, 0);
                } 
                else{ action_foreground(calls); }
            }
        }
    }//End while
    //Free memory
    destroy_library(lib);
    destroy_queue(History);
    return 0;
}