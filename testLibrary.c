/*
#-----------------------------------------------------------------
# Romel Mendoza
# Lab 1
# Lab X01L
# CMPT 360 Jeffrey Davis
#----------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include "dataStructures.h"

void print_operations()
{
    printf("\nOperations:\n");
    printf("1: New entry\n");
    printf("2: Delete entry\n");
    printf("3: Get Value\n");
    printf("4: Print a specific entry\n");
    printf("5. Print all entries\n");
    printf("6. Quit\n");

}

int get_option()
{
    int choice, numOptions = 6;
    while (1)
    {
        printf("Enter operation: ");
        if (scanf("%d", &choice) != 1)
        {
            //Invalid input
            printf("Invalid input please enter a valid number.\n");
            //Clear input buffer
            scanf("%*[^\n]");
        }
        else 
        {//Check if in valid range
            if (choice < 1 || choice > numOptions) 
            {
                printf("Number out of range. Please enter a number between 1-5\n");
            }
            else    
            {
                return choice;
            }
        }

    }
}

/*
 * Tests the functionality of the dataStructures library
 */
int main()
{
    int option = 0;
    char *request;
    Library *lib = create_library();
    
    while (1)
    {
        print_operations(); 

        //Get user choice
        option = get_option();
        clear_input_buffer();

        switch(option)
        {
            case 1:
                new_entry(lib);
                break;
            case 2:
                request = get_request();
                remove_entry(lib, request);
                free(request);
                break;
            case 3:
                request = get_request();
                get_entry(lib, request);
                free(request);
                break;
            case 4:
                request = get_request();
                print_specific_entry(lib, request);
                free(request);
                break;
            case 5:
                print_all_entries(lib);
                break;
            case 6:
                printf("Goodbye!\n");
                return 0; //Exit the program
                break;
            default:
                printf("Invalid option. Please choose a valid option.\n");
                break;
        }
    }
    destroy_library(lib);

}

