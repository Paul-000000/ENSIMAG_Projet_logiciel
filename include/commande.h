#ifndef _COMMANDE_H_
#define _COMMANDE_H_

#include <stdint.h>
#include <stdbool.h>

struct facteur_echantillonnage_t {
	uint8_t h1,v1,h2,v2,h3,v3;
};

struct parametres_commande_t {
	char *chemin_entree; // le chemin du fichier en entrée non vérifié (peut être NULL)
	char *chemin_sortie; // le chemin de sortie si modifié sinon contient NULL
	bool help; // vrai si on a mis --help, on s'arrète, les autres champs ne sont pas forcément initialisés
	struct facteur_echantillonnage_t facteurs; // les facteurs récupérés
};


/*
récupère les valeurs des paramètres dans une ligne de commande

parametres est une structure non initialisée qui sera remplie à la fin de l'appel de fonction
*/

bool initialiser_parametres_commande(int argc, char **argv, struct parametres_commande_t *parametres);

bool help_demande(struct parametres_commande_t *parametres);

void liberer_parametres_commande(struct parametres_commande_t *parametres);

#endif /* _COMMANDE_H_ */
