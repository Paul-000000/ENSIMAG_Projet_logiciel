#ifndef _DCT_H_
#define _DCT_H_

#include <stdint.h>


// crée une table de cosinus pour ne pas les recalculer par la suite
void init_table_cosinus();

// transforme la matrice bloc_spatial en matrice sous forme fréquentielle
void applique_dct(const int16_t bloc_spatial[64], int16_t bloc_frequentiel[64]);



#endif /* _DCT_H_ */
