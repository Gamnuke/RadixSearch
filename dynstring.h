#ifndef _DYNSTRING_H_
#define _DYNSTRING_H_

typedef struct dynstring dynstring_t;

struct dynstring{
    char *A;
    int n;
    int size;
};

void dynstring_append(dynstring_t *dynstring, char character);
void dynstring_ensure_size(dynstring_t *dynstring);
void dynstring_copy(char *string, dynstring_t *target);
void dynstring_print(dynstring_t *string);
void dynstring_free(dynstring_t *string);
void dynstring_initialize(dynstring_t *string);

#endif