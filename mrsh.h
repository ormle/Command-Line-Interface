/*
#-----------------------------------------------------------------
# Montana ?,Romel Mendoza
# Lab 1
# Lab X01L
# CMPT 360 Jeffrey Davis
#----------------------------------------------------------------
# Adapted from Lab 1
*/

#define HISTSIZE = 5;

extern Library *lib;

/*
Setting up HISTSIZE of entered commands
*/
char * init_history();

/*
Adds commands to the command history
If history is full, last entry will be deleted and other entries will shift by one entry
*/
void add_history(char *command);

/*
Print the history of all commands entered
*/
void print_history();

/*
Returns from the application
*/
void quit_exit();

/*
Displays all environmental variables
*/
void print_evn_variables();

/*
Sets an environmental variable
*/
void export(char *name, char *value, Library *library);