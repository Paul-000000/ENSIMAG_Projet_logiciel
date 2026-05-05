

#include "downsampler.h"
#include "rgb_to_ycbcr.h"
#include "commande.h"
#include <math.h>



Dimensions_cbcr determiner_dimensions_cb_cr(Facteurs_echantillonnage facteurs) {

	uint8_t largeur_mcu = 8 * facteurs.h1;
	uint8_t hauteur_mcu = 8 * facteurs.v1;

	Dimensions_cbcr dimensions = {
		largeur_mcu / (facteurs.h1 / facteurs.h2),
		hauteur_mcu / (facteurs.v1 / facteurs.v2),
		largeur_mcu / (facteurs.h1 / facteurs.h3),
		hauteur_mcu / (facteurs.v1 / facteurs.v3)
	};
	
	return dimensions;
}

uint8_t moyenne_micro_matrice(Couleur_ycbcr matrice[MCU_MAX][MCU_MAX], uint8_t hauteur, uint8_t largeur, uint8_t i, uint8_t j, bool cb) {

	uint32_t somme = 0;

	for (uint8_t y = 0; y < hauteur; y++) {

		for (uint8_t x = 0; x < largeur; x++) {

			Couleur_ycbcr couleur = matrice[i * hauteur + y][j * largeur + x];
			somme += cb ? couleur.cb : couleur.cr;
		}
	}

	return (uint8_t)round((double)somme / (largeur * hauteur));

}


void ajouter_vecteur(Vecteurs_ycbcr *vecteurs_sortie, uint8_t *nb_vecteurs, Composante composante, uint8_t hauteur, uint8_t largeur, uint8_t matrice_cb[hauteur][largeur], uint8_t i, uint8_t j) {

	Vecteur vec = {.composante = composante};

	for (uint8_t x = 0; x < 8; x++) {
		for (uint8_t y = 0; y < 8; y++) {
			vec.valeur[x * 8 + y] = matrice_cb[8 * i + x][8 * j + y];
		}
	}

	vecteurs_sortie->vecteurs[*nb_vecteurs] = vec;
	*nb_vecteurs += 1;
}

void decouper_matrices_couleur(Couleur_ycbcr matrice[MCU_MAX][MCU_MAX], uint8_t largeur_mcu, uint8_t hauteur_mcu, Dimensions_cbcr dimensions_sortie, Vecteurs_ycbcr *vecteurs_sortie) {

	uint8_t nb_vecteurs = 0;

	// y
	uint8_t nb_blocs_hauteur_y = hauteur_mcu / 8;
	uint8_t nb_blocs_largeur_y = largeur_mcu / 8;

	for (uint8_t i = 0; i < nb_blocs_hauteur_y; i++) {
		for (uint8_t j = 0; j < nb_blocs_largeur_y; j++) {

			Vecteur vec = {.composante = Y};
			
			for (uint8_t x = 0; x < 8; x++) {
				for (uint8_t y = 0; y < 8; y++) {
					vec.valeur[x * 8 + y] = matrice[8 * i + x][8 * j + y].y;
				}
			}

			vecteurs_sortie->vecteurs[nb_vecteurs] = vec;
			nb_vecteurs++;
 		}
	}


	// cb
	uint8_t matrice_cb[dimensions_sortie.hauteur_mcu_cb][dimensions_sortie.largeur_mcu_cb];

	uint8_t largeur_micro_matrices_cb = largeur_mcu / dimensions_sortie.largeur_mcu_cb;
	uint8_t hauteur_micro_matrices_cb = hauteur_mcu / dimensions_sortie.hauteur_mcu_cb;

	for (uint8_t i = 0; i < dimensions_sortie.hauteur_mcu_cb; i++) {
		for (uint8_t j = 0; j < dimensions_sortie.largeur_mcu_cb; j++) {

			matrice_cb[i][j] = moyenne_micro_matrice(matrice ,hauteur_micro_matrices_cb, largeur_micro_matrices_cb, i, j, true);
		}
	}

	uint8_t nb_blocs_largeur_cb = dimensions_sortie.largeur_mcu_cb / 8;
	uint8_t nb_blocs_hauteur_cb = dimensions_sortie.hauteur_mcu_cb / 8;

	for (uint8_t i = 0; i < nb_blocs_hauteur_cb; i++) {
		for (uint8_t j = 0; j < nb_blocs_largeur_cb; j++) {

			ajouter_vecteur(vecteurs_sortie, &nb_vecteurs, CB, dimensions_sortie.hauteur_mcu_cb, dimensions_sortie.largeur_mcu_cb, matrice_cb, i, j);
		}
	}


	// cr
	uint8_t matrice_cr[dimensions_sortie.hauteur_mcu_cr][dimensions_sortie.largeur_mcu_cr];

	uint8_t largeur_micro_matrices_cr = largeur_mcu / dimensions_sortie.largeur_mcu_cr;
	uint8_t hauteur_micro_matrices_cr = hauteur_mcu / dimensions_sortie.hauteur_mcu_cr;

	for (uint8_t i = 0; i < dimensions_sortie.hauteur_mcu_cr; i++) {
		for (uint8_t j = 0; j < dimensions_sortie.largeur_mcu_cr; j++) {

			matrice_cr[i][j] = moyenne_micro_matrice(matrice ,hauteur_micro_matrices_cr, largeur_micro_matrices_cr, i, j, false);
		}
	}

	uint8_t nb_blocs_largeur_cr = dimensions_sortie.largeur_mcu_cr / 8;
	uint8_t nb_blocs_hauteur_cr = dimensions_sortie.hauteur_mcu_cr / 8;

	for (uint8_t i = 0; i < nb_blocs_hauteur_cr; i++) {
		for (uint8_t j = 0; j < nb_blocs_largeur_cr; j++) {

			ajouter_vecteur(vecteurs_sortie, &nb_vecteurs, CR, dimensions_sortie.hauteur_mcu_cr, dimensions_sortie.largeur_mcu_cr, matrice_cr, i, j);
		}
	}


	vecteurs_sortie->nb_vecteurs = nb_vecteurs;
}

void decouper_matrice_gris(uint8_t matrice[8][8], Vecteur *vecteur) {

	vecteur->composante = Y;

	for (uint8_t i = 0; i < 8; i++) {
		for (uint8_t j = 0; j < 8; j++) {
			vecteur->valeur[i * 8 + j] = matrice[i][j];		
 		}
	}
}