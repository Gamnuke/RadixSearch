#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "stage_3.h"

int main(int argc, char* argsv[]){
    FILE* data_file = fopen(argsv[2], "r");

    // keep track of where we are at on each line of the CSV file
    int char_num = 0;
    char read_buffer[MAX_LINE_LEN];

    // skip the header line in the csv file
    char header[NUM_FIELDS][MAX_FIELD_LEN];
    while((read_buffer[char_num] = fgetc(data_file)) != '\n') {
        char_num++;
    }
    process_line(read_buffer, header);
    char_num = 0;

    // process the data file containing records into a list
    array_t *data_array = calloc(1, sizeof(array_t));
    process_data_file(data_file, data_array);

    // create array to store nodes to free later
    array_t *all_nodes = calloc(1, sizeof(array_t));

    radix_t *tree = calloc(1, sizeof(radix_t));
    for(int i = 0; i < data_array->n; i++){
        radix_insert(tree, (node_t*)(data_array->A[i]), all_nodes);
    }

    fclose(data_file);

    // process query file
    FILE* output_file = fopen(argsv[3], "w");
    
    array_t *queries = calloc(1, sizeof(array_t));
    process_query(output_file, header, queries);
    int bit_comps = 0;
    int char_comps = 0;
    int str_comps = 1;

    double times[1000];
    int time_i = 0;

    // goes through each query, searches and then outputs it
    for(int i = 0; i < queries->n; i++){
        
        while(((char *)(queries->A[i]))[char_comps] != '\0'){
            char_comps++;
        }

        array_t *found_array = calloc(1, sizeof(array_t));
        clock_t c;
        c = clock();
        radix_search(tree->root, queries->A[i], found_array, &bit_comps);
        c = clock() - c;
        times[time_i] = (double)(c);
        time_i++;

        printf("%s --> b%d c%d s%d\n", (char*)(queries->A[i]), bit_comps, char_comps, str_comps);

        fprintf(output_file, "%s\n", (char *)queries->A[i]);
        for(int i = 0; i < found_array->n; i++){
            fprintf(output_file, "--> ");

            for(int j = 0; j < NUM_FIELDS; j++){
                fprintf(output_file, "%s: %s || ", header[j], ((node_t *)found_array->A[i])->data[j]);
            }

            fprintf(output_file, "\n");
        }

        array_free(found_array);
    }

    double average_time = 0;

    for(int i = 0; i < time_i; i++){
        average_time += times[i];
    }
    average_time = average_time/time_i;

    printf("Average time taken: %lf", average_time);

    // free all nodes
    for(int i = 0; i < all_nodes->n; i++){
        if(all_nodes->A[i] != NULL){
            radix_free_node(all_nodes->A[i]);
        }
    }

    array_free(all_nodes);
    array_free(queries);
    array_free(data_array);
    free(tree);

    fclose(output_file);
}