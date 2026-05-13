#include <stdio.h>
#include <math.h>
#include "zigzag_quantification.h"



void applique_zigzag_quantification(const double vecteur_frequentiel[64], Composante composante, int16_t vecteur_sortie[64])
{

    const uint8_t *table_quantification = (composante == Y) ? quantification_table_Y : quantification_table_CbCr;

    for (int x = 0; x < 64; x++)
    {
        vecteur_sortie[x] = (int16_t)round((vecteur_frequentiel[tab_zigzag[x]]) / table_quantification[x]);
    }
}