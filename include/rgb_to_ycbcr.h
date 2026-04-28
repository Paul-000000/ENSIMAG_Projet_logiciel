#ifndef _RGB_TO_YCBCR_H_
#define _RGB_TO_YCBCR_H_

#include <stdint.h>

typedef struct couleur_rgb_t {
	uint8_t r,g,b;
}couleur_rgb_t;

typedef struct couleur_ycbcr_t {
	uint8_t y,cb,cr;
}couleur_ycbcr_t;

// convertion d'un pixel au format RGB en format YCbCr 
struct couleur_ycbcr_t rgb_to_ycbcr(struct couleur_rgb_t couleur);

#endif /* _RGB_TO_YCBCR_H_ */
