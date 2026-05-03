

#include <stdio.h>
#include "zz.h"
#include <string.h>
#include "qtables.h"
#include "dct.h"

// tableau des incdices des enchenement du zigzag
uint16_t tab_zz [64] = {
     0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};


void appli_zigzag (int16_t bloc[64] ){
    uint16_t bloc_temp[64];

    for (int x=0; x<64; x++){
        bloc_temp[x] = bloc[tab_zz[x]];
    }
    memcpy(bloc,bloc_temp,64*sizeof(uint16_t));
}


void applique_quant (int16_t bloc[64], uint8_t table_quantification[64]){

    for (int i=0; i<64;i++){

        float val = (float)bloc[i] / (float)table_quantification[i];

        bloc[i] = (int16_t)round(val);
    }
}

void quantification (int16_t bloc[64], Typequant type , uint8_t tab_quantif[64]){

    int16_t bloc_sq[64];

    init_table_cosinus();
    applique_dct(bloc,bloc_sq);

    if (type == Luminence_Y) {
        applique_quant ( bloc_sq, quantification_table_Y);
    }
    else {
        applique_quant ( bloc_sq, quantification_table_CbCr);
    }
    appli_zigzag ( bloc_sq);

}