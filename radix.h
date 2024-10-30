#ifndef _RADIX_H_
#define _RADIX_H_

#include "dynstring.h"
#include "node.h"
#include "array.h"

typedef struct radix radix_t;
typedef struct radix_node radix_node_t;

struct radix{
    radix_node_t *root;
};

struct radix_node{
    int prefix_bits;
    dynstring_t *string;
    radix_node_t *branchA;
    radix_node_t *branchB;

    node_t *data_node;
};

void radix_insert(radix_t *radix_tree, node_t *element, array_t *all_nodes);
void get_bits(int key, int bit_array[8]);
void print_bits(int bit_array[8]);
int bits_to_num(int bits[8]);
void print_tree(radix_node_t *start_node, int padding);
void radix_search(radix_node_t *start_node, char *query, array_t *found_array, int *bit_count);
void radix_find_all(radix_node_t *next_node, array_t *found_array);
void get_bits_from_string(char *string, dynstring_t *bit_array);
void get_bit_section(char *string, dynstring_t *bit_array, int bit_limit);
void radix_free_tree(radix_node_t *next_node);
void radix_free_node(radix_node_t *node);
radix_node_t *radix_new_node();

#endif


