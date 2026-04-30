

#include "downsampler.h"
#include "rgb_to_ycbcr.h"
#include "commande.h"



void determiner_facteurs_mcu(Facteurs_echantillonnage facteurs, uint8_t *largeur_mcu, uint8_t *hauteur_mcu) {

	*largeur_mcu = 8 * facteurs.h1;
	*hauteur_mcu = 8 * facteurs.v1;
}

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

uint8_t moyenne_micro_matrice(Couleur_ycbcr matrice[MCU_MAX][MCU_MAX], uint8_t hauteur, uint8_t largeur, uint8_t i, uint8_t j) {

	uint32_t somme = 0;

	for (uint8_t y = 0; y < hauteur; y++) {

		for (uint8_t x = 0; x < largeur; x++) {

			somme += matrice[i * hauteur + y][j * largeur + x].cb;
		}
	}

	return (uint8_t)round((double)somme / (largeur * hauteur));

}

void decouper_matrices(Couleur_ycbcr matrice[MCU_MAX][MCU_MAX], uint8_t largeur_mcu, uint8_t hauteur_mcu, Dimensions_cbcr dimensions_sortie,Vecteurs_ycbcr *vecteur_sortie) {

	Vecteurs_ycbcr vecteurs = { .nb_vecteurs = 0};



	// y
	uint8_t nb_blocs_hauteur = hauteur_mcu / 8;
	uint8_t nb_blocs_largeur = largeur_mcu / 8;


	for (uint8_t i = 0; i < nb_blocs_hauteur; i++) {
		for (uint8_t j = 0; j < nb_blocs_largeur; j++) {

			
			//matrices_sortie->matrice_y[i][j] = matrice[i][j].y;
 		}
	}


	// cb
	uint8_t largeur_micro_matrices_cb = largeur_mcu / dimensions_sortie.largeur_mcu_cb;
	uint8_t hauteur_micro_matrices_cb = hauteur_mcu / dimensions_sortie.hauteur_mcu_cb;

	for (uint8_t i = 0; i < dimensions_sortie.hauteur_mcu_cb; i++) {
		for (uint8_t j = 0; j < dimensions_sortie.largeur_mcu_cb; j++) {

			// moyenne
			//matrices_sortie->matrice_cb[i][j] =  moyenne_micro_matrice(matrice, hauteur_micro_matrices_cb, largeur_micro_matrices_cb, i, j);
		}
	}

	// cr
	uint8_t largeur_micro_matrices_cr = largeur_mcu / dimensions_sortie.largeur_mcu_cr;
	uint8_t hauteur_micro_matrices_cr = hauteur_mcu / dimensions_sortie.hauteur_mcu_cr;

	for (uint8_t i = 0; i < dimensions_sortie.hauteur_mcu_cr; i++) {
		for (uint8_t j = 0; j < dimensions_sortie.largeur_mcu_cr; j++) {

			// moyenne
			//matrices_sortie->matrice_cr[i][j] =  moyenne_micro_matrice(matrice, hauteur_micro_matrices_cr, largeur_micro_matrices_cr, i, j);
		}
	}

}

