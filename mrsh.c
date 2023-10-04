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
#include "dataStructures.h"
#include "mrsh.h"

extern Library *lib;

int main()
{   
    //Initialize at startup
    //Library
    Library *lib = create_library();
    //Environmental Variables
    init_evn_variables(lib);
    //Command History
    
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
