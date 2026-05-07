

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "quantification.h"
#include "qtables.h"



void applique_quant(int16_t bloc[64], const uint8_t table_quantification[64]){

    for (int i=0; i<64;i++){

        float val = (float)bloc[i] / (float)table_quantification[i];

        bloc[i] = (int16_t)round(val);
    }
}

void quantification(int16_t bloc[64], Composante composante){

    if (composante == Y) {
        applique_quant (bloc, quantification_table_Y);
    
    } else {
        applique_quant (bloc, quantification_table_CbCr);
    }
}