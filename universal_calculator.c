#include <stdlib.h>
#include <math.h>
#include "universal_calculator.h"
#include "universal_bio_info.h"
#include "constants.h"
#include "utils.h"

void calculate(struct universal_bio_params *bio_params, void *ptr, void (*callback_crunched)(void *, int))
{
	int a;
	int b;

	int point;

	//Srovės tankis
	double i, last_i = 0;

	//Kintamasis rodo kaip pakito srovės tankis nuo praėjusios iteracijos
	double di;

	//Medžiagų koncentracijų masyvai
	double **last, **curr;

	//Žingsnių pagal erdvę masyvas
	double *space_steps;

	//Tinklo taškų skaičius per visus biojutiklio sluoksnius
	int point_count;

	//Iteracija pagal laiką
	long long int t;

	//Simuliacijos laikas sekundėmis
	double execution_time;

	//Kintamasis nurodo ties kuriuo sluoksniu esame
	int layer;

	//Kintamasis nurodo ties kuria sluoksnių sandūra esame
	int interface;

	//Kintamasis nurodo, kad esame ties sluoksnio kraštu (ties sluoksnių sandūra)
	int is_boundary;

	//Apskaičiuojamas tinklo taškų skaičius per visus biojutiklio sluoksnius
	point_count = bio_params->layer_count * bio_params->n + 1;

	//Išskiriama atmintis medžiagų koncentracijų masyvams
	last = alloc_conc_arr(bio_params, point_count);
	curr = alloc_conc_arr(bio_params, point_count);

	//Inicializuojami koncentracijų masyvai
	init_conc_arr(bio_params, last, point_count);
	init_conc_arr(bio_params, curr, point_count);

	create_file(bio_params->out_file_name);
	
	t = 0;
	do {
		//Iteruojama per gardelės taškus, skaičiuojamos medžiagų koncentracijos
		layer = 0;
		for (a = 1; a < point_count - 1; a++) {
			//Nustatome ar tai nėra sluoksnių sandūra
			is_boundary = (a % bio_params->n == 0);
			
			//Reikšmės sluoksnių sandūrose bus skaičiuojamos vėliau pagal derinimo sąlygas
			if (!is_boundary)
				for (b = 0; b < bio_params->substance_count; b++) {
					curr[b][a] = last[b][a];
					curr[b][a] += diffusion_increment(bio_params, a, layer, b, last) * bio_params->dt;
					curr[b][a] += kinetics_increment(bio_params, a, layer, b, last) * bio_params->dt;
				}
			else
				layer++;
		}

		//Sluoksnių sandūroms pritaikomos derinimo sąlygos
		for (interface = 0; interface < bio_params->layer_count - 1; interface++) {
			point = bio_params->n * (interface + 1);
			for (b = 0; b < bio_params->substance_count; b++)
				curr[b][point] = interface_concentration(bio_params, interface, b, point, curr);
		}

		//Nepratekėjimo sąlygos ant elektrodo paviršiaus
		for (a = 0; a < bio_params->electrode_neutral_count; a++)
			curr[bio_params->electrode_neutrals[a]][0] = curr[bio_params->electrode_neutrals[a]][1];

		//Kraštinės sąlygos atitinkančios elektrochemines reakcijas
		for (a = 0; a < bio_params->electrochem_count; a++)
			for (b = 0; b < bio_params->electrochems[a].product_count; b++)
				curr[bio_params->electrochems[a].products[b]][0] = electrochem_concentration(bio_params, a, bio_params->electrochems[a].products[b], curr);

		//Srovės tankio skaičiavimas
		i = 0;
		for (a = 0; a < bio_params->electrochem_count; a++)
			for (b = 0; b < bio_params->electrochems[a].reactant_count; b++)
				i += electrochem_current(bio_params, a, bio_params->electrochems[a].reactants[b], curr);

		di = fabs(i - last_i);
		last_i = i;

		//Masyvų sukeitimas vietomis
		swap_arrays(&curr, &last);

		//Apskaičiuojamas laikas
		t++;
		execution_time = t * bio_params->dt;
		
		//Rezultatų spausdinimas
		if ((t % INTERVAL) == 0) {
			append_file(bio_params->out_file_name, i, execution_time);
			if (callback_crunched != NULL)
				callback_crunched(ptr, (int) execution_time);
		}
	} while (!stop_simulation(bio_params, i, di, execution_time));

	//Atspausdinamas paskutinis taškas
	append_file(bio_params->out_file_name, i, execution_time);
	if (callback_crunched != NULL)
		callback_crunched(ptr, (int) execution_time);

	//Atlaisvinama atmintis
	free_conc_arr(bio_params, last);
	free_conc_arr(bio_params, curr);
}

int stop_simulation(struct universal_bio_params *bio_params, double i, double di, double execution_time)
{
	//Nustatoma ar tęsti simuliaciją
	switch (bio_params->resp_t_meth) {
	case MIN_TIME:
		if (execution_time < bio_params->min_t) {
			return 0;
		}
		//Jeigu jau pasiekė minimalų laiką, tuomet tikrinama pagal DEFAULT_TIME sąlygas
	case DEFAULT_TIME:
		if (i > 1e-30)
			return ((execution_time / i) * (di / bio_params->dt) <= EPSILON);
		else
			return 0;
		break;
	case FIXED_TIME:
		return (execution_time >= bio_params->resp_t);
	}
}

double **alloc_conc_arr(struct universal_bio_params *bio_params, int point_count)
{
	int a;
	double **conc_arr;
	conc_arr = malloc(sizeof(*conc_arr) * bio_params->substance_count);
	for (a = 0; a < bio_params->substance_count; a++)
		conc_arr[a] = malloc(sizeof(*conc_arr[a]) * point_count);
	return conc_arr;
}

void free_conc_arr(struct universal_bio_params *bio_params, double **conc_arr)
{
	int a;
	for (a = 0; a < bio_params->substance_count; a++)
		free(conc_arr[a]);
	free(conc_arr);
}

void init_conc_arr(struct universal_bio_params *bio_params, double **conc_arr, int point_count)
{
	int a;
	int b;

	//Inicializuojame koncentracijas ant elektrodo paviršiaus
	for (a = 0; a < bio_params->substance_count; a++)
		conc_arr[a][0] = bio_params->layers[0].init_concs[a];
	for (a = 0; a < bio_params->electrochem_count; a++)
		for (b = 0; b < bio_params->electrochems[a].reactant_count; b++)
			conc_arr[bio_params->electrochems[a].reactants[b]][0] = 0;

	//Inicializuojame koncentracijas sluoksnių viduje
	for (a = 0; a < bio_params->substance_count; a++)
		for (b = 0; b < bio_params->layer_count; b++)
			fill_array(&conc_arr[a][b * bio_params->n + 1], bio_params->n - 1, bio_params->layers[b].init_concs[a]);

	//Inicializuojame koncentracijas sluoksnių sandūrose
	for (a = 0; a < bio_params->substance_count; a++)
		for (b = 1; b < bio_params->layer_count; b++)
			conc_arr[a][b * bio_params->n] = fmax(bio_params->layers[b - 1].init_concs[a], bio_params->layers[b].init_concs[a]);

	//Inicializuojame koncentracijas sandūroje su tirpalu
	for (a = 0; a < bio_params->substance_count; a++)
		conc_arr[a][point_count - 1] = bio_params->init_concs[a];
}

double diffusion_increment(struct universal_bio_params *bio_params, int point, int layer, int substance, double **last)
{
	double dx = bio_params->layers[layer].d / bio_params->n;
	double D = bio_params->layers[layer].diff_coefs[substance];
	return D * (last[substance][point - 1] - 2 * last[substance][point] + last[substance][point + 1]) / (dx * dx);
}

double kinetics_increment(struct universal_bio_params *bio_params, int point, int layer, int substance, double **last)
{
	int a;
	int b;
	int reactant;
	int product;
	double increment = 0;
	double rate;
	for (a = 0; a < bio_params->reaction_count; a++)
		//Patikriname ar reakcija vyksta šiame sluoksnyje
		if (found_in_array(bio_params->layers[layer].reactions, bio_params->layers[layer].reaction_count, a)) {
			reactant = found_in_array(bio_params->reactions[a].reactants, bio_params->reactions[a].reactant_count, substance);
			product = found_in_array(bio_params->reactions[a].products, bio_params->reactions[a].product_count, substance);
			if (reactant || product) {
				rate = bio_params->reactions[a].k;
				for (b = 0; b < bio_params->reactions[a].reactant_count; b++)
					rate *= last[bio_params->reactions[a].reactants[b]][point];
				if (reactant)
					increment -= rate;
				else
					increment += rate;
			}
		}
	return increment;
}

double interface_concentration(struct universal_bio_params *bio_params, int interface, int substance, int point, double **curr)
{
	int layer0 = interface;
	int layer1 = interface + 1;
	double D0 = bio_params->layers[layer0].diff_coefs[substance];
	double D1 = bio_params->layers[layer1].diff_coefs[substance];
	double dx0 = bio_params->layers[layer0].d / bio_params->n;
	double dx1 = bio_params->layers[layer1].d / bio_params->n;
	const double epsilon = 1e-21;

	//Jeigu šios medžiagos difuzijos koeficientai abiejuose sluoksniuose lygūs 0, tuomet paliekame esamą reikšmę
	if ((fabs(D0) > epsilon) || (fabs(D1) > epsilon))
		return (D1 * dx0 * curr[substance][point + 1] + D0 * dx1 * curr[substance][point - 1]) / (D0 * dx1 + D1 * dx0);
	else
		return curr[substance][point];
}

double electrochem_concentration(struct universal_bio_params *bio_params, int electrochem, int product, double **curr)
{
	//Išsirenkame pirmąjį elektrocheminės reakcijos reagentą
	int reactant = bio_params->electrochems[electrochem].reactants[0];
	//Reagento difuzijos koeficientas pirmajame sluoksnyje
	double Dr = bio_params->layers[0].diff_coefs[reactant];
	//Produkto difuzijos koeficientas pirmajame sluoksnyje
	double Dp = bio_params->layers[0].diff_coefs[product];

	return curr[product][1] + (Dr / Dp) * (curr[reactant][1] - curr[reactant][0]);
}

double electrochem_current(struct universal_bio_params *bio_params, int electrochem, int reactant, double **curr)
{
	//Reagento difuzijos koeficientas pirmajame sluoksnyje
	double Dr = bio_params->layers[0].diff_coefs[reactant];
	//dx pirmajame sluoksnyje
	double dx0 = bio_params->layers[0].d / bio_params->n;

	return bio_params->electrochems[electrochem].ne * F * Dr * (curr[reactant][1] - curr[reactant][0]) / dx0;
}
