#ifndef _ZIGZAG_QUANTIFICATION_H_
#define _ZIGZAG_QUANTIFICATION_H_

#include <stdint.h>
#include "qtables.h"
#include "rgb_to_ycbcr.h"



static const uint16_t tab_zigzag [64] = {
     0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};



// divise le vecteur fréquentiel par certains coefficients selon sa composante
void applique_zigzag_quantification(const double vecteur_frequentiel[64], Composante composante, int16_t vecteur_sortie[64]);



#endif /* _ZIGZAG_QUANTIFICATION_H_ */
