#ifndef _ECRITURE_H_
#define _ECRITURE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define TAILLE_BUFFER_ECRITURE 10000
#define LIMITE_ECRITURE 8192

#define OCTET_BYTE_STUFFING (uint8_t)0xff
#define OCTET_AJOUT_BYTE_STUFFING (uint8_t)0x00

#define OCTET_DEBUT_MARQUEUR (uint8_t)0xff

#define MARQUEUR_SOI_DEBUT_IMAGE (uint8_t)0xd8
#define MARQUEUR_EOI_FIN_IMAGE (uint8_t)0xd9



typedef struct octets_successifs
{
	uint8_t msb;
	uint8_t lsb;

} Octets_successifs;

typedef union decoupage_2_octets
{
	uint16_t deux_octets;
	Octets_successifs un_octet;

} Decoupage_2_octets;

typedef struct buffer_ecriture {
	uint8_t buffer_octets[TAILLE_BUFFER_ECRITURE];
	uint32_t taille_octets;
	uint8_t buffer_bits;
	uint8_t taille_bits;

} Buffer_ecriture;



FILE *ouvrir_fichier_sortie(char *chemin_sortie, Buffer_ecriture *buffer);

//void ecrire_entete(uint16_t largeur_image, uint16_t hauteur_image, bool niveaux_gris, Buffer_ecriture *buffer);

void ajouter_octets(uint8_t *octets, uint32_t taille_octets, FILE *fichier, Buffer_ecriture *buffer);

void ajouter_bits(uint8_t *bits, uint8_t taille_bits, FILE *fichier, Buffer_ecriture *buffer);

void ajouter_marqueur(uint8_t code_marqueur, FILE *fichier, Buffer_ecriture *buffer);

void completer_derniers_bits(FILE *fichier, Buffer_ecriture *buffer);

void fermer_fichier_sortie(FILE *fichier, Buffer_ecriture *buffer);



#endif /* _ECRITURE_H_ */
