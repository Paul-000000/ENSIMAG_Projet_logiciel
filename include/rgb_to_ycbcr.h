#ifndef _RGB_TO_YCBCR_H_
#define _RGB_TO_YCBCR_H_

#include <stdint.h>

typedef struct couleur_rgb_t {
	uint8_t r,g,b;
 } couleur_rgb_t;

typedef struct couleur_ycbcr_t {
	uint8_t y,cb,cr;
 } couleur_ycbcr_t;

#include "downsampler.h"

// convertion d'un pixel au format RGB en format YCbCr
struct couleur_ycbcr_t rgb_to_ycbcr(struct couleur_rgb_t couleur);

void matrice_rgb_to_ycbcr(struct couleur_rgb_t matrice[MCU_MAX][MCU_MAX], uint8_t largeur, uint8_t hauteur, struct couleur_ycbcr_t matrice_sortie[MCU_MAX][MCU_MAX]);


#endif /* _RGB_TO_YCBCR_H_ */
