#ifndef _NODE_H_
#define _NODE_H_

#include <stdio.h>
#include "definitions.h"

typedef struct node node_t;

struct node {
    node_t* next;
    char data[NUM_FIELDS][MAX_FIELD_LEN];
};

void print_data(FILE* output_file, char header[NUM_FIELDS][MAX_FIELD_LEN], node_t *node);
node_t *make_new_node();

#endif