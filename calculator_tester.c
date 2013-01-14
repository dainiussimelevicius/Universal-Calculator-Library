#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "universal_calculator.h"
#include "universal_bio_info.h"

void callback_crunched(void *ptr, int time)
{
	printf("%ds simulated\n", time);
}

int main()
{
	int i;
	struct universal_bio_params *bio_params;

	/*
	*Eox - 0
	*Ered - 1
	*R - 2
	*P - 3
	*S1 - 4
	*P1 - 5
	*S2 - 6
	*P2 - 7
	*/
	bio_params = malloc(sizeof(*bio_params));
	bio_params->substance_count = 8;

	//Medžiagų koncentracijos tirpale
	bio_params->init_concs = malloc(sizeof(*(bio_params->init_concs)) * bio_params->substance_count);
	bio_params->init_concs[0] = 0;
	bio_params->init_concs[1] = 0;
	bio_params->init_concs[2] = 40e-6;
	bio_params->init_concs[3] = 0;
	bio_params->init_concs[4] = 8e-6;
	bio_params->init_concs[5] = 0;
	bio_params->init_concs[6] = 4e-7;
	bio_params->init_concs[7] = 0;

	//Reakcijų informacija
	bio_params->reaction_count = 4;
	bio_params->reactions = malloc(sizeof(*(bio_params->reactions)) * bio_params->reaction_count);
	//0 reakcija
	bio_params->reactions[0].k = 1.25e7;
	bio_params->reactions[0].reactant_count = 2;
	bio_params->reactions[0].reactants = malloc(sizeof(*(bio_params->reactions[0].reactants)) * bio_params->reactions[0].reactant_count);
	bio_params->reactions[0].reactants[0] = 0;
	bio_params->reactions[0].reactants[1] = 2;
	bio_params->reactions[0].react_stoichs = malloc(sizeof(*(bio_params->reactions[0].react_stoichs)) * bio_params->reactions[0].reactant_count);
	bio_params->reactions[0].react_stoichs[0] = 1;
	bio_params->reactions[0].react_stoichs[1] = 1;
	bio_params->reactions[0].react_orders = malloc(sizeof(*(bio_params->reactions[0].react_orders)) * bio_params->reactions[0].reactant_count);
	bio_params->reactions[0].react_orders[0] = 1;
	bio_params->reactions[0].react_orders[1] = 1;
	bio_params->reactions[0].product_count = 2;
	bio_params->reactions[0].products = malloc(sizeof(*(bio_params->reactions[0].products)) * bio_params->reactions[0].product_count);
	bio_params->reactions[0].products[0] = 1;
	bio_params->reactions[0].products[1] = 3;
	bio_params->reactions[0].prod_stoichs = malloc(sizeof(*(bio_params->reactions[0].prod_stoichs)) * bio_params->reactions[0].product_count);
	bio_params->reactions[0].prod_stoichs[0] = 1;
	bio_params->reactions[0].prod_stoichs[1] = 1;
	//1 reakcija
	bio_params->reactions[1].k = 1.2e5;
	bio_params->reactions[1].reactant_count = 2;
	bio_params->reactions[1].reactants = malloc(sizeof(*(bio_params->reactions[1].reactants)) * bio_params->reactions[1].reactant_count);
	bio_params->reactions[1].reactants[0] = 1;
	bio_params->reactions[1].reactants[1] = 4;
	bio_params->reactions[1].react_stoichs = malloc(sizeof(*(bio_params->reactions[1].react_stoichs)) * bio_params->reactions[1].reactant_count);
	bio_params->reactions[1].react_stoichs[0] = 1;
	bio_params->reactions[1].react_stoichs[1] = 1;
	bio_params->reactions[1].react_orders = malloc(sizeof(*(bio_params->reactions[1].react_orders)) * bio_params->reactions[1].reactant_count);
	bio_params->reactions[1].react_orders[0] = 1;
	bio_params->reactions[1].react_orders[1] = 1;
	bio_params->reactions[1].product_count = 2;
	bio_params->reactions[1].products = malloc(sizeof(*(bio_params->reactions[1].products)) * bio_params->reactions[1].product_count);
	bio_params->reactions[1].products[0] = 0;
	bio_params->reactions[1].products[1] = 5;
	bio_params->reactions[1].prod_stoichs = malloc(sizeof(*(bio_params->reactions[1].prod_stoichs)) * bio_params->reactions[1].product_count);
	bio_params->reactions[1].prod_stoichs[0] = 1;
	bio_params->reactions[1].prod_stoichs[1] = 1;
	//2 reakcija
	bio_params->reactions[2].k = 1.4e9;
	bio_params->reactions[2].reactant_count = 2;
	bio_params->reactions[2].reactants = malloc(sizeof(*(bio_params->reactions[2].reactants)) * bio_params->reactions[2].reactant_count);
	bio_params->reactions[2].reactants[0] = 1;
	bio_params->reactions[2].reactants[1] = 6;
	bio_params->reactions[2].react_stoichs = malloc(sizeof(*(bio_params->reactions[2].react_stoichs)) * bio_params->reactions[2].reactant_count);
	bio_params->reactions[2].react_stoichs[0] = 1;
	bio_params->reactions[2].react_stoichs[1] = 1;
	bio_params->reactions[2].react_orders = malloc(sizeof(*(bio_params->reactions[2].react_orders)) * bio_params->reactions[2].reactant_count);
	bio_params->reactions[2].react_orders[0] = 1;
	bio_params->reactions[2].react_orders[1] = 1;
	bio_params->reactions[2].product_count = 2;
	bio_params->reactions[2].products = malloc(sizeof(*(bio_params->reactions[2].products)) * bio_params->reactions[2].product_count);
	bio_params->reactions[2].products[0] = 0;
	bio_params->reactions[2].products[1] = 7;
	bio_params->reactions[2].prod_stoichs = malloc(sizeof(*(bio_params->reactions[2].prod_stoichs)) * bio_params->reactions[2].product_count);
	bio_params->reactions[2].prod_stoichs[0] = 1;
	bio_params->reactions[2].prod_stoichs[1] = 1;
	//3 reakcija
	bio_params->reactions[3].k = 1.4e9;
	bio_params->reactions[3].reactant_count = 2;
	bio_params->reactions[3].reactants = malloc(sizeof(*(bio_params->reactions[3].reactants)) * bio_params->reactions[3].reactant_count);
	bio_params->reactions[3].reactants[0] = 4;
	bio_params->reactions[3].reactants[1] = 7;
	bio_params->reactions[3].react_stoichs = malloc(sizeof(*(bio_params->reactions[3].react_stoichs)) * bio_params->reactions[3].reactant_count);
	bio_params->reactions[3].react_stoichs[0] = 1;
	bio_params->reactions[3].react_stoichs[1] = 1;
	bio_params->reactions[3].react_orders = malloc(sizeof(*(bio_params->reactions[3].react_orders)) * bio_params->reactions[3].reactant_count);
	bio_params->reactions[3].react_orders[0] = 1;
	bio_params->reactions[3].react_orders[1] = 1;
	bio_params->reactions[3].product_count = 2;
	bio_params->reactions[3].products = malloc(sizeof(*(bio_params->reactions[3].products)) * bio_params->reactions[3].product_count);
	bio_params->reactions[3].products[0] = 5;
	bio_params->reactions[3].products[1] = 6;
	bio_params->reactions[3].prod_stoichs = malloc(sizeof(*(bio_params->reactions[3].prod_stoichs)) * bio_params->reactions[3].product_count);
	bio_params->reactions[3].prod_stoichs[0] = 1;
	bio_params->reactions[3].prod_stoichs[1] = 1;

	//Elektrocheminių reakcijų informacija
	bio_params->electrochem_count = 2;
	bio_params->electrochems = malloc(sizeof(*(bio_params->electrochems)) * bio_params->electrochem_count);
	//0 elektrocheminė reakcija
	bio_params->electrochems[0].ne = 1;
	bio_params->electrochems[0].reactant_count = 1;
	bio_params->electrochems[0].reactants = malloc(sizeof(*(bio_params->electrochems[0].reactants)) * bio_params->electrochems[0].reactant_count);
	bio_params->electrochems[0].reactants[0] = 5;
	bio_params->electrochems[0].react_stoichs = malloc(sizeof(*(bio_params->electrochems[0].react_stoichs)) * bio_params->electrochems[0].reactant_count);
	bio_params->electrochems[0].react_stoichs[0] = 1;
	bio_params->electrochems[0].product_count = 1;
	bio_params->electrochems[0].products = malloc(sizeof(*(bio_params->electrochems[0].products)) * bio_params->electrochems[0].product_count);
	bio_params->electrochems[0].products[0] = 4;
	bio_params->electrochems[0].prod_stoichs = malloc(sizeof(*(bio_params->electrochems[0].prod_stoichs)) * bio_params->electrochems[0].product_count);
	bio_params->electrochems[0].prod_stoichs[0] = 1;
	//1 elektrocheminė reakcija
	bio_params->electrochems[1].ne = 1;
	bio_params->electrochems[1].reactant_count = 1;
	bio_params->electrochems[1].reactants = malloc(sizeof(*(bio_params->electrochems[1].reactants)) * bio_params->electrochems[1].reactant_count);
	bio_params->electrochems[1].reactants[0] = 7;
	bio_params->electrochems[1].react_stoichs = malloc(sizeof(*(bio_params->electrochems[1].react_stoichs)) * bio_params->electrochems[1].reactant_count);
	bio_params->electrochems[1].react_stoichs[0] = 1;
	bio_params->electrochems[1].product_count = 1;
	bio_params->electrochems[1].products = malloc(sizeof(*(bio_params->electrochems[1].products)) * bio_params->electrochems[1].product_count);
	bio_params->electrochems[1].products[0] = 6;
	bio_params->electrochems[1].prod_stoichs = malloc(sizeof(*(bio_params->electrochems[1].prod_stoichs)) * bio_params->electrochems[1].product_count);
	bio_params->electrochems[1].prod_stoichs[0] = 1;

	//Sluoksnių informacija
	bio_params->layer_count = 3;
	bio_params->layers = malloc(sizeof(*(bio_params->layers)) * bio_params->layer_count);
	//0 sluoksnis
	bio_params->layers[0].d = 23.3e-4;
	bio_params->layers[0].reaction_count = 4;
	bio_params->layers[0].reactions = malloc(sizeof(*(bio_params->layers[0].reactions)) * bio_params->layers[0].reaction_count);
	bio_params->layers[0].reactions[0] = 0;
	bio_params->layers[0].reactions[1] = 1;
	bio_params->layers[0].reactions[2] = 2;
	bio_params->layers[0].reactions[3] = 3;
	bio_params->layers[0].diff_coefs = malloc(sizeof(*(bio_params->layers[0].diff_coefs)) * bio_params->substance_count);
	bio_params->layers[0].diff_coefs[0] = 0;
	bio_params->layers[0].diff_coefs[1] = 0;
	bio_params->layers[0].diff_coefs[2] = 3.15e-6;
	bio_params->layers[0].diff_coefs[3] = 3.15e-6;
	bio_params->layers[0].diff_coefs[4] = 3.15e-6;
	bio_params->layers[0].diff_coefs[5] = 3.15e-6;
	bio_params->layers[0].diff_coefs[6] = 3.15e-6;
	bio_params->layers[0].diff_coefs[7] = 3.15e-6;
	bio_params->layers[0].init_concs = malloc(sizeof(*(bio_params->layers[0].init_concs)) * bio_params->substance_count);
	bio_params->layers[0].init_concs[0] = 4e-8;
	bio_params->layers[0].init_concs[1] = 0;
	bio_params->layers[0].init_concs[2] = 0;
	bio_params->layers[0].init_concs[3] = 0;
	bio_params->layers[0].init_concs[4] = 0;
	bio_params->layers[0].init_concs[5] = 0;
	bio_params->layers[0].init_concs[6] = 0;
	bio_params->layers[0].init_concs[7] = 0;
	//1 sluoksnis
	bio_params->layers[1].d = 18.6e-4;
	bio_params->layers[1].reaction_count = 1;
	bio_params->layers[1].reactions = malloc(sizeof(*(bio_params->layers[1].reactions)) * bio_params->layers[1].reaction_count);
	bio_params->layers[1].reactions[0] = 3;
	bio_params->layers[1].diff_coefs = malloc(sizeof(*(bio_params->layers[1].diff_coefs)) * bio_params->substance_count);
	bio_params->layers[1].diff_coefs[0] = 0;
	bio_params->layers[1].diff_coefs[1] = 0;
	bio_params->layers[1].diff_coefs[2] = 4.2e-7;
	bio_params->layers[1].diff_coefs[3] = 4.2e-7;
	bio_params->layers[1].diff_coefs[4] = 4.2e-7;
	bio_params->layers[1].diff_coefs[5] = 4.2e-7;
	bio_params->layers[1].diff_coefs[6] = 4.2e-7;
	bio_params->layers[1].diff_coefs[7] = 4.2e-7;
	bio_params->layers[1].init_concs = malloc(sizeof(*(bio_params->layers[1].init_concs)) * bio_params->substance_count);
	bio_params->layers[1].init_concs[0] = 0;
	bio_params->layers[1].init_concs[1] = 0;
	bio_params->layers[1].init_concs[2] = 0;
	bio_params->layers[1].init_concs[3] = 0;
	bio_params->layers[1].init_concs[4] = 0;
	bio_params->layers[1].init_concs[5] = 0;
	bio_params->layers[1].init_concs[6] = 0;
	bio_params->layers[1].init_concs[7] = 0;
	//2 sluoksnis
	bio_params->layers[2].d = 116.5e-4;
	bio_params->layers[2].reaction_count = 1;
	bio_params->layers[2].reactions = malloc(sizeof(*(bio_params->layers[2].reactions)) * bio_params->layers[2].reaction_count);
	bio_params->layers[2].reactions[0] = 3;
	bio_params->layers[2].diff_coefs = malloc(sizeof(*(bio_params->layers[2].diff_coefs)) * bio_params->substance_count);
	bio_params->layers[2].diff_coefs[0] = 0;
	bio_params->layers[2].diff_coefs[1] = 0;
	bio_params->layers[2].diff_coefs[2] = 6.3e-6;
	bio_params->layers[2].diff_coefs[3] = 6.3e-6;
	bio_params->layers[2].diff_coefs[4] = 6.3e-6;
	bio_params->layers[2].diff_coefs[5] = 6.3e-6;
	bio_params->layers[2].diff_coefs[6] = 6.3e-6;
	bio_params->layers[2].diff_coefs[7] = 6.3e-6;
	bio_params->layers[2].init_concs = malloc(sizeof(*(bio_params->layers[2].init_concs)) * bio_params->substance_count);
	bio_params->layers[2].init_concs[0] = 0;
	bio_params->layers[2].init_concs[1] = 0;
	bio_params->layers[2].init_concs[2] = 0;
	bio_params->layers[2].init_concs[3] = 0;
	bio_params->layers[2].init_concs[4] = 0;
	bio_params->layers[2].init_concs[5] = 0;
	bio_params->layers[2].init_concs[6] = 0;
	bio_params->layers[2].init_concs[7] = 0;

	//Medžiagos, kurioms reikia taikyti nepratekėjimo sąlygą ant elektrodo paviršiaus
	bio_params->electrode_neutral_count = 1;
	bio_params->electrode_neutrals = malloc(sizeof(*(bio_params->electrode_neutrals)) * bio_params->electrode_neutral_count);
	bio_params->electrode_neutrals[0] = 2;

	bio_params->n = 200;

	double dt0 = pow(bio_params->layers[0].d / bio_params->n, 2) / (2 * bio_params->layers[0].diff_coefs[2]);
	double dt1 = pow(bio_params->layers[1].d / bio_params->n, 2) / (2 * bio_params->layers[1].diff_coefs[2]);
	double dt2 = pow(bio_params->layers[2].d / bio_params->n, 2) / (2 * bio_params->layers[2].diff_coefs[2]);
	bio_params->dt = 0.9 * fmin(dt0, fmin(dt1, dt2));

	bio_params->resp_t_meth = MIN_TIME;
	bio_params->min_t = 100;
	bio_params->resp_t = 0;
	bio_params->out_file_name = "output.dat";

	calculate(bio_params, NULL, &callback_crunched);

	free(bio_params->electrode_neutrals);
	for (i = 0; i < bio_params->layer_count; i++) {
		free(bio_params->layers[i].reactions);
		free(bio_params->layers[i].diff_coefs);
		free(bio_params->layers[i].init_concs);
	}
	free(bio_params->layers);
	for (i = 0; i < bio_params->electrochem_count; i++) {
		free(bio_params->electrochems[i].reactants);
		free(bio_params->electrochems[i].react_stoichs);
		free(bio_params->electrochems[i].products);
		free(bio_params->electrochems[i].prod_stoichs);
	}
	free(bio_params->electrochems);
	for (i = 0; i < bio_params->reaction_count; i++) {
		free(bio_params->reactions[i].reactants);
		free(bio_params->reactions[i].react_stoichs);
		free(bio_params->reactions[i].react_orders);
		free(bio_params->reactions[i].products);
		free(bio_params->reactions[i].prod_stoichs);
	}
	free(bio_params->reactions);
	free(bio_params->init_concs);
	free(bio_params);

	return 0;
}
