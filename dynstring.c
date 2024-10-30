#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dynstring.h"


/*
adds an element to the dynstring
*/
void dynstring_append(dynstring_t *dynstring, char character){
    dynstring_ensure_size(dynstring);
    dynstring->A[dynstring->n] = character;
    (dynstring->n)++;
}

/*
initialize the string with default values
*/
void dynstring_initialize(dynstring_t *string){
    string->n = 0;
    string->size = 0;
    dynstring_ensure_size(string);
}

/*
makes sure there is enough space for at least one element in the dynstring
*/
void dynstring_ensure_size(dynstring_t *dynstring){
    if(dynstring->size == 0){
        dynstring->A = calloc(1, sizeof(char));
        (dynstring->size)++;
    }
    if(dynstring->size == dynstring->n){
        dynstring->A = realloc(dynstring->A, sizeof(char) + sizeof(char) * dynstring->size * 2);
        (dynstring->size) *= 2;
    }
}

/*
copies a string and dynamically stores it as a dynstring
*/
void dynstring_copy(char *string, dynstring_t *target){
    int i = 0;
    while(string[i] != '\0'){
        dynstring_append(target, string[i]);
        i++;
    }

    dynstring_append(target, '\0');
}

/*
prints the string
*/
void dynstring_print(dynstring_t *string){
    printf("%s\n", string->A);
}

/*
frees the string
*/
void dynstring_free(dynstring_t *string){
    if(string != NULL){
        if(string->n > 0){
            free(string->A);
        }
        free(string);
    }
}
