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
	double *dxs;

	double dx;
	double dx0;
	double dx1;

	//Difuzijos koeficientų masyvas
	double *Ds;

	double D0;
	double D1;

	//Tinklo taškų skaičius per visus biojutiklio sluoksnius
	int point_count;

	//Iteracija pagal laiką
	long long int t;

	//Simuliacijos laikas sekundėmis
	double execution_time;

	//Kintamasis nurodo ties kuriuo sluoksniu esame
	int layer;
	
	//Rodyklė į sluoksnį
	struct layer *layer_ptr;

	//Rodyklė į elektrocheminę reakciją
	struct electrochem *electrochem_ptr;

	//Kintamasis nurodo ties kuria sluoksnių sandūra esame
	int interface;

	//Kintamasis nurodo, kad esame ties sluoksnio kraštu (ties sluoksnių sandūra)
	int is_boundary;

	int n = bio_params->n;
	double dt = bio_params->dt;
	int substance_count = bio_params->substance_count;
	int layer_count = bio_params->layer_count;
	int electrode_neutral_count = bio_params->electrode_neutral_count;
	int electrochem_count = bio_params->electrochem_count;
	int reaction_count = bio_params->reaction_count;
	char *out_file_name = bio_params->out_file_name;
	enum resp_method resp_t_meth = bio_params->resp_t_meth;
	double min_t = bio_params->min_t;
	double resp_t = bio_params->resp_t;
	int *electrode_neutrals = bio_params->electrode_neutrals;
	struct electrochem *electrochems = bio_params->electrochems;
	struct layer *layers = bio_params->layers;
	struct reaction *reactions = bio_params->reactions;

	Ds = malloc(sizeof(*Ds) * substance_count);

	//Apskaičiuojamas tinklo taškų skaičius per visus biojutiklio sluoksnius
	point_count = layer_count * n + 1;

	dxs = malloc(sizeof(*dxs) * layer_count);
	for (a = 0; a < layer_count; a++)
		dxs[a] = layers[a].d / n;

	//Išskiriama atmintis medžiagų koncentracijų masyvams
	last = alloc_conc_arr(substance_count, point_count);
	curr = alloc_conc_arr(substance_count, point_count);

	//Inicializuojami koncentracijų masyvai
	init_conc_arr(bio_params, last, point_count);
	init_conc_arr(bio_params, curr, point_count);

	create_file(out_file_name);

	t = 0;
	do {
		layer = 0;
		dx = dxs[layer];
		layer_ptr = layers + layer;
		for (a = 0; a < substance_count; a++)
			Ds[a] = layer_ptr->diff_coefs[a];

		//Iteruojama per tinklo taškus, skaičiuojamos medžiagų koncentracijos
		for (a = 1; a < point_count - 1; a++) {
			//Nustatome ar tai nėra sluoksnių sandūra
			is_boundary = (a % n == 0);

			//Reikšmės sluoksnių sandūrose bus skaičiuojamos vėliau pagal derinimo sąlygas
			if (!is_boundary) {
				for (b = 0; b < substance_count; b++) {
					curr[b][a] = last[b][a];
					curr[b][a] += diffusion_increment(a, Ds[b], dx, b, last) * dt;
					curr[b][a] += kinetics_increment(reaction_count, reactions, a, layer_ptr, b, last) * dt;
				}
			}
			else {
				layer++;
				dx = dxs[layer];
				layer_ptr = layers + layer;
				for (b = 0; b < substance_count; b++)
					Ds[b] = layer_ptr->diff_coefs[b];
			}
		}

		//Sluoksnių sandūroms pritaikomos derinimo sąlygos
		for (interface = 0; interface < layer_count - 1; interface++) {
			point = n * (interface + 1);
			dx0 = dxs[interface];
			dx1 = dxs[interface + 1];
			for (b = 0; b < substance_count; b++) {
				D0 = layers[interface].diff_coefs[b];
				D1 = layers[interface + 1].diff_coefs[b];
				curr[b][point] = interface_concentration(D0, D1, dx0, dx1, b, point, curr);
			}
		}

		//Nepratekėjimo sąlygos ant elektrodo paviršiaus
		for (a = 0; a < electrode_neutral_count; a++)
			curr[electrode_neutrals[a]][0] = curr[electrode_neutrals[a]][1];

		//Kraštinės sąlygos atitinkančios elektrochemines reakcijas
		for (a = 0; a < electrochem_count; a++) {
			electrochem_ptr = electrochems + a;
			for (b = 0; b < electrochem_ptr->product_count; b++)
				curr[electrochem_ptr->products[b]][0] = electrochem_concentration(electrochem_ptr, layers, electrochem_ptr->products[b], curr);
		}

		//Srovės tankio skaičiavimas
		i = 0;
		for (a = 0; a < electrochem_count; a++) {
			electrochem_ptr = electrochems + a;
			for (b = 0; b < electrochem_ptr->reactant_count; b++)
				i += electrochem_current(electrochem_ptr, layers, dxs[0], electrochem_ptr->reactants[b], curr);
		}

		di = fabs(i - last_i);
		last_i = i;

		//Masyvų sukeitimas vietomis
		swap_arrays(&curr, &last);

		//Apskaičiuojamas laikas
		t++;
		execution_time = t * dt;
		
		//Rezultatų spausdinimas
		if ((t % INTERVAL) == 0) {
			append_file(out_file_name, i, execution_time);
			if (callback_crunched != NULL)
				callback_crunched(ptr, (int) execution_time);
		}
	} while (!stop_simulation(resp_t_meth, min_t, resp_t, i, di, execution_time, dt));

	//Atspausdinamas paskutinis taškas
	append_file(out_file_name, i, execution_time);
	if (callback_crunched != NULL)
		callback_crunched(ptr, (int) execution_time);

	//Atlaisvinama atmintis
	free_conc_arr(substance_count, last);
	free_conc_arr(substance_count, curr);
	free(dxs);
	free(Ds);
}

inline int stop_simulation(enum resp_method resp_t_meth, double min_t, double resp_t, double i, double di, double execution_time, double dt)
{
	//Nustatoma ar tęsti simuliaciją
	switch (resp_t_meth) {
	case MIN_TIME:
		if (execution_time < min_t) {
			return 0;
		}
		//Jeigu jau pasiekė minimalų laiką, tuomet tikrinama pagal DEFAULT_TIME sąlygas
	case DEFAULT_TIME:
		if (i > 1e-30)
			return ((execution_time / i) * (di / dt) <= EPSILON);
		else
			return 0;
		break;
	case FIXED_TIME:
		return (execution_time >= resp_t);
	}
}

double **alloc_conc_arr(int substance_count, int point_count)
{
	int a;
	double **conc_arr;
	conc_arr = malloc(sizeof(*conc_arr) * substance_count);
	for (a = 0; a < substance_count; a++)
		conc_arr[a] = malloc(sizeof(*conc_arr[a]) * point_count);
	return conc_arr;
}

void free_conc_arr(int substance_count, double **conc_arr)
{
	int a;
	for (a = 0; a < substance_count; a++)
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

inline double diffusion_increment(int point, double D, double dx, int substance, double **last)
{
	return D * (last[substance][point - 1] - 2 * last[substance][point] + last[substance][point + 1]) / (dx * dx);
}

inline double kinetics_increment(int reaction_count, struct reaction *reactions, \
	int point, struct layer *layer, int substance, double **last)
{
	int a;
	int b;
	int reactant;
	int product;
	double increment = 0;
	double rate;
	for (a = 0; a < reaction_count; a++)
		//Patikriname ar reakcija vyksta šiame sluoksnyje
		if (found_in_array(layer->reactions, layer->reaction_count, a)) {
			reactant = found_in_array(reactions[a].reactants, reactions[a].reactant_count, substance);
			product = found_in_array(reactions[a].products, reactions[a].product_count, substance);
			if (reactant || product) {
				rate = reactions[a].k;
				for (b = 0; b < reactions[a].reactant_count; b++)
					rate *= last[reactions[a].reactants[b]][point];
				if (reactant)
					increment -= rate;
				else
					increment += rate;
			}
		}
	return increment;
}

inline double interface_concentration(double D0, double D1, double dx0, double dx1, \
	int substance, int point, double **curr)
{
	const double epsilon = 1e-21;

	//Jeigu šios medžiagos difuzijos koeficientai abiejuose sluoksniuose lygūs 0, tuomet paliekame esamą reikšmę
	if ((fabs(D0) > epsilon) || (fabs(D1) > epsilon))
		return (D1 * dx0 * curr[substance][point + 1] + D0 * dx1 * curr[substance][point - 1]) / (D0 * dx1 + D1 * dx0);
	else
		return curr[substance][point];
}

inline double electrochem_concentration(struct electrochem *electrochem, struct layer *layer0, int product, double **curr)
{
	//Išsirenkame pirmąjį elektrocheminės reakcijos reagentą
	int reactant = electrochem->reactants[0];
	//Reagento difuzijos koeficientas pirmajame sluoksnyje
	double Dr = layer0->diff_coefs[reactant];
	//Produkto difuzijos koeficientas pirmajame sluoksnyje
	double Dp = layer0->diff_coefs[product];

	return curr[product][1] + (Dr / Dp) * (curr[reactant][1] - curr[reactant][0]);
}

inline double electrochem_current(struct electrochem *electrochem, struct layer *layer0, double dx0, int reactant, double **curr)
{
	//Reagento difuzijos koeficientas pirmajame sluoksnyje
	double Dr = layer0->diff_coefs[reactant];

	return electrochem->ne * F * Dr * (curr[reactant][1] - curr[reactant][0]) / dx0;
}

inline int found_in_array(int *array, int length, double value)
{
	int a;
	for (a = 0; a < length; a++)
		if (array[a] == value)
			return 1;
	return 0;
}
