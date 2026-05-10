#include <magnitude.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void codage_magnitude(const int16_t bloc[64], int16_t *dc_prec, Magnitude bloc_enc[64]){

    bloc_enc[0]=encoder_val(bloc[0]- (*dc_prec));
    
    for (int i = 1; i < 64; i++)
    {
       bloc_enc[i]=encoder_val(bloc[i]);
        
    }
    *dc_prec=bloc[0];

}

Magnitude encoder_val(int16_t val){

    int16_t tmp=abs(val);
    uint8_t magnitude=0;
    uint16_t puissance_2=1;
    Magnitude val_enc;
    
    if (val==0)
    {
        return (Magnitude){0,0};
    }
    

    while ( tmp > 0)
    { 
      magnitude++;
      puissance_2*=2;
      tmp=tmp >> 1;
    }
    val_enc.class_mag=magnitude;
    uint16_t indice;
    int16_t sup_int=puissance_2-1;
    
    if (val<0)
    {
       indice=sup_int+val;
    }
    else
    {
     indice=val;
    }
    val_enc.indice= indice;

    return val_enc;

}