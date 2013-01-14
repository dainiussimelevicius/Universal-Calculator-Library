#ifndef UNIVERSALCALCULATOR_H
#define UNIVERSALCALCULATOR_H

#include "universal_bio_info.h"

void calculate(struct universal_bio_params *bio_params, void *ptr, void (*callback_crunched)(void *, int));
inline int stop_simulation(enum resp_method resp_t_meth, double min_t, double resp_t, double i, double di, double execution_time, double dt);
double **alloc_conc_arr(int substance_count, int point_count);
void free_conc_arr(int substance_count, double **conc_arr);
void init_conc_arr(struct universal_bio_params *bio_params, double **conc_arr, int point_count);
inline double diffusion_increment(int point, double D, double dx, int substance, double **last);
inline double kinetics_increment(int reaction_count, struct reaction *reactions, \
	int point, struct layer *layer, int substance, double **last);
inline double interface_concentration(double D0, double D1, double dx0, double dx1, \
	int substance, int point, double **curr);
inline double electrochem_concentration(struct electrochem *electrochem, struct layer *layer0, int product, double **curr);
inline double electrochem_current(struct electrochem *electrochem, struct layer *layer0, double dx0, int reactant, double **curr);
inline int found_in_array(int *array, int length, double value); 

#endif

