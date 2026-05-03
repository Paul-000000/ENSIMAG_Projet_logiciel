#ifndef _ZZ_H_
#define _ZZ_H_

#include <stdint.h>

// Un prototype pour une ZZ qui travaille en place
// A jeter selon vos besoins/choix
typedef enum  {
    Luminence_Y,
    Cb,
    Cr,
}Typequant;

void appli_zigzag (int16_t bloc[64] );
void applique_quant (int16_t bloc[64], uint8_t table_quantification[64]);
void quantification (int16_t bloc[64], Typequant type, uint8_t tab_quantif[64]);

#endif /* _ZZ_H_ */
