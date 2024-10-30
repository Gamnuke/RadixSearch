#ifndef _PROCESSORS_H_
#define _PROCESSORS_H_

#include <stdio.h>
#include <string.h>
#include "definitions.h"
#include "array.h"
#include "radix.h"

void process_data_file(FILE *input_file, array_t *array);
void process_query(FILE *output_file, char header[NUM_FIELDS][MAX_FIELD_LEN], array_t *queries);
void process_line(char* line, char data[NUM_FIELDS][MAX_FIELD_LEN]);

#endif