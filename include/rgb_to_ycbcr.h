#ifndef _RGB_TO_YCBCR_H_
#define _RGB_TO_YCBCR_H_

#include <stdint.h>



typedef struct couleur_rgb { // les 3 composantes R, G et B des pixels au format ppm
	uint8_t r,g,b;

} Couleur_rgb;

typedef struct couleur_ycbcr { // les 3 composantes Y, Cb et Cr des pixels
	uint8_t y,cb,cr;
	
} Couleur_ycbcr;



#include "downsampler.h"



// convertion d'un pixel au format RGB en format YCbCr
Couleur_ycbcr rgb_to_ycbcr(Couleur_rgb couleur);

// convertit une matrice du format RGB au format YCbCr
void matrice_rgb_to_ycbcr(Couleur_rgb matrice[MCU_MAX][MCU_MAX], uint8_t largeur, uint8_t hauteur, Couleur_ycbcr matrice_sortie[MCU_MAX][MCU_MAX]);



#endif /* _RGB_TO_YCBCR_H_ */
