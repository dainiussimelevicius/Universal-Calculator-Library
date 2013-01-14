#include <stdio.h>
#include "utils.h"

void swap_arrays(double ***array1, double ***array2)
{
	double **tmp = *array1;
	*array1 = *array2;
	*array2 = tmp;
}

void fill_array(double *array, int length, double value)
{
	int a;
	for (a = 0; a < length; a++)
		array[a] = value;
}

void create_file(char *file_name)
{
	FILE *output_file = fopen(file_name, "w");
	fclose(output_file);
}

void append_file(char *file_name, double val1, double val2)
{
	FILE *output_file = fopen(file_name, "a");
	fprintf(output_file, "%e %e\n", val1, val2);
	fclose(output_file);
}
