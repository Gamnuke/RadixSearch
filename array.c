#include <stdlib.h>
#include "array.h"
#include <string.h>
#include <assert.h>

/*
adds an element to the array
*/
void array_append(array_t *array, void *element){
    array_ensure_size(array);
    int char_cmps = 0;

    if(array->n == 0){
        array->A[array->n] = element;
        (array->n)++;
    }
    else{
        for(int i = 0; i < array->n; i++){
            if(string_compare(((node_t *)(element))->data[QUERY_LOC], ((node_t *)(array->A[i]))->data[QUERY_LOC], &char_cmps) <= 0){
                // shift all upwards
                for(int j = array->n + 1; j > i; j--){
                    array->A[j] = array->A[j - 1];
                }
                array->A[i] = element;
                (array->n)++;
                return;
            }
        }

        // element is the most alphabetical so add to end of array
        array->A[array->n] = element;
        (array->n)++;
    }
}

void array_append_unsorted(array_t *array, void *element){
    array_ensure_size(array);

    array->A[array->n] = element;
    (array->n)++;
}

void binary_search(char key_name[MAX_FIELD_LEN], array_t *array, array_t *found_array, int *char_cmps, int *str_cmps){
    int lo = 0;
    int hi = array->n - 1;
    int mid = (lo + hi)/2;

    while(lo <= hi){
        
        // compare the key and each element in the array
        assert(array->A[mid]);
        node_t *mid_node = (node_t *)(array->A[mid]);


        int cmp_result = string_compare(key_name, mid_node->data[QUERY_LOC], char_cmps);
        (*str_cmps)++;

        // binary search stuff
        if(cmp_result < 0){
            hi = mid - 1;
        }
        else if(cmp_result > 0){
            lo = mid + 1;
        }
        else if(cmp_result == 0){
            array_append(found_array, mid_node);
            
            // linear search around the found item for duplicates
            for(int i = mid - 1; i >= 0; i--){
                node_t *i_node = (node_t *)(array->A[i]);
                int i_cmp_result = string_compare(key_name, i_node->data[QUERY_LOC], char_cmps);
                (*str_cmps)++;
                
                // we dont care about the alphabetical order return value
                // of string_compare, we just need to make sure it compares
                // the same number of characters as the key to get duplicates.
                if(i_cmp_result == 0){
                    array_append(found_array, (void *)i_node);
                }
                else{
                    // prefix not found
                    break;
                }
            }

            for(int i = mid + 1; i < array->n; i++){
                node_t *i_node = (node_t *)(array->A[i]);
                int i_cmp_result = string_compare(key_name, i_node->data[QUERY_LOC], char_cmps);
                (*str_cmps)++;
                // we dont care about the alphabetical order return value
                // of string_compare, we just need to make sure it compares
                // the same number of characters as the key to get duplicates.
                if(i_cmp_result == 0){
                    array_append(found_array, (void *)i_node);
                }
                else{
                    // prefix not found
                    break;
                }
            }

            return;
        }

        mid = (lo + hi)/2;
    }
}

/*
compares two string, returning whether string 2 is syntactically higher,
also incrementing the char_comp variable according to how many characters
were compared.
*/
int string_compare(
    char str1[MAX_FIELD_LEN], 
    char str2[MAX_FIELD_LEN],
    int *char_comps
    )
{
    int i = 0;
    while(str1[i] != '\0' && str2[i] != '\0') {
        (*char_comps)++;

        if(str1[i] < str2[i]){
            return -1;
        }
        else if(str1[i] > str2[i]){
            return 1;
        }
        i++;
    }

    (*char_comps)++;

    return 0;
}

/*
makes sure there is enough space for at least one element in the array
*/
void array_ensure_size(array_t *array){
    if(array->size == 0){
        array->A = calloc(1, sizeof(void *));
        (array->size)++;
    }
    if(array->size == array->n){
        array->A = realloc(array->A, sizeof(void *) * array->size * 2);
        (array->size) *= 2;
    }
}

/*
frees memory allocated for the array
*/
void array_free(array_t *array){
    if(array == NULL){
        return;
    }

    free(array->A);
    free(array);
}

/*
if the array contains nodes, use a different method to free the array
which frees each element
*/
void array_free_nodes(array_t *array){
    if(array == NULL){
        return;
    }

    for(int i = 0; i < array->n; i++){
        free(array->A[i]);
    }

    free(array->A);
    free(array);
}

/*
debug tool - prints an array
*/
void array_print(array_t *array, FILE *output_file, char header[NUM_FIELDS][MAX_FIELD_LEN]){
    for(int i = 0; i < array->n; i++){
        print_data(output_file, header, array->A[i]);
    }
}

/*
initializes an array
*/
array_t *array_new(){
    array_t *new_array = calloc(1, sizeof(array_t));
    assert(new_array);
    return new_array;
}