#include <stdio.h>
#include "dct.h"
#include <math.h>

# define M_PI		3.14159265358979323846	/* pi */



float tab_cos[8][8];

void init_table_cosinus() {

    for (int x=0;x<8; x++) {
        for (int i=0;i<8;i++) {
            tab_cos[x][i] = cos(((2.0*x+1.0)*i*M_PI)/16.0);
        }
    }
}

void applique_dct(const uint8_t bloc_spatial[64], int16_t bloc_frequentiel[64]){

    float somme;
    float raci = 1.0/sqrt(2.0);
    float c_i,c_j;

    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){

            somme = 0.0;

            for (int x=0;x<8;x++){

                float cos_x = tab_cos[x][i];

                for(int y=0;y<8;y++){

                    int valeur_decal = (int16_t)(bloc_spatial[x*8 + y]) - 128;

                    float cos_y = tab_cos[y][j];
                    somme += valeur_decal*cos_x*cos_y;
                }
            }
            c_i = (i == 0) ? raci : 1.0;
            c_j = (j == 0) ? raci : 1.0;

            float freq = 0.25*c_i*c_j*somme;
            bloc_frequentiel[i*8 + j] = (int16_t)(freq);


        }
    }
}

/*void applique_dct(const int16_t bloc_spatial[64], int16_t bloc_frequentiel[64]){
    
    int16_t temp[8][8];
    float somme_x, somme_y;
    float raci = 1.0/sqrt(2.0);
    float c_i,c_j;

    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){

            somme = 0.0;

            for (int x=0;x<8;x++){

                float cos_x = tab_cos[x][i];

                for(int y=0;y<8;y++){

                    int valeur_decal = bloc_spatial[x*8 + y] - 128;

                    float cos_y = tab_cos[y][j];
                    somme += valeur_decal*cos_x*cos_y;
                }
            }
            c_i = (i == 0) ? raci : 1.0;
            c_j = (j == 0) ? raci : 1.0;

            float freq = 0.25*c_i*c_j*somme;
            bloc_frequentiel[i*8 + j] = (int16_t)(freq);


        }
    }
} */
