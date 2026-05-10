#ifndef _MAGNITUDE_H_
#define _MAGNITUDE_H_
#include <stdint.h>



typedef struct Magnitude // définition de la magnitude 
{
   uint16_t indice; // indice associé à la classe de magnitude
   uint8_t class_mag; // classe de magnitude

} Magnitude;



// encode les coefficients AC et le coefficient DC d'un bloc 8x8 sous forme de magnitude
void codage_magnitude(const int16_t bloc[64], int16_t * dc_prec, Magnitude bloc_enc[64]);

// transforme une valeur en magnitude
Magnitude encoder_val(int16_t val);



#endif 
