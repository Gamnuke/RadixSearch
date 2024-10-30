#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "node.h"

typedef struct array array_t;


struct array {
    void **A;
    int size;
    int n;
};

array_t *array_new();
void array_append_unsorted(array_t *array, void *element);
void array_free_nodes(array_t *array);
void array_append(array_t *array, void *element);
void array_ensure_size(array_t *array);
void array_free(array_t *array);
void array_print(array_t *array, FILE *output_file, char header[NUM_FIELDS][MAX_FIELD_LEN]);
int string_compare(
    char str1[MAX_FIELD_LEN], 
    char str2[MAX_FIELD_LEN],
    int *char_comps
    );

void binary_search(char key_name[MAX_FIELD_LEN], array_t *array, array_t *found_array, int *char_cmps, int *str_cmps);

#endif