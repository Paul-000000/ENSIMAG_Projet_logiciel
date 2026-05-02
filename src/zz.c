

#include <stdio.h>
#include "zz.h"
#include <string.h>



// tableau des incdices des enchaînements du zigzag
uint8_t tab_zz [64] = {
     0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};

void appli_zigzag (uint8_t bloc[64]){
    
    uint8_t bloc_temp[64];

    for (int x=0; x<64; x++){
        bloc_temp[x] = bloc[tab_zz[x]];
    }

    memcpy(bloc,bloc_temp,64*sizeof(uint8_t));
}
