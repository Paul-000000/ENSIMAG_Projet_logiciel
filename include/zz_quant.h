#ifndef _ZZ_H_
#define _ZZ_H_

#include "downsampler.h"
#include <stdint.h>



// réordonne le vecteur sous forme zigzag
void zigzag (int16_t bloc[64]);

// divise le vecteur par certains coefficiients selon sa composante
void quantification (int16_t bloc[64], Composante composante);



#endif /* _ZZ_H_ */
