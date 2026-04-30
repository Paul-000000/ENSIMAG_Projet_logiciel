

#include "commande.h"
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static struct option options[] = {
	{"outfile", required_argument, NULL, 0},
	{"sample", 	required_argument, NULL, 1},
	{"help", 	no_argument,       NULL, 2},
	{0, 0, 0, 0}
};


// récupère les valeurs des paramètres dans une ligne de commande
bool recuperer_parametres_commande(int argc, char **argv, struct parametres_commande_t *parametres, bool *facteurs_initialises) {

	if (argc < 2) return false;

	parametres->chemin_entree = NULL;
	parametres->chemin_sortie = NULL;
	parametres->help = false;
	*facteurs_initialises = false;

	int opt;
	optind = 1;
	
	while ((opt = getopt_long(argc, argv, "", options, NULL)) != -1) {
		
		switch (opt) {
			
			case 0: // outfile

				parametres->chemin_sortie = optarg;
				break;

			case 1: // sample
				if (optarg == NULL) return false;
				
				int args_corrects = sscanf(
					optarg,
					"%hhux%hhu,%hhux%hhu,%hhux%hhu",
					&(parametres->facteurs.h1),
					&(parametres->facteurs.v1),
					&(parametres->facteurs.h2),
					&(parametres->facteurs.v2),
					&(parametres->facteurs.h3),
					&(parametres->facteurs.v3)
				);

				if (args_corrects != 6) return false;

				*facteurs_initialises = true;
				break;

			case 2: // help
				parametres->help = true;
				return true;
			
			default:
				return false;
			}
	}

	if (optind >= argc) return false;

	parametres->chemin_entree = argv[optind];

	return true;
}

char *extension_jpg(char *chemin) {

	char *dernier_point = strrchr(chemin,'.');
	int len = strlen(chemin);

	if (dernier_point == NULL) {

		char *chaine = (char *)malloc(len + 5);
		if (chaine == NULL) return NULL;

		snprintf(chaine ,len + 5, "%s.jpg", chemin);
		
		return chaine;
	}
	
	int len_avant_point = (int)(dernier_point - chemin);
	char *chaine = (char *)malloc(len_avant_point + 5);
	if (chaine == NULL) return NULL;

	memcpy(chaine, chemin, len_avant_point);
	memcpy(chaine + len_avant_point, ".jpg", 5);

	return chaine;
}

char *dupliquer_chaine(char *chaine) {

	char *nouvelle_chaine = malloc(strlen(chaine) + 1);
	
	if (nouvelle_chaine == NULL) return NULL;

	strcpy(nouvelle_chaine, chaine);

	return nouvelle_chaine;
}

bool initialiser_parametres_commande(int argc, char **argv, struct parametres_commande_t *parametres) {

	bool facteurs_initialises;
	bool res = recuperer_parametres_commande(argc, argv, parametres, &facteurs_initialises);

	if (!res) {
		parametres->chemin_sortie = NULL;
		return false;
	}
	if (parametres->help) {
		parametres->chemin_sortie = NULL;
		return true;
	}
	if (parametres->chemin_entree == NULL) {
		parametres->chemin_sortie = NULL;
		return false;
	}

	char *chemin_sortie = (parametres->chemin_sortie == NULL) ? extension_jpg(parametres->chemin_entree) : dupliquer_chaine(parametres->chemin_sortie);

	if (chemin_sortie == NULL) return false;

	parametres->chemin_sortie = chemin_sortie;
	
	
	if (!facteurs_initialises) {

		struct facteurs_echantillonnage_t facteurs = {2, 2, 1, 1, 1, 1};
		parametres->facteurs = facteurs;
	
	} else {
		
		if (!verifier_facteurs_echantillonnage(parametres->facteurs)) return false; 
	}

	return true;
}

bool help_demande(struct parametres_commande_t *parametres) {

	if (!parametres->help) return false;

	printf("Usage : ./ppm2jpeg <chemin du fichier en entrée> <options>\n");
	printf("--help : affiche cette page d'aide\n");
	printf("--outfile <chemin de sortie> : spécifie le chemin de sortie du fichier JPEG\n");
	printf("--sample <AxB,CxD,ExF>: permet de définir avec les variables A,B,C,D,E,F les facteurs d'échantillonnage des composantes Y, Cb et Cr\n");

	return true;
}

void liberer_parametres_commande(struct parametres_commande_t *parametres) {

	if (parametres->chemin_sortie == NULL) return;

	free(parametres->chemin_sortie);
	parametres->chemin_sortie = NULL;
}



bool verifier_facteurs_echantillonnage(struct facteurs_echantillonnage_t facteurs) {

	if (
		facteurs.h1 < 1 || facteurs.h1 > 4 || 
		facteurs.v1 < 1 || facteurs.v1 > 4 || 
		facteurs.h2 < 1 || facteurs.h2 > 4 ||
		facteurs.v2 < 1 || facteurs.v2 > 4 || 
		facteurs.h3 < 1 || facteurs.h3 > 4 ||
		facteurs.v3 < 1 || facteurs.v3 > 4
	) return false;

	if ((facteurs.h1 * facteurs.v1 + facteurs.h2 * facteurs.v2 + facteurs.h3 * facteurs.v3) > 10) return false;

	if (
		facteurs.h1 % facteurs.h2 != 0 || 
		facteurs.h1 % facteurs.h3 != 0 ||
		facteurs.v1 % facteurs.v2 != 0 || 
		facteurs.v1 % facteurs.v3 != 0
	) return false;

	return true;
}
