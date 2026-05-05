#include "ecriture.h"
#include <math.h>



#define OCTET_BYTE_STUFFING (uint8_t)0xff
#define OCTET_AJOUT_BYTE_STUFFING (uint8_t)0x00

#define OCTET_DEBUT_MARQUEUR (uint8_t)0xff

#define MARQUEUR_SOI_DEBUT_IMAGE (uint8_t)0xd8
#define MARQUEUR_EOI_FIN_IMAGE (uint8_t)0xd9



FILE *ouvrir_fichier_sortie(char *chemin_sortie, Buffer_ecriture *buffer) {

    FILE *fichier = fopen(chemin_sortie, "wb");
    if (fichier == NULL) return NULL;

    buffer->taille_bits = 0;
    buffer->taille_octets = 0;
    
    uint8_t debut[2] = {OCTET_DEBUT_MARQUEUR, MARQUEUR_SOI_DEBUT_IMAGE};
    ajouter_octets((uint8_t *)debut, 2, fichier, buffer);

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

uint8_t masquer_derniers_bits(uint8_t octet, uint8_t n) {

    return octet & ~((1 << n) - 1);
}

void ecrire_octet_buffer(uint8_t octet_a_ecrire, FILE *fichier, Buffer_ecriture *buffer) {

    buffer->buffer_octets[buffer->taille_octets] = octet_a_ecrire;
    buffer->taille_octets += 1;

    if (octet_a_ecrire == OCTET_BYTE_STUFFING) {
        buffer->buffer_octets[buffer->taille_octets] = OCTET_AJOUT_BYTE_STUFFING;
        buffer->taille_octets += 1;
    }

    if (buffer->taille_octets >= LIMITE_ECRITURE) {
        ecrire_donnees(fichier, buffer);
    }
}

void ajouter_octet(uint8_t octet, FILE *fichier, Buffer_ecriture *buffer) {

    uint8_t octet_a_ecrire;

    if (buffer->taille_bits == 0) {
        octet_a_ecrire = octet;

    } else {
        uint8_t deccalage = 8 - buffer->taille_bits;
        Decoupage_2_octets decoupage;
        decoupage.deux_octets = masquer_derniers_bits(octet, deccalage) + ((uint16_t)octet << deccalage);
        
        octet_a_ecrire = decoupage.un_octet[0];
        buffer->buffer_bits = decoupage.un_octet[1];
    }

    ecrire_octet_buffer(octet_a_ecrire, fichier, buffer);
}

void ajouter_octets(uint8_t *octets, uint32_t taille_octets, FILE *fichier, Buffer_ecriture *buffer) {

    for (uint32_t i = 0; i < taille_octets; i++) {

        ajouter_octet(octets[i], fichier, buffer);
    }
}

void ajouter_moins_8_bits(uint8_t bits, uint8_t taille_bits, FILE *fichier, Buffer_ecriture *buffer) {

    uint8_t longueur_bits_totale = buffer->taille_bits + taille_bits;
    uint8_t deccalage = 8 - buffer->taille_bits;

    Decoupage_2_octets decoupage;
    decoupage.deux_octets = ((uint16_t)masquer_derniers_bits(bits, 8 - taille_bits)) << deccalage;

    if (longueur_bits_totale < 8) {

        buffer->buffer_bits = decoupage.un_octet[0];
        buffer->taille_bits = longueur_bits_totale;

    } else {    

        ecrire_octet_buffer(decoupage.un_octet[0], fichier, buffer);

        if (longueur_bits_totale == 8) {
            buffer->taille_bits = 0;
            
        } else {

            buffer->buffer_bits = decoupage.un_octet[1];
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
    
    ecrire_octet_buffer(masquer_derniers_bits(buffer->buffer_bits, 8 - buffer->taille_bits), fichier, buffer);

    buffer->taille_bits = 0;
}


/*
void ajouter_donnees(uint8_t *donnees, uint32_t taille_donnees_bits, FILE *fichier, Buffer_ecriture *buffer) {

    // mettre les bits inutiles du dernier octet à 0
    if (taille_donnees_bits % 8 != 0) {

        uint8_t deccalage = 8 - taille_donnees_bits % 8;
        donnees[taille_donnees_bits / 8] = donnees[taille_donnees_bits / 8] & ~((1 << deccalage) - 1);
    }


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
            buffer->buffer[index_dernier_element_buffer + j] = donnees[taille_donnees_bits / 8];
        }
        
    
    } else {

        uint8_t reste_bits = buffer->taille_bits % 8;
        uint8_t deccalage = 8 - reste_bits;
        
        uint32_t j = 0;
        Decoupage_2_octets decoupage;

        for (uint32_t i = 0; i < (taille_donnees_bits / 8); i++) {

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
        
        if (taille_donnees_bits % 8 != 0) {

            decoupage.deux_octets = donnees[index_dernier_element_buffer + j] << deccalage;
            uint8_t octet = buffer->buffer[index_dernier_element_buffer + j] + decoupage.un_octet[0]; // poids fort
            buffer->buffer[index_dernier_element_buffer + j] = octet;
            j++;

            if (octet == OCTET_BYTE_STUFFING) { // byte stuffing
                buffer->buffer[index_dernier_element_buffer + j] = OCTET_AJOUT_BYTE_STUFFING;
                j++;
            }

            if ((reste_bits + (taille_donnees_bits % 8)) > 8) {
                buffer->buffer[index_dernier_element_buffer + j] = decoupage.un_octet[1]; // poids faible 
            }
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
*/


void fermer_fichier_sortie(FILE *fichier, Buffer_ecriture *buffer) {

    uint8_t fin[2] = {OCTET_DEBUT_MARQUEUR, MARQUEUR_EOI_FIN_IMAGE};
    
    completer_derniers_bits(fichier, buffer);
    ajouter_octets((uint8_t *)fin, 2, fichier, buffer);
    ecrire_donnees(fichier, buffer);

    fclose(fichier);
}