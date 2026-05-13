#include <stdio.h>
#include "dct.h"
#include "fast-dct-8.h"
#include <math.h>
// #define M_PI		3.14159265358979323846	// pi


// table des cosinus précalculée pour la DCT naive et la DCT optimisée
/*
static double tab_cos[8][8] = {

    { 1.0, 0.98078528040323043,  0.92387953251128674,    0.83146961230254524,    0.70710678118654757,    0.55557023301960229,    0.38268343236508984,    0.19509032201612833    }, 
    { 1.0, 0.83146961230254524,  0.38268343236508984,    -0.19509032201612819,   -0.70710678118654746,   -0.98078528040323043,   -0.92387953251128685,   -0.55557023301960218   }, 
    { 1.0, 0.55557023301960229,  -0.38268343236508973,   -0.98078528040323043,   -0.70710678118654768,   0.1950903220161283,     0.92387953251128652,    0.83146961230254546    }, 
    { 1.0, 0.19509032201612833,  -0.92387953251128674,   -0.55557023301960218,   0.70710678118654735,    0.83146961230254546,    -0.38268343236508989,   -0.98078528040323065   }, 
    { 1.0, -0.19509032201612819, -0.92387953251128685,   0.55557023301960184,    0.70710678118654768,    -0.83146961230254512,   -0.38268343236509056,   0.98078528040323043    }, 
    { 1.0, -0.55557023301960196, -0.38268343236509034,   0.98078528040323043,    -0.70710678118654668,   -0.19509032201612803,   0.92387953251128674,    -0.83146961230254501   }, 
    { 1.0, -0.83146961230254535, 0.38268343236509,       0.19509032201612878,    -0.70710678118654713,   0.98078528040323065,    -0.92387953251128641,   0.55557023301960151    }, 
    { 1.0, -0.98078528040323043, 0.92387953251128652,    -0.83146961230254512,   0.70710678118654657,    -0.55557023301960151,   0.38268343236508956,    -0.19509032201612858   }
};
*/

/*
// fonction d'initialisation de la table des cosinus pour la DCT naive

void init_table_cosinus() {

    for (int x=0;x<8; x++) {
        for (int i=0;i<8;i++) {

            tab_cos[x][i] = cos(((2.0*x+1.0)*i*M_PI)/16.0);
        }
    }
}
*/

// DCT naive en O(n⁴)
/*void applique_dct(const int16_t bloc_spatial[64], int16_t bloc_frequentiel[64]){
    
    int16_t temp[8][8];
    double somme_x, somme_y;
    double raci = 1.0/sqrt(2.0);
    double c_i,c_j;

    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){

            somme = 0.0;

            for (int x=0;x<8;x++){

                double cos_x = tab_cos[x][i];

                for(int y=0;y<8;y++){

                    int valeur_decal = (int16_t)(bloc_spatial[x*8 + y]) - 128;

                    double cos_y = tab_cos[y][j];
                    somme += valeur_decal*cos_x*cos_y;
                }
            }
            c_i = (i == 0) ? raci : 1.0;
            c_j = (j == 0) ? raci : 1.0;

            double freq = 0.25*c_i*c_j*somme;
            bloc_frequentiel[i*8 + j] = (int16_t)round(freq);


        }
    }
} */

// DCT améliorée en O(n³)
/*void applique_dct(const uint8_t bloc_spatial[64], int16_t bloc_frequentiel[64]) {
    double temp[64];
    double raci = 1.0 / sqrt(2.0);


    for (int x = 0; x < 8; x++) {
        for (int j = 0; j < 8; j++) {
            double somme_y = 0.0;
            double c_j = (j == 0) ? raci : 1.0;
            for (int y = 0; y < 8; y++) {
                int valeur_decal = (int16_t)bloc_spatial[x * 8 + y] - 128;
                somme_y += valeur_decal * tab_cos[y][j];
            }
            temp[x * 8 + j] = somme_y * c_j;
        }
    }


    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            double somme_x = 0.0;
            double c_i = (i == 0) ? raci : 1.0;
            for (int x = 0; x < 8; x++) {
                somme_x += temp[x * 8 + j] * tab_cos[x][i];
            }
            // Le facteur 0.25 (1/4) vient de (1/2 * 1/2) des deux DCT-1D
            bloc_frequentiel[i * 8 + j] = (int16_t)(0.25 * c_i * somme_x);
        }
    }



}*/

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

void applique_dct(const uint8_t bloc_spatial[64], int16_t bloc_frequentiel[64]) {
    
    double vecteur_temp[64];

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

    // reporte frequentiel 
    for (int i = 0; i < 64; i++) {
        bloc_frequentiel[i] = (int16_t)round(vecteur_temp[i]);
    }
}