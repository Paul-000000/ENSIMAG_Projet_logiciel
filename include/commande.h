#ifndef _COMMANDE_H_
#define _COMMANDE_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct facteurs_echantillonnage {
	uint8_t h1,v1,h2,v2,h3,v3;
	
} Facteurs_echantillonnage;

typedef struct parametres_commande {
	char *chemin_entree; // le chemin du fichier en entrée non vérifié (peut être NULL)
	char *chemin_sortie; // le chemin de sortie si modifié sinon contient NULL
	bool help; // vrai si on a mis --help, on s'arrète, les autres champs ne sont pas forcément initialisés
	Facteurs_echantillonnage facteurs; // les facteurs récupérés

} Parametres_commande;


/*
récupère les valeurs des paramètres dans une ligne de commande

parametres est une structure non initialisée qui sera remplie à la fin de l'appel de fonction
*/

bool initialiser_parametres_commande(int argc, char **argv, Parametres_commande *parametres);

bool help_demande(Parametres_commande *parametres);

void liberer_parametres_commande(Parametres_commande *parametres);


bool verifier_facteurs_echantillonnage(Facteurs_echantillonnage facteurs);



#endif /* _COMMANDE_H_ */
