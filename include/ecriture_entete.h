#ifndef _ECRITURE_ENTETE_H_
#define _ECRITURE_ENTETE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "commande.h"
#include "qtables.h"
#include "htables.h"

#define DEBUT_MARQUEUR 0xff
#define MARQUEUR_APP0 0xe0
#define MARQEUR_DQT 0xdb
#define MARQUEUR_SOF0 0xc0
#define MARQUEUR_DHT 0xc4
#define MARQUEUR_SOS 0xda
#define MARQUEUR_SOI 0xd8
#define MARQUEUR_EOI 0xd9

#define ID_QUANTIFICATION_Y 0
#define ID_QUANTIFICATION_CBCR 1

#define ID_COMPOSANTE_Y 1
#define ID_COMPOSANTE_CB 2
#define ID_COMPOSANTE_CR 3

#define ID_DC 0
#define ID_AC 1



bool ecrire_SOI(FILE *f);

bool ecrire_EOI(FILE *f);

bool ecrire_entete(
    FILE *f, uint16_t hauteur_image, uint16_t largeur_image, bool couleur, Facteurs_echantillonnage facteurs,
    const uint8_t *table_q_y, const uint8_t *table_q_cbcr,
    const uint8_t longueurs_huffman[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS][16],
    const uint8_t *table_symboles_huffman[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS],
    const uint8_t taille_table_huffman[NB_SAMPLE_TYPES][NB_COLOR_COMPONENTS]
);



#endif /* _ECRITURE_ENTETE_H_ */
