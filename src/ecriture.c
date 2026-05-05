#include "ecriture.h"
#include <math.h>



FILE *ouvrir_fichier_sortie(char *chemin_sortie, Buffer_ecriture *buffer) {

    FILE *fichier = fopen(chemin_sortie, "wb");
    if (fichier == NULL) return NULL;

    buffer->taille_bits = 0;
    buffer->taille_octets = 0;

    ajouter_marqueur(MARQUEUR_SOI_DEBUT_IMAGE, fichier, buffer);

    return fichier;
}

/*
void ecrire_entete(uint16_t largeur_image, uint16_t hauteur_image, bool niveaux_gris, Buffer_ecriture *buffer) {

}
*/


void ecrire_donnees(FILE *fichier, Buffer_ecriture *buffer) {

    fwrite(buffer->buffer_octets, sizeof(uint8_t), buffer->taille_octets, fichier);

    buffer->taille_octets = 0;
}

uint8_t masquer_derniers_bits(uint8_t octet, uint8_t nb_bits) {

    if (nb_bits == 8) return octet;
    if (nb_bits == 0) return 0;

    return octet & (0xFF << (8 - nb_bits));
}

void concatener_bits(uint8_t bits_1, uint8_t longueur_1, uint8_t bits_2, uint8_t longueur_2, uint8_t *octet_1, uint8_t *octet_2) {

    bits_1 = masquer_derniers_bits(bits_1, longueur_1);
    bits_2 = masquer_derniers_bits(bits_2, longueur_2);

    if (longueur_1 == 8) {
        *octet_1 = bits_1;
        *octet_2 = bits_2;
        return;
    }

    *octet_1 = bits_1 | (bits_2 >> longueur_1);

    if ((longueur_1 + longueur_2) < 8) {
        *octet_2 = 0;
        return;
    }

    *octet_2 = bits_2 << (8 - longueur_1);
}

void ecrire_octet_buffer(uint8_t octet_a_ecrire, bool stuffing, FILE *fichier, Buffer_ecriture *buffer) {

    buffer->buffer_octets[buffer->taille_octets] = octet_a_ecrire;
    buffer->taille_octets += 1;

    if (stuffing && octet_a_ecrire == OCTET_BYTE_STUFFING) {
        buffer->buffer_octets[buffer->taille_octets] = OCTET_AJOUT_BYTE_STUFFING;
        buffer->taille_octets += 1;
    }

    if (buffer->taille_octets >= LIMITE_ECRITURE) {
        ecrire_donnees(fichier, buffer);
    }
}

void ajouter_octet(uint8_t octet, bool stuffing, FILE *fichier, Buffer_ecriture *buffer) {

    uint8_t octet_a_ecrire;

    if (buffer->taille_bits == 0) {
        octet_a_ecrire = octet;

    } else {
        //uint8_t deccalage = 8 - buffer->taille_bits;
        //Decoupage_2_octets decoupage;
        //decoupage.deux_octets = masquer_derniers_bits(octet, buffer->taille_bits) + ((uint16_t)octet << deccalage);
        
        concatener_bits(buffer->buffer_bits, buffer->taille_bits, octet, 8, &octet_a_ecrire, &(buffer->buffer_bits));
        //octet_a_ecrire = decoupage.un_octet.msb;
        //buffer->buffer_bits = decoupage.un_octet.lsb;
    }

    ecrire_octet_buffer(octet_a_ecrire, stuffing, fichier, buffer);
}

void ajouter_octets(uint8_t *octets, uint32_t taille_octets, FILE *fichier, Buffer_ecriture *buffer) {

    for (uint32_t i = 0; i < taille_octets; i++) {

        ajouter_octet(octets[i], true, fichier, buffer);
    }
}

void ajouter_marqueur(uint8_t code_marqueur, FILE *fichier, Buffer_ecriture *buffer) {

    ajouter_octet(OCTET_DEBUT_MARQUEUR, false, fichier, buffer);
    ajouter_octet(code_marqueur, false, fichier, buffer);
}

void ajouter_moins_8_bits(uint8_t bits, uint8_t taille_bits, FILE *fichier, Buffer_ecriture *buffer) {

    uint8_t longueur_bits_totale = buffer->taille_bits + taille_bits;
    //uint8_t deccalage = 8 - buffer->taille_bits;

    //Decoupage_2_octets decoupage;
    //decoupage.deux_octets = ((uint16_t)masquer_derniers_bits(bits, taille_bits)) << deccalage;
    uint8_t octet_1, octet_2;

    concatener_bits(buffer->buffer_bits, buffer->taille_bits, bits, taille_bits, &octet_1, &octet_2);

    if (longueur_bits_totale < 8) {

        buffer->buffer_bits = octet_1;
        buffer->taille_bits = longueur_bits_totale;

    } else {    

        ecrire_octet_buffer(octet_1, true, fichier, buffer);

        if (longueur_bits_totale == 8) {
            buffer->taille_bits = 0;
            
        } else {

            buffer->buffer_bits = octet_2;
            buffer->taille_bits = (longueur_bits_totale - 8);
        }
    }
}

void ajouter_bits(uint8_t *bits, uint8_t taille_bits, FILE *fichier, Buffer_ecriture *buffer) {

    ajouter_octets(bits, taille_bits / 8, fichier, buffer);
    uint8_t bits_restants = taille_bits % 8;

    if (bits_restants != 0) {
        
        ajouter_moins_8_bits(bits[taille_bits / 8], bits_restants, fichier, buffer);   
    }
}

void completer_derniers_bits(FILE *fichier, Buffer_ecriture *buffer) {

    if (buffer->taille_bits == 0) return;

    ecrire_octet_buffer(masquer_derniers_bits(buffer->buffer_bits, buffer->taille_bits), true, fichier, buffer);

    buffer->taille_bits = 0;
}




void fermer_fichier_sortie(FILE *fichier, Buffer_ecriture *buffer) {

    completer_derniers_bits(fichier, buffer);
    ajouter_marqueur(MARQUEUR_EOI_FIN_IMAGE, fichier, buffer);

    ecrire_donnees(fichier, buffer);

    fclose(fichier);
}