#include <stdio.h>
#include "dct.h"
#include "fast-dct-8.h"
#include <math.h>


/*
DCT optimisée en O(n²) utilisant l'algorithme de DCT rapide type II en 1D
créé par Arai, Agui, Nakajima en 1988. https://web.stanford.edu/class/ee398a/handouts/lectures/07-TransformCoding.pdf#page=30
et du Projet Nayuki. https://www.nayuki.io/page/fast-discrete-cosine-transform-algorithms
*/

/* 
 * Fast discrete cosine transform algorithms (C)
 * 
 * Copyright (c) 2021 Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/fast-discrete-cosine-transform-algorithms
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */

uint16_t tab_zigzag [64] = {
     0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};

void dct_zigzag_quantification(const uint8_t bloc_spatial[64], Composante composante, int16_t bloc_frequentiel[64]) {

    double vecteur_temp[64];
    const uint8_t *table_quantification = (composante == Y) ? quantification_table_Y : quantification_table_CbCr;
    
    for (int i = 0; i < 64; i++) {
        vecteur_temp[i] = (double)bloc_spatial[i] - 128.0;
    }

    // applique ligne
    for (int i = 0; i < 8; i++) {
        FastDct8_transform_modifiee(&(vecteur_temp[i * 8]), 1);
    }

    // applique colonne
    for (int j = 0; j < 8; j++) {
        FastDct8_transform_modifiee(&(vecteur_temp[j]), 8);
    }

    // zigzag et quantification
    for (int x=0; x<64; x++){
        bloc_frequentiel[x] = (int16_t)round((vecteur_temp[tab_zigzag[x]]) / table_quantification[x]);
    }
}