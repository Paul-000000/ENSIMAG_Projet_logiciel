
#include "commande.h"
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>

// récupère les valeurs des paramètres dans une ligne de commande
bool recuperer_parametres_commande(int argc, char **argv, struct parametres_commande_t *parametres) {

	if (argc < 2) return false;

	parametres->chemin_entree = NULL;
	parametres->chemin_sortie = NULL;
	parametres->help = false;
	parametres->facteur_initialises = false;

	int opt;

	static struct option options[] = {
        {"outfile", required_argument, NULL, 0},
        {"sample", 	required_argument, NULL, 1},
		{"help", 	no_argument,       NULL, 2},
        {0, 0, 0, 0}
    };
	
	while ((opt = getopt_long(argc, argv, "", options, NULL)) != -1) {
		
		switch (opt) {
			
			case 0: // outfile
				parametres->chemin_sortie = optarg;
				break;

			case 1: // sample

				if (optarg == NULL) return false;
				
				int args_corrects = sscanf(
					optarg,
					"h%hhuxv%hhu,h%hhuxv%hhu,h%hhuxv%hhu",
					&(parametres->facteurs[0].h),
					&(parametres->facteurs[0].v),
					&(parametres->facteurs[1].h),
					&(parametres->facteurs[1].v),
					&(parametres->facteurs[2].h),
					&(parametres->facteurs[2].v)
				);
				
				if (args_corrects != 6) return false;

				parametres->facteur_initialises = true;
				break;

			case 2: // help
				parametres->help = true;
				break;
			
			default:
				break;
			}
	}
	
	if (optind >= argc) return false;

	parametres->chemin_entree = argv[optind];

	return true;
}



