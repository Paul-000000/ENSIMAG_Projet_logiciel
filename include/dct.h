#ifndef _DCT_H_
#define _DCT_H_

#include <stdint.h>

// Un prototype pour une DCT qui travaille en place
// A jeter selon vos besoins/choix
void dct_naive(int16_t *block);
void init_table_cosinus();
void applique_dct(const int16_t  bloc_spatial[64], int16_t bloc_sequen [64]);
#endif /* _DCT_H_ */
