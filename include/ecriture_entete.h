#ifndef _ECRITURE_ENTETE_H_
#define _ECRITURE_ENTETE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "commande.h"
#include "qtables.h"
#include "htables.h"



bool ecrire_entete(
    FILE *f, uint16_t hauteur_image, uint16_t largeur_image, bool couleur, Facteurs_echantillonnage facteurs,
    const uint8_t *table_q_y, const uint8_t *table_q_cbcr,
    const uint8_t longueurs_huffman[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS][16],
    const uint8_t *table_symboles_huffman[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS],
    const uint8_t taille_table_huffman[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS]
);



#endif /* _ECRITURE_ENTETE_H_ */
