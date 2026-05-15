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

inline Magnitude encoder_val(int16_t val){

    if (val==0)
    {
        return (Magnitude){0,0};
    }
    int16_t tmp=abs(val);
    uint8_t magnitude=0;
   
    Magnitude val_enc;

    while ( tmp > 0)
    { 
      magnitude++;
      tmp=tmp >> 1;
    }
    uint16_t puissance_2 = 1 << magnitude;
    val_enc.class_mag=magnitude;
    int16_t sup_int=puissance_2-1;
    int16_t masque_signe = val >> 15;
    val_enc.indice = val + (masque_signe & sup_int);
    

    return val_enc;

}