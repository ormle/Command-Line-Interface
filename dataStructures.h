/*
#-----------------------------------------------------------------
# Montana ?,Romel Mendoza
# Lab 1
# Lab X01L
# CMPT 360 Jeffrey Davis
#----------------------------------------------------------------
# Adapted from Lab 1
*/

//Represents an entry or environmental variable
typedef struct {
    char *Name; 
    char *Value;
} Entry;

typedef struct {
    int Array_size;   //Keeps track of max size of array
    int Num_entries; //Keeps track of how many entries in the array
    Entry *entryArray;
} Library;

extern Library *lib;

/*
 * Initializes the Library struct and returns a pointer to the
 * Library. Allocates memory for the Library and entries in the library
 */
Library* create_library();

/*
 * Frees all dynamically allocated memory
 */
void destroy_library(Library*);

/*
 * Reallocates memory for an array once it is fully filled
 * Array must be fully filled
 * Array size is doubled
 */
void grow_array(Library*);

/*
 * Prints all entries in the library
 */
void print_all_entries(Library*);

/*
 * Gets the requested name from the user
 */
char* get_request();

/*
 * Returns the value of an entry
 * Pre: index of entry which can be obtained through search_entry
 */
char* get_entry(Library*, char * requested_name);

/*
 * Removes an entry from the Library if it exists
 * Entry is freed and flagged with a null value
 */
void remove_entry(Library*, char * requested_name);

/* Helper fxn for get_entry
 * Returns the index of where the entry occurs in the Library
 * -1 if entry does not exist, otherwise return is > 0
 */
int search_entry(Library*, char * requested_name);

/*
 * Prints the entry onto the screen 
 */
void print_specific_entry(Library*, char * requested_name);

/* Helper fxn for new_entry
 * Adds an entry into the library as new last entry
 * If Library has reached max size, array will increase in size to make room
 */
void add_entry(Library*, char * entry_name, char * entry_value);

/*
 * Asks the user to enter name, value to enter into the data library
 * If the name exists, the previous value will be overwritten
 * otherwise it will be added to the library
 */
void new_entry(Library*);

/*
 *Clears the input buffer
 */
void clear_input_buffer();


//Struct to represent a queue
//Adapted from https://www.geeksforgeeks.org/introduction-and-array-implementation-of-queue/

typedef struct {
    int front;              //Keeps track of front position
    int back;               //Keeps track of back position
    int amount;             //Keeps track of the number of elements in the queue
    unsigned int maxSize;   //Maximum size of the queue
    char* array;            //Elements of the queue
} Queue;

/*
Creates a queue object with a given maxSize
Inital size of the queue is 0
*/
Queue* create_queue(unsigned int maxSize);

/*
Determines if the queue is full
1 = True  (Full queue)
0 = False (Space available)
*/
int is_full(Queue* queue);

/*
Determines if the queue is empty
1 = True (Empty queue)
0 = False (At least one element in queue)
*/
;
int is_empty(Queue* queue);

/*
Adds an item to the end of the queue (back end)
End and size are changed accordingly
*/
void enqueue(Queue* queue, char* value);

/*
Removes an item from the queue
Front and size are changed accordingly
*/
void dequeue(Queue* queue);

/*
Returns the front item of the queue
*/
char* front(Queue* queue);

/*
Returns the back item of the queue
*/
char* back(Queue* queue);
