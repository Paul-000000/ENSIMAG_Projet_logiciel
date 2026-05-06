#ifndef _RLE_H_
#define _RLE_H_

#include <stdint.h>
#include "magnitude.h"

#define ZRL 0xF0
#define EOB 0x00



typedef struct symbole_RLE {
	uint8_t symbole; // 4 bits pour les coefficients nuls précédents, 4 bits pour la magnitude ou ZRL ou EOB
	uint8_t classe_mag;
	uint16_t indice; // m bits pour l'indice, avec 0 <= m <= 10

} Symbole_RLE; 

typedef struct symboles_RLE {
	Symbole_RLE symboles[63];
	uint8_t taille;

} Symboles_RLE;



void rle(int16_t bloc[64], Symboles_RLE *symboles_rle, Magnitude bloc_enc[64]);


#endif /* _RLE_H_ */
