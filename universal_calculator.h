#ifndef UNIVERSALCALCULATOR_H
#define UNIVERSALCALCULATOR_H

#include "universal_bio_info.h"

void calculate(struct universal_bio_params *bio_params, void *ptr, void (*callback_crunched)(void *, int));
int stop_simulation(struct universal_bio_params *bio_params, double i, double di, double execution_time);
double **alloc_conc_arr(struct universal_bio_params *bio_params, int point_count);
void free_conc_arr(struct universal_bio_params *bio_params, double **conc_arr);
void init_conc_arr(struct universal_bio_params *bio_params, double **conc_arr, int point_count);
double diffusion_increment(struct universal_bio_params *bio_params, int point, int layer, int substance, double **last);
double kinetics_increment(struct universal_bio_params *bio_params, int point, int layer, int substance, double **last);
double interface_concentration(struct universal_bio_params *bio_params, int interface, int substance, int point, double **curr);
double electrochem_concentration(struct universal_bio_params *bio_params, int electrochem, int product, double **curr);
double electrochem_current(struct universal_bio_params *bio_params, int electrochem, int reactant, double **curr);

#endif

