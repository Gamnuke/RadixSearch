#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "stage_2.h"


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

    fclose(data_file);

    // process query file
    FILE* output_file = fopen(argsv[3], "w");
    
    array_t *queries = calloc(1, sizeof(array_t));
    process_query(output_file, header, queries);
    int bit_comps = 0;
    int char_comps = 0;
    int str_comps = 0;

    double times[1000];
    int time_i = 0;

    // goes through each query and searches it, then prints them to the output file
    for(int i = 0; i < queries->n; i++){

        array_t *found_array = calloc(1, sizeof(array_t));

        clock_t c;
        c = clock();
        binary_search(queries->A[i], data_array, found_array, &char_comps, &str_comps);
        c = clock() - c;
        times[time_i] = (double)(c);
        time_i++;

        bit_comps = char_comps * 8;
        printf("%s --> b%d c%d s%d\n", (char*)(queries->A[i]), bit_comps, char_comps, str_comps);

        fprintf(output_file, "%s\n", (char *)queries->A[i]);
        for(int i = 0; i < found_array->n; i++){
            fprintf(output_file, "--> ");

            for(int j = 0; j < NUM_FIELDS; j++){
                fprintf(output_file, "%s: %s || ", header[j], ((node_t *)found_array->A[i])->data[j]);
            }

            fprintf(output_file, "\n");
        }

        bit_comps = 0;
        char_comps = 0;
        str_comps = 0;
    }

    double average_time = 0;

    for(int i = 0; i < time_i; i++){
        average_time += times[i];
    }
    average_time = average_time/time_i;

    printf("Average time taken: %lf", average_time);

    fclose(output_file);
}