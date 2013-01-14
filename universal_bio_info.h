#ifndef UNIVERSALBIOINFO_H
#define UNIVERSALBIOINFO_H

enum resp_method
{
	DEFAULT_TIME, //0 - iki pusiausvyros
	MIN_TIME,     //1 - iki pusiausvyros su nurodytu minimaliu laiku
	FIXED_TIME    //2 - fiksuotas laikas
};

struct reaction {
	//Reagentų skaičius
	int reactant_count;
	//Reagentų masyvas
	int *reactants;
	//Stechiometrijos koeficientai prie reagentų
	int *react_stoichs;
	//Reakcijos laipsniai pagal reagentus
	double *react_orders;

	//Produktų skaičius
	int product_count;
	//Produktų masyvas
	int *products;
	//Stechiometrijos koeficientai prie produktų
	int *prod_stoichs;
	//Reakcijos greičio konstanta
	double k;
};

struct electrochem {
	//Pernešamų elektronų skaičius
	int ne;
	//Reagentų skaičius
	int reactant_count;
	//Reagentų masyvas
	int *reactants;
	//Stechiometrijos koeficientai prie reagentų
	int *react_stoichs;

	//Produktų skaičius
	int product_count;
	//Produktų masyvas
	int *products;
	//Stechiometrijos koeficientai prie produktų
	int *prod_stoichs;
};

struct layer {
	//Visų medžiagų difuzijos koeficientai sluoksnyje (cm^2/s)
	double *diff_coefs;
	//Visų medžiagų pradinės koncentracijos sluoksnyje (mol/cm^3)
	double *init_concs;
	//Kiek reakcijų vyksta šiame sluoksnyje
	int reaction_count;
	//Kurios reakcijos vyksta šiame sluoksnyje
	int *reactions;
	//Sluoksnio storis (cm)
	double d;
};

struct universal_bio_params {
	//Medžiagų skaičius
	int substance_count;
	//Reakcijų skaičius
	int reaction_count;
	//Reakcijos
	struct reaction *reactions;
	//Elektrocheminių reakcijų skaičius
	int electrochem_count;
	//Elektrocheminės reakcijos
	struct electrochem *electrochems;
	//Biojutiklio sluoksnių skaičius
	int layer_count;
	//Biojutiklio sluoksniai
	struct layer *layers;
	//Medžiagų, kurioms reikia taikyti nepratekėjimo sąlygą ant elektrodo paviršiaus, skaičius
	int electrode_neutral_count;
	//Medžiagos, kurioms reikia taikyti nepratekėjimo sąlygą ant elektrodo paviršiaus
	int *electrode_neutrals;
	//Medžiagų koncentracijos tirpale
	double *init_concs;
	//Žingsnis pagal laiką (s)
	double dt;
	//Į kiek dalių dalinami sluoksniai
	int n;
	//Metodas, kuriuo bus nustatomas atsako laikas:
	enum resp_method resp_t_meth;
	//Minimalus atsako laikas (s)
	double min_t;
	//Fiksuotas atsako laikas (s)
	double resp_t;
	//Išvedimo failas
	char *out_file_name;
};

#endif
