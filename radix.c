#include "radix.h"
#include <stdlib.h>
#include <assert.h>

void radix_insert(radix_t *radix_tree, node_t *element, array_t *all_nodes){

    // create a new node for thte radix tree
    radix_node_t *new_node = radix_new_node();
    array_append_unsorted(all_nodes, (void *)(new_node));

    new_node->data_node = element;
    dynstring_copy(element->data[QUERY_LOC], new_node->string);
    new_node->prefix_bits = (new_node->string->n - 1) * 8;

    // create a prefix node to allow branching
    radix_node_t *new_prefix = radix_new_node();
    array_append_unsorted(all_nodes, (void *)(new_prefix));
    dynstring_copy(element->data[QUERY_LOC], new_prefix->string);

    // this is an array of 0s and 1s of the element we're inserting
    // in other words, a binary representation of the string we're inserting
    dynstring_t *this_bits = calloc(1, sizeof(dynstring_t));
    dynstring_initialize(this_bits);
    get_bits_from_string(new_node->data_node->data[QUERY_LOC], this_bits);

    // add first element, otherwise..
    if(radix_tree->root == NULL){
        radix_tree->root = new_node;
    }
    else{
        // go through each bit of the element we're inserting and compare it
        // with the prefix nodes.
        int bit_i = 0;

        radix_node_t *next = radix_tree->root;
        radix_node_t *previous = NULL;

        int previous_branch = 0;

        while(next != NULL){
            dynstring_t *prefix_bits = calloc(1, sizeof(dynstring_t));
            assert(prefix_bits);

            dynstring_initialize(prefix_bits);
            get_bit_section(next->string->A, prefix_bits, next->prefix_bits);

            while(bit_i < next->prefix_bits && bit_i < this_bits->n){

                // we deviated from this prefix, so make a new node
                if(this_bits->A[bit_i] > prefix_bits->A[bit_i]){

                    new_prefix->branchB = new_node;
                    new_prefix->branchA = next;

                    if(next == radix_tree->root){
                        radix_tree->root = new_prefix;
                    }

                    if(previous != NULL){
                        if(previous_branch == 0){
                            previous->branchA = new_prefix;
                        }
                        else if(previous_branch == 1){
                            previous->branchB = new_prefix;
                        }
                    }

                    dynstring_free(this_bits);
                    dynstring_free(prefix_bits);
                    return;
                }
                else if(this_bits->A[bit_i] < prefix_bits->A[bit_i]){
                    new_prefix->branchB = next;
                    new_prefix->branchA = new_node;

                    if(next == radix_tree->root){
                        radix_tree->root = new_prefix;
                    }
                    if(previous != NULL){
                        if(previous_branch == 0){
                            previous->branchA = new_prefix;
                        }
                        else if(previous_branch == 1){
                            previous->branchB = new_prefix;
                        }
                    }

                    dynstring_free(this_bits);
                    dynstring_free(prefix_bits);

                    return;
                }
                (new_prefix->prefix_bits)++;
                bit_i++;
            }

            // case where we found a duplicate
            if(bit_i == this_bits->n && bit_i == next->prefix_bits){

                // make a prefix and add the current prefix to the right, delete the prefix node
                // special case where a radix node becomes a prefix node with data, because its both
                // a data node AND a prefix
                new_node->branchB = next;
                new_node->prefix_bits = new_prefix->prefix_bits;

                if(next == radix_tree->root){
                    radix_tree->root = new_node;
                }

                if(previous != NULL){
                    if(previous_branch == 0){
                        previous->branchA = new_node;
                    }
                    else if(previous_branch == 1){
                        previous->branchB = new_node;
                    }
                }

                dynstring_free(this_bits);
                dynstring_free(prefix_bits);

                return;
            }

            // we've gone through the prefix, so move on to the next node.
            else if(bit_i >= this_bits->n){

                if(prefix_bits->A[bit_i] == 0){
                    new_prefix->branchA = next;
                    new_prefix->branchB = new_node;
                }
                else{
                    new_prefix->branchA = new_node;
                    new_prefix->branchB = next;
                }
                

                if(next == radix_tree->root){
                    radix_tree->root = new_prefix;
                }

                if(previous != NULL){
                    if(previous_branch == 0){
                        previous->branchA = new_prefix;
                    }
                    else if(previous_branch == 1){
                        previous->branchB = new_prefix;
                    }
                }

                dynstring_free(this_bits);
                dynstring_free(prefix_bits);

                return;
            } 
            else if(this_bits->A[bit_i] == 0){
                previous = next;
                next = next->branchA;
                previous_branch = 0;

                if(next == NULL){
                    previous->branchA = new_node;
                    dynstring_free(prefix_bits);
                    dynstring_free(this_bits);

                    return;
                }
            }
            else{
                previous = next;
                next = next->branchB;
                previous_branch = 1;

                if(next == NULL){
                    previous->branchB = new_node;
                    dynstring_free(prefix_bits);
                    dynstring_free(this_bits);

                    return;
                }
            }

            dynstring_free(prefix_bits);
        }
    }

    dynstring_free(this_bits);
}

radix_node_t *radix_new_node(){
    radix_node_t *new_node = calloc(1, sizeof(radix_node_t));
    assert(new_node);

    new_node->string = calloc(1, sizeof(dynstring_t));
    assert(new_node->string);
    dynstring_initialize(new_node->string);

    return new_node;
}

void get_bits(int key, int bit_array[8]){
    int number = key;

    for(int i = 0; i < 8; i++) {
        bit_array[8 - i - 1] = number % 2;
        number = number / 2;
    }
}

/*
extracts a section of bits from a string
*/
void get_bit_section(char *string, dynstring_t *bit_array, int bit_limit){
    int bit_count = 0;
    dynstring_t *bits = calloc(1, sizeof(dynstring_t));
    assert(bits);
    dynstring_initialize(bits);

    get_bits_from_string(string, bits);

    while(bit_count < bit_limit && bit_count < bits->n){
        dynstring_append(bit_array, bits->A[bit_count]);
        bit_count++;
    }

    dynstring_free(bits);
}

/*
prints bits
*/
void print_bits(int bit_array[8]){
    for(int i = 0; i < 8; i++){
        printf("%d", bit_array[i]);
    }
}
int bits_to_num(int bits[8]){
    int num = 0;
    int bit = 1;
    for(int i = 7; i >= 0; i--){
        num += bits[i] * bit;
        bit *= 2;
    }

    return num;
}

/*
prints a whole tree - used for debugging
*/
void print_tree(radix_node_t *start_node, int padding){
    assert(start_node);

    dynstring_t *all_bits = calloc(1, sizeof(dynstring_t));
    assert(all_bits);
    dynstring_initialize(all_bits);
    get_bits_from_string(start_node->string->A, all_bits);

    printf("Node: %s, Prefix bits: %d, Binary: ", start_node->string->A, start_node->prefix_bits);
    for(int i = 0; i < start_node->prefix_bits; i++){
        printf("%d", all_bits->A[i]);
    }
    printf("\n");
    
    if(start_node->branchA != NULL){
        for(int i = 0; i < padding + 1; i++){
            printf(" ");
        }
        printf("Branch A: ");
        print_tree(start_node->branchA, padding + 1);
    }

    if(start_node->branchB != NULL){
        for(int i = 0; i < padding + 1; i++){
            printf(" ");
        }
        printf("Branch B: ");
        print_tree(start_node->branchB, padding + 1);
    }

    dynstring_free(all_bits);
}

/*
returns a dynamic array where each element represents a bit
*/
void get_bits_from_string(char *string, dynstring_t *bit_array){
    int char_i = 0;

    while(string[char_i] != '\0'){
        int bits[8];
        get_bits(string[char_i], bits);

        for(int i = 0; i < 8; i++){
            dynstring_append(bit_array, bits[i]);
        }

        char_i++;
    }
}

/*
searches through the radix tree given a query string
*/
void radix_search(radix_node_t *start_node, char *query, array_t *found_array, int *bit_count){
    int bit_i = 0;

    // we can store the bits as a dynamic char array
    // and store the query as binary
    dynstring_t *bit_array = calloc(1, sizeof(dynstring_t));
    assert(bit_array);
    dynstring_initialize(bit_array);
    get_bits_from_string(query, bit_array);

    radix_node_t *next_node = start_node;

    // continue looking for the prefix node until we run out of query characters
    while(bit_i < bit_array->n){

        // if we run out of bits in the prefix but still have bits in the query...
        if(bit_i == next_node->prefix_bits){

            // go through the branches according to the next bit
            if(bit_array->A[bit_i] == 0 && next_node->branchA != NULL){
                next_node = next_node->branchA;
            }
            else if(bit_array->A[bit_i] == 1 && next_node->branchB != NULL){
                next_node = next_node->branchB;
            }
            else{
                // no branches left, so this node must be a leaf node
                break;
            }
        }

        (bit_i)++;
    }

    (*bit_count) = bit_i;

    if(next_node != NULL){
        radix_find_all(next_node, found_array);
    }

    dynstring_free(bit_array);
}

/*
recursively goes through all branches given a node and stores it in the found_array variable
*/
void radix_find_all(radix_node_t *next_node, array_t *found_array){
    if(next_node->data_node != NULL){
        array_append(found_array, next_node->data_node);
    }
    
    if(next_node->branchA != NULL){
        radix_find_all(next_node->branchA, found_array);
    }

    if(next_node->branchB != NULL){
        radix_find_all(next_node->branchB, found_array);
    }
}

/*
recursively frees a radix tree
*/
void radix_free_tree(radix_node_t *next_node){
    if(next_node->branchA != NULL){
        radix_free_tree(next_node->branchA);
    }
    if(next_node->branchB != NULL){
        radix_free_tree(next_node->branchB);
    }

    radix_free_node(next_node);
}

/*
frees a single node
*/
void radix_free_node(radix_node_t *node){
    if(node != NULL){
        if(node->data_node != NULL){
            free(node->data_node);
        }

        if(node->string != NULL){
            dynstring_free(node->string);
        }

        free(node);
    }
}