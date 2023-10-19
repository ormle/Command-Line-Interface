/*
#-----------------------------------------------------------------
# Montana K, Romel M
# Lab 2
# Lab X01L
# CMPT 360 
#----------------------------------------------------------------
# Adapted from Lab 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "dataStructures.h"

Library * create_library() 
{
    int size = 10; //Starting size of library array

    Library * lib = malloc(sizeof(Library));
    //If malloc fails
    if (!lib) {
        fprintf(stderr, "Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    
    //Allocate memory for array
    lib->entryArray = malloc(sizeof(Entry) * size);//Array of Entry structs
    lib->Array_size = size; //Keeps track of array size
    lib->Num_entries = 0; // Keeps track of the number of entries in array
    //If malloc fails
    if (!lib->entryArray) {
        fprintf(stderr, "Error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    return lib;
}

void destroy_library(Library *lib)
{
    if (sizeof(lib) == 0) {
        printf("No Library to free\n");
        return;
    }

    //Free Name and values
    if (lib->Array_size > 0) {
        for (int i = 0; i < lib->Array_size; i++) {
            free(lib->entryArray[i].Name);
            free(lib->entryArray[i].Value);
        }
    }

    free(lib->entryArray);
    free(lib);
}

void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void new_entry(Library *lib)
{
    char *name = NULL;
    size_t nameSize = 32;
    //size_t characters; //Used in testing to count amount of char read
    char *value = NULL;
    size_t valueSize = 32;
    int len = 0, b = 1;

    while (b)
    {
        //Get Name Entry    
        printf("Enter name : ");
        //Read entered name until valid input 
        getline(&name, &nameSize, stdin);
        //Remove newline character if present from name
        len = strlen(name);
        if (strlen(name) <= 2 || strchr(name, ' ') != NULL)
        {
            printf("Name must be longer than 1 character and have no spaces\n");
            continue;
        }
        else if (len > 2 && name[len-1] == '\n') 
        {
            name[len-1] = '\0';
            b = 0;
        }
        //printf("Entered Name is: %s\n", name);
    }//end while
    
    b = 1;
    while (b)
    {
        //Get Value Entry
        printf("Enter value : ");
        //Read entered name until the press enter
        getline(&value, &valueSize, stdin);
        //Remove newline character if present from name
        len = strlen(value);
        if (strlen(value) <= 2 || strchr(value, ' ') != NULL)
        {
            printf("Value must be longer than 1 character and have no spaces\n");
            continue;
        }
        else if (len > 2 && value[len-1] == '\n') 
        {
            value[len-1] = '\0';
            b = 0;
        }
        //printf("Entered Value is: %s\n", value);
    }//end while
    //Add entries into array
    add_entry(lib, name, value);

    return;
    
}

void add_entry(Library *lib, char *entry_name, char *entry_value)
{
    //printf("Name : %s, Value: %s", entry_name, entry_value);
    
    int i = 0;
    int *size = &(lib->Array_size);    //Max array size
    int *amount = &(lib->Num_entries); //How many in the array
    Entry *entries = lib->entryArray;  //Array

    if (*amount == 0)
    {//Array is empty
        //Allocate memory for name
        entries[0].Name = malloc(strlen(entry_name)+1);
        //If malloc fails
        if (!entries[0].Name) {
            fprintf(stderr, "Error allocating memory");
            exit(EXIT_FAILURE);
        }
        //Copy name into array
        strcpy(entries[0].Name, entry_name);
        
        //Allocate memory for value
        entries[0].Value = malloc(strlen(entry_value)+1);
        //If malloc fails
        if (!entries[0].Value) {
            fprintf(stderr, "Error allocating memory");
            exit(EXIT_FAILURE);
        }
        //Copy value into array
        strcpy(entries[0].Value, entry_value);
        
        //Increase amount
        (*amount) ++;
    } 
    else if (*amount > 0)
    {//entries has at least one entry
        //printf("One entry!\n");
        for (i = 0; i < *size; i ++){
            if (entries[i].Name == NULL)
            {//entries has an empty space, not full
                //Allocate memory for name
                entries[i].Name = malloc(strlen(entry_name)+1);
                //If malloc fails
                if (!entries[i].Name) {
                    fprintf(stderr, "Error allocating memory");
                    exit(EXIT_FAILURE);
                }
                
                //Copy name into array
                strcpy(entries[i].Name, entry_name);
                
                //Allocate memory for value
                entries[i].Value = malloc(strlen(entry_value)+1);
                //If malloc fails
                if (!entries[i].Value) {
                    fprintf(stderr, "Error allocating memory");
                    exit(EXIT_FAILURE);
                }
                //Copy value into array
                strcpy(entries[i].Value, entry_value);
                
                //Increase amount
                (*amount) ++;
                if (*amount == *size) {
                    //entries has reached max capacity, increase array size
                    grow_array(lib);
                }
                break;
            }
            else if (strcmp(entries[i].Name, entry_name) == 0)
            {//Name is in array, replace value only
                //Free old value
                free(entries[i].Value);
                //Allocate memory for new value
                entries[i].Value = malloc(strlen(entry_value)+1);
                //If malloc fails
                if (!entries[i].Value) {
                    fprintf(stderr, "Error allocating memory");
                    exit(EXIT_FAILURE);
                }
                //Copy value into array
                strcpy(entries[i].Value, entry_value);
                break;
            }
        }//end for
    
    }//end else if
}

void grow_array(Library *lib)
{
    int *size = &(lib->Array_size);
    *size = *size * 2; //Double the array size
    //Reallocate entryArray 
    lib->entryArray = realloc(lib->entryArray, sizeof(Entry) * *size);
}

void print_all_entries(Library *lib)
{
    int i = 0, j = 0; //Counters, i for amount, j for size
    int *amount = &(lib->Num_entries);
    int *size = &(lib->Array_size);
    Entry *entries = lib->entryArray;  //Array

    if (*amount == 0)
    {//Array is empty, no entries to print
        printf("No entries to print\n");
        return;
    }
    else if (*amount > 0)
    {//At least one entry in the array
        //printf("Name=Value\n");
        while (j < *size || i < *amount)
        {
            if (entries[j].Name != NULL) {
                printf("%s=%s\n", entries[j].Name, entries[j].Value);
                i++;//To end once all names have been printed
            }
            j++;//To potentially go through the whole array
        }
    }
}

int search_entry(Library *lib, char *search_name)
{
    int i = 0, j = 0, flag = -1;//Flag to notify that search_name DNE
    int *amount = &(lib->Num_entries);
    int *size = &(lib->Num_entries);
    Entry *entries = lib->entryArray;  //Array
    
    if (*amount == 0)
    {//No entries to search for
        printf("No entries to search for.\n");
        return flag;
    }
    else if (*amount > 0)
    {
        while (j < *size || i < *amount)
        {
            if (entries[j].Name != NULL) {
                if (strcmp(entries[j].Name, search_name) == 0) {
                    return j;
                }
                i++;//To end once all names have been printed
            }
            j++;//To potentially go through the whole array
        }
        if (i >= *amount)
            {//Went through all entries and search_name does not exist
                printf("'%s' does not exist.\n", search_name);
                return flag;
            }
    }
    return i;
}

char * get_entry(Library *lib, char *search_name)
{
    int i = search_entry(lib, search_name);
    char * value = NULL;
    Entry *entries = lib->entryArray;

    if (i == -1)
    {
        value = NULL;
    }
    else if (i > -1)
    {
        //printf("Value: %s\n", entries[i].Value);
        value = entries[i].Value;
    }
    return value;
}

void remove_entry(Library *lib, char *remove_name)
{
    int *amount = &(lib->Num_entries);//Amount of entered entries
    int i = 0;//Counter
    Entry *entries = lib->entryArray;//Array

    if (*amount == 0)
    {//Array is empty, no entries to remove
        printf("No entries available to delete\n");
    }
    else if (*amount > 0)
    {//At least one entry in the array
        while (i < *amount)
        {
            if (strcmp(remove_name, entries[i].Name) == 0) {
                //Found a match
                //Free and flag with NULL
                free(entries[i].Name);
                free(entries[i].Value);
                entries[i].Name = NULL;
                entries[i].Value = NULL;
                //Decrease amount
                (*amount) --;
                //printf("Deleted val: %s", entries[i].Value);
                printf("%s entry has been deleted\n", remove_name);
                return;
            }
            i++;
        }
        if (i >= *amount) {
            //Went through all entries and remove_name DNE
                printf("'%s' does not exist.\n", remove_name);
            }

    }//end else if
}

char * get_request()
{
    char *request = NULL;
    size_t size = 0; //size of entered chars by user
    int b = 1, len = 0;//b for while loop
    
    while (b)
    {
        printf("Enter name: ");
        //Read entered name until the press enter and valid
        getline(&request, &size, stdin);
        len = strlen(request);
        //Make sure user input is valid
        if (strlen(request) <= 2 || strchr(request, ' ') != NULL)
        {
            printf("Input must be longer than 1 character and have no spaces\n");
            continue;
        }
        else if (len > 2 && request[len-1] == '\n') 
        {//Remove newline character if present from name
            request[len-1] = '\0';
            b = 0;
        }
    }
    return request;

}

void print_specific_entry(Library *lib, char * request)
{
    int i = search_entry(lib, request);
    Entry *entries = lib->entryArray;
    
    if (i > -1){
        printf("Name: %s ,Value: %s\n", entries[i].Name, entries[i].Value);
    }

}

char * capitalize(const char* str)
{
    char *capitalized = malloc(strlen(str) +1);
    if (capitalized == NULL){
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    strcpy(capitalized, str);
    for (int i = 0; i < strlen(capitalized); i++){
        capitalized[i] = toupper(capitalized[i]);
    }
    return capitalized;
}

Queue * create_queue(unsigned int maxSize)
{
    //Allocate memory for queue
    Queue *Q = malloc(sizeof(Queue));
    //If malloc fails
    if (Q == NULL) {
        perror("Failed to allocate memory for queue");
        exit(EXIT_FAILURE);
    }
    //Set max size
    Q->maxSize = maxSize;
    //Set front position
    Q->front = 0;
    //Set amount in queue
    Q->amount = 0;
    //Set the back position
    Q->back = 0;
    //Allocate memory for the queue
    Q->array = malloc(Q->maxSize * sizeof(Node*));
    //If malloc fails
    if (Q->array == NULL){
        perror("Failed to allocate memory for queue array");
        free(Q);
        exit(EXIT_FAILURE);
    }
    
    return Q;
}

Queue * copy_queue(Queue* original)
{
    Queue* copy = create_queue(original->maxSize);
    int oamount = original->amount;
    int count;

    for (count=0 ; count < oamount ; count++) {
        //Enqueue element from original to copy
        enqueue(copy, original->array[count].line);
    }

    return copy;
}

int is_full(Queue * Q)
{
    return (Q->maxSize == Q->amount);
}

int is_empty(Queue * Q)
{
    return (Q->amount == 0);
}

void enqueue(Queue * Q, char* value)
{
    //Check if the queue is full
    if (is_full(Q)){
        //Return if full
        return;
    }
    //Malloc
    Q->array[Q->back].line = malloc(strlen(value)+1);
    //Copy into queue
    strcpy(Q->array[Q->back].line, value);
    //Move back
    Q->back = (Q->back + 1) % Q->maxSize;
    Q->amount++;
}

void dequeue(Queue* Q)
{
    //Check if queue is empty
    if (is_empty(Q)){return;}

    free(Q->array[Q->front].line);
    Q->front = (Q->front + 1) % Q->maxSize;
    Q->amount--;
}

char* front(Queue* Q)
{
    if (is_empty(Q)){return "";}
    return Q->array[Q->front].line;
}

char* back(Queue* Q)
{
    if (is_empty(Q)){return "";}
    return Q->array[Q->back].line;
}

void print_queue(Queue* Q)
{
    //Iterate through Queue and print
    for (int i = 0; i < Q->amount; i++){
        printf("  %-3d %s", i,Q->array[i].line);
    }
}

void destroy_queue(Queue* Q)
{
    //Iterate through Queue and free
    for (int i = 0; i < Q->amount; i++){
        free(Q->array[i].line);
    }
    free(Q->array);
    free(Q);
}
