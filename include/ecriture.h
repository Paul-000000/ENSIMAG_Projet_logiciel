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

#define OCTET_DEBUT_MARQUEUR (uint8_t)0xff

#define MARQUEUR_SOI_DEBUT_IMAGE (uint8_t)0xd8
#define MARQUEUR_EOI_FIN_IMAGE (uint8_t)0xd9



typedef struct buffer_ecriture {
	uint8_t buffer_octets[TAILLE_BUFFER_ECRITURE];
	uint32_t taille_octets;
	uint64_t buffer_bits;
	uint8_t taille_bits;

} Buffer_ecriture;



FILE *ouvrir_fichier_sortie(char *chemin_sortie, Buffer_ecriture *buffer);

void ajouter_octets(uint8_t *octets, uint32_t taille_octets, FILE *fichier, Buffer_ecriture *buffer);

void ajouter_bits(uint16_t bits, uint8_t taille_bits, FILE *fichier, Buffer_ecriture *buffer);

void ajouter_marqueur(uint8_t code_marqueur, FILE *fichier, Buffer_ecriture *buffer);

void completer_derniers_bits(FILE *fichier, Buffer_ecriture *buffer);

void fermer_fichier_sortie(FILE *fichier, Buffer_ecriture *buffer);

void ajouter_donnees_compressees(AC_DC *coefficients_ac_dc, FILE *fichier, Buffer_ecriture *buffer);



#endif /* _ECRITURE_H_ */
