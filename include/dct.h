#ifndef _DCT_H_
#define _DCT_H_

#include <stdint.h>
#include "qtables.h"
#include "rgb_to_ycbcr.h"



/*
DCT optimisée qui transforme la matrice bloc_spatial en matrice sous forme fréquentielle en O(n²) utilisant l'algorithme de DCT rapide type II en 1D
créé par Arai, Agui, Nakajima en 1988. https://web.stanford.edu/class/ee398a/handouts/lectures/07-TransformCoding.pdf#page=30
et du Projet Nayuki. https://www.nayuki.io/page/fast-discrete-cosine-transform-algorithms
*/

// divise le vecteur par certains coefficients selon sa composante


void dct_zigzag_quantification(const uint8_t bloc_spatial[64], Composante composante, int16_t bloc_frequentiel[64]);



#endif /* _DCT_H_ */
