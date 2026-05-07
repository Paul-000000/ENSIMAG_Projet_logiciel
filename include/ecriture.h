#ifndef _ECRITURE_H_
#define _ECRITURE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "huffman.h"

#define TAILLE_BUFFER_ECRITURE 10000
#define LIMITE_ECRITURE 8192

#define OCTET_BYTE_STUFFING (uint8_t)0xff
#define OCTET_AJOUT_BYTE_STUFFING (uint8_t)0x00



typedef struct buffer_octets {
	uint8_t buffer[TAILLE_BUFFER_ECRITURE];
	uint32_t taille;

} Buffer_Octets;

typedef struct buffer_bits {
	uint64_t buffer;
	uint8_t taille;

} Buffer_Bits;

typedef struct flux_ecriture {
	Buffer_Octets buffer_octets;
	Buffer_Bits buffer_bits;
	FILE *fichier_sortie;

} Flux_Ecriture;


bool ouvrir_fichier_sortie(char *chemin_sortie, Flux_Ecriture *flux);

void ajouter_bits(uint16_t bits, uint8_t taille_bits, Flux_Ecriture *flux);

void completer_derniers_bits(Flux_Ecriture *flux);

void fermer_fichier_sortie(Flux_Ecriture *flux);

void ajouter_donnees_compressees(AC_DC *coefficients_ac_dc, Flux_Ecriture *flux);



#endif /* _ECRITURE_H_ */
