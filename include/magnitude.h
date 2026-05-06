#ifndef _MAGNITUDE_H_
#define _MAGNITUDE_H_
#include <stdint.h>


typedef struct Magnitude
{
   uint16_t indice;
   uint8_t class_mag ;

} Magnitude;


void codage_magnitude(int16_t bloc[64], int16_t * dc_prec, Magnitude bloc_enc[64]);

Magnitude encoder_val(int16_t val);



#endif 
