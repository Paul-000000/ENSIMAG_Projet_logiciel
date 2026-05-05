#include "ecriture.h"
#include <math.h>



#define OCTET_BYTE_STUFFING (uint8_t)0xff
#define OCTET_AJOUT_BYTE_STUFFING (uint8_t)0x00

#define OCTET_DEBUT_MARQUEUR (uint8_t)0xff

#define MARQUEUR_SOI_DEBUT_IMAGE (uint16_t)0xffd8
#define MARQUEUR_EOI_FIN_IMAGE (uint16_t)0xffd9



FILE *ouvrir_fichier_sortie(char *chemin_sortie, Buffer_ecriture *buffer) {

    FILE *fichier = fopen(chemin_sortie, "wb");
    if (fichier == NULL) return NULL;

    uint16_t octet_debut = MARQUEUR_SOI_DEBUT_IMAGE;
    ajouter_donnees((uint8_t *)&octet_debut, 16, fichier, buffer);

    return fichier;
}




void ecrire_entete(uint16_t largeur_image, uint16_t hauteur_image, bool niveaux_gris, Buffer_ecriture *buffer) {


}

void ajouter_donnees(uint8_t *donnees, uint32_t taille_donnees_bits, FILE *fichier, Buffer_ecriture *buffer) {

    uint32_t index_dernier_element_buffer = buffer->taille_bits / 8;

    if (buffer->taille_bits % 8 == 0) {

        uint32_t j = 0;
        for (uint32_t i = 0; i < (taille_donnees_bits / 8); i++) {

            buffer->buffer[index_dernier_element_buffer + j] = donnees[i];
            j++;

            if (donnees[i] == OCTET_BYTE_STUFFING) { // byte stuffing
                buffer->buffer[index_dernier_element_buffer + j] = OCTET_AJOUT_BYTE_STUFFING;
                j++;
            }
        }

        if (taille_donnees_bits % 8 != 0) {

            uint8_t deccalage = 8 - taille_donnees_bits % 8;
            uint8_t octet = donnees[taille_donnees_bits / 8] & ~((1 << deccalage) - 1); // ajoute les bits avec le reste de l'octet à 0
            buffer->buffer[index_dernier_element_buffer + j] = octet;
        }
        
    
    } else {

        uint8_t reste_bits = buffer->taille_bits % 8;
        uint8_t deccalage = 8 - reste_bits;
        uint32_t nb_ajouts_octet = (reste_bits + taille_donnees_bits) / 8;
        
        uint32_t j = 0;
        Decoupage_2_octets decoupage;

        for (uint32_t i = 0; i < nb_ajouts_octet; i++) {

            decoupage.deux_octets = donnees[i] << deccalage;
            uint8_t octet = buffer->buffer[index_dernier_element_buffer + j] + decoupage.un_octet[0]; // poids fort
            buffer->buffer[index_dernier_element_buffer + j] = octet;
            j++;

            if (octet == OCTET_BYTE_STUFFING) { // byte stuffing
                buffer->buffer[index_dernier_element_buffer + j] = OCTET_AJOUT_BYTE_STUFFING;
                j++;
            }

            buffer->buffer[index_dernier_element_buffer + j] = decoupage.un_octet[1]; // poids faible
        }
        
        if ((reste_bits + taille_donnees_bits) % 8 != 0) {

            uint8_t deccalage = 8 - taille_donnees_bits % 8;
            uint8_t octet = donnees[taille_donnees_bits / 8] & ~((1 << deccalage) - 1); // ajoute les bits avec le reste de l'octet à 0
            buffer->buffer[index_dernier_element_buffer + j] = octet;
        }
    }


    
    buffer->taille_bits += taille_donnees_bits;

    if (buffer->taille_bits / 8 >= LIMITE_ECRITURE) { // si on dépasse la limite d'écriture on écrit

        ecrire_donnees(fichier, buffer, buffer->taille_bits / 8);
        uint8_t reste_bits = buffer->taille_bits % 8;

        if (reste_bits != 0) {
            buffer->buffer[0] = buffer->buffer[buffer->taille_bits / 8]; // deccaler les bits au début si il en reste
        }

        buffer->taille_bits = reste_bits;
    } 
    
}

void ecrire_donnees(FILE *fichier, Buffer_ecriture *buffer, uint32_t nb_bits) {

    uint32_t nb_octets_ecrire = nb_octets_ecrire / 8 + (nb_octets_ecrire % 8 == 0 ? 0 : 1);

    fwrite(&(buffer->buffer), sizeof(uint8_t), nb_octets_ecrire, fichier);

    buffer->taille_bits -= nb_bits;
}

void fermer_fichier_sortie(FILE *fichier, Buffer_ecriture *buffer) {

    uint16_t octet_fin = MARQUEUR_EOI_FIN_IMAGE;
    uint8_t alignement = 0;
    uint8_t taille_bits_alignement = 8 - buffer->taille_bits % 8;

    ajouter_donnees(&alignement, taille_bits_alignement, fichier, buffer);
    ajouter_donnees((uint8_t *)&octet_fin, 16, fichier, buffer);
    ecrire_donnees(fichier, buffer, buffer->taille_bits);

    fclose(fichier);
}