#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "processors.h"

/* 
processes the data file and returns an unsorted array of nodes
*/ 
void process_data_file(FILE *input_file, array_t *array){
    int char_num = 0;
    char character;
    char read_buffer[MAX_LINE_LEN];

    // go through each character in the data file
    while((character = fgetc(input_file)) != EOF) {
        read_buffer[char_num] = character;
        char_num++;

        // make a new node for each line in the CSV file
        if(character == '\n'){
            node_t *new_node = make_new_node();

            process_line(read_buffer, new_node->data);

            array_append(array, new_node);

            char_num = 0;

            // clear line array
            for(int i = 0; i < MAX_LINE_LEN; i++){
                read_buffer[i] = '\0';
            }
        }
    }
}

/*
processes the query file, and adds the query as a string to the queries array
*/
void process_query(FILE *output_file, char header[NUM_FIELDS][MAX_FIELD_LEN], array_t *queries){

    // keep track of position of the array
    int char_num = 0;
    char character;

    char *query = calloc(1, sizeof(char) * MAX_LINE_LEN);
    assert(query);
    
    while(scanf("%c", &character) != EOF){
        if(character == '\n'){

            // process this query and search
            array_append_unsorted(queries, query);
            char_num = 0;

            query = calloc(1, sizeof(char) * MAX_LINE_LEN);
            assert(query);
        }
        else{
            query[char_num] = character;
            char_num++;
        }
    }
    
    free(query);
}

/*
processes a line of characters into an array of strings
*/
void process_line(char* line, char data[NUM_FIELDS][MAX_FIELD_LEN]){

    // keep track of position of the array
    int line_char_i = 0;
    int char_i = 0;
    int field_i = 0;

    int quote_switch = 0;

    // go through each character
    while(line[line_char_i] != '\n'){

        // check if the field is bounded by quotation marks
        if(line[line_char_i] == '"'){
            quote_switch = !quote_switch;
        }

        // if statement to break each field into a new array location
        if(line[line_char_i] == ',' && quote_switch == 0){
            data[field_i][char_i] = '\0';
            field_i++;
            char_i = 0;
        }
        else if(line[line_char_i] != '"'){
            data[field_i][char_i] = line[line_char_i];
            char_i++;
        }

        line_char_i++;
    }

    data[field_i][char_i] = '\0';
}

