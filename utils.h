#ifndef UTILS_H
#define UTILS_H

void swap_arrays(double ***array1, double ***array2);
void fill_array(double *array, int length, double value);
int found_in_array(int *array, int length, double value); 
void create_file(char *file_name);
void append_file(char *file_name, double val1, double val2);

#endif
