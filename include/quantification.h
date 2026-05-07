#ifndef _QUANTIFICATION_H_
#define _QUANTIFICATION_H_

#include "downsampler.h"
#include <stdint.h>



// divise le vecteur par certains coefficiients selon sa composante
void quantification(int16_t bloc[64], Composante composante);



#endif /* _QUANTIFICATION_H_ */
