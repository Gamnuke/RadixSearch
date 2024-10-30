#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "node.h"

/*
creates a new node
*/
node_t *make_new_node(){
    node_t *new_node = malloc(sizeof(node_t) + 1);
    assert(new_node);
    return new_node;
}

/*
prints the data of each node in the list to an output file
*/
void print_data(FILE* output_file, char header[NUM_FIELDS][MAX_FIELD_LEN], node_t *node){

    assert(node);
    
    fprintf(output_file, "%s\n%s", node->data[QUERY_LOC],"--> ");

    for(int i = 0; i < NUM_FIELDS; i++){
        fprintf(output_file, "%s: %s || ", header[i], node->data[i]);
    }
    fprintf(output_file, "\n");
}