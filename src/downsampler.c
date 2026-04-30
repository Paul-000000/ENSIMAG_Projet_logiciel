

#include "downsampler.h"
#include "rgb_to_ycbcr.h"
#include "commande.h"



void determiner_facteurs_mcu(struct facteurs_echantillonnage_t facteurs, uint8_t *largeur_mcu, uint8_t *hauteur_mcu) {

	*largeur_mcu = 8 * facteurs.h1;
	*hauteur_mcu = 8 * facteurs.v1;
}

struct dimensions_cbcr_t determiner_dimensions_cb_cr(struct facteurs_echantillonnage_t facteurs) {

	uint8_t largeur_mcu = 8 * facteurs.h1;
	uint8_t hauteur_mcu = 8 * facteurs.v1;

	struct dimensions_cbcr_t dimensions = {
		largeur_mcu / (facteurs.h1 / facteurs.h2),
		hauteur_mcu / (facteurs.v1 / facteurs.v2),
		largeur_mcu / (facteurs.h1 / facteurs.h3),
		hauteur_mcu / (facteurs.v1 / facteurs.v3)
	};
	
	return dimensions;
}

void decouper_matrices(struct couleur_ycbcr_t matrice[MCU_MAX][MCU_MAX], uint8_t largeur_mcu, uint8_t hauteur_mcu, struct dimensions_cbcr_t dimensions_sortie, struct matrices_ycbcr_t *matrices_sortie) {

	struct vecteurs_ycbcr_t vecteurs = {};

	// y
	for (uint8_t i = 0; i < hauteur_mcu; i++) {
		for (uint8_t j = 0; j < largeur_mcu; j++) {

			matrices_sortie->matrice_y[i][j] = matrice[i][j].y;
 		}
	}

	// cb
	uint8_t largeur_micro_matrices_cb = largeur_mcu / dimensions_sortie.largeur_mcu_cb;
	uint8_t hauteur_micro_matrices_cb = hauteur_mcu / dimensions_sortie.hauteur_mcu_cb;

	for (int i = 0; i < dimensions_sortie.hauteur_mcu_cb; i++) {
		for (int j = 0; j < dimensions_sortie.largeur_mcu_cb; j++) {

			// moyenne
			uint32_t somme = 0;

			for (uint8_t y = 0; y < hauteur_micro_matrices_cb; y++) {

				for (uint8_t x = 0; x < largeur_micro_matrices_cb; x++) {

					somme += matrice[i * hauteur_micro_matrices_cb + y][j * largeur_micro_matrices_cb + x].cb;
				}
			}

			matrices_sortie->matrice_cb[i][j] = somme / (largeur_micro_matrices_cb * hauteur_micro_matrices_cb);
		}
	}

	// cr
	uint8_t largeur_micro_matrices_cr = largeur_mcu / dimensions_sortie.largeur_mcu_cr;
	uint8_t hauteur_micro_matrices_cr = hauteur_mcu / dimensions_sortie.hauteur_mcu_cr;

	for (uint8_t i = 0; i < dimensions_sortie.hauteur_mcu_cr; i++) {
		for (uint8_t j = 0; j < dimensions_sortie.largeur_mcu_cr; j++) {

			// moyenne
			uint32_t somme = 0;

			for (uint8_t y = 0; y < hauteur_micro_matrices_cr; y++) {

				for (uint8_t x = 0; x < largeur_micro_matrices_cr; x++) {

					somme += matrice[i * hauteur_micro_matrices_cr + y][j * largeur_micro_matrices_cr + x].cr;
				}
			}

			matrices_sortie->matrice_cr[i][j] = somme / (largeur_micro_matrices_cr * hauteur_micro_matrices_cr);
		}
	}

}

