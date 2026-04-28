#ifndef _COMMANDE_H_
#define _COMMANDE_H_

#include <stdint.h>
#include <stdbool.h>

struct facteur_echantillonnage_t {
	uint8_t h,v;
};

struct parametres_commande_t {
	char *chemin_entree; // le chemin du fichier en entrée non vérifié (peut être NULL)
	char *chemin_sortie; // le chemin de sortie si modifié sinon contient NULL
	bool help; // si on a mis --help
	bool facteur_initialises; // est à true si le champ facteurs est initialisé
	struct facteur_echantillonnage_t facteurs[3]; // les facteurs récupérés
};


/*
récupère les valeurs des paramètres dans une ligne de commande

parametres est une structure non initialisée qui sera remplie à la fin de l'appel de fonction
*/
bool recuperer_parametres_commande(int argc, char **argv, struct parametres_commande_t *parametres);



#endif /* _COMMANDE_H_ */
