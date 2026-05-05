#ifndef _ECRITURE_H_
#define _ECRITURE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define TAILLE_BUFFER_ECRITURE 10000
#define LIMITE_ECRITURE 8192



typedef union decoupage_2_octets
{
	uint16_t deux_octets;
	uint8_t un_octet[2];

} Decoupage_2_octets;

typedef struct buffer_ecriture {
	uint8_t buffer[TAILLE_BUFFER_ECRITURE];
	uint32_t taille_bits;

} Buffer_ecriture;



FILE *ouvrir_fichier_sortie(char *chemin_sortie, Buffer_ecriture *buffer);

void ecrire_entete(uint16_t largeur_image, uint16_t hauteur_image, bool niveaux_gris, Buffer_ecriture *buffer);

void ajouter_donnees(uint8_t *donnees, uint32_t taille_donnees_bits, FILE *fichier, Buffer_ecriture *buffer);

void fermer_fichier_sortie(FILE *fichier, Buffer_ecriture *buffer);



#endif /* _ECRITURE_H_ */
