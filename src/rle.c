#include "rle.h"
#include "magnitude.h"



void rle(int16_t bloc[64], Symboles_RLE *symboles_rle, Magnitude bloc_enc[64]) {

    symboles_rle->taille = 0;
    uint8_t indice_fin = 63;
    
    while (indice_fin > 0 && bloc[indice_fin] == 0) {
        indice_fin--;
    }

    uint8_t nb_zero = 0;
    for(uint8_t i = 1; i <= indice_fin; i++) {

        if (bloc[i] != 0) {
            symboles_rle->symboles[symboles_rle->taille].symbole = ((nb_zero << 4) + bloc_enc[i].class_mag);
            symboles_rle->symboles[symboles_rle->taille].indice = bloc_enc[i].indice;

            nb_zero = 0;
            symboles_rle->taille += 1;
        
        } else {
            nb_zero++;

            if (nb_zero == 16) {
                symboles_rle->symboles[symboles_rle->taille].symbole = ZRL;
                nb_zero = 0;
                symboles_rle->taille += 1;
            }
        }
    }

    if (indice_fin < 63) {// ajout des derniers 0

        symboles_rle->symboles[symboles_rle->taille].symbole = EOB;
        symboles_rle->taille += 1;
    }
}

void rle_magnitude(int16_t bloc[64],int16_t * dc_prec, Magnitude bloc_enc[64], Symboles_RLE *symboles_rle_ac) {

    codage_magnitude(bloc, dc_prec, bloc_enc);

    rle(bloc, symboles_rle_ac, bloc_enc);

}