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

void ecrire_donnees(FILE *fichier, Buffer_ecriture *buffer) {

    fwrite(buffer->buffer_octets, sizeof(uint8_t), buffer->taille_octets, fichier);

    buffer->taille_octets = 0;
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
        ecrire_octet_buffer(octet_a_ecrire, stuffing, fichier, buffer);

    } else {

        ajouter_bits((uint16_t)octet, 8, fichier, buffer);
    }
}

void ajouter_octets(uint8_t *octets, uint32_t taille_octets, FILE *fichier, Buffer_ecriture *buffer) {

    for (uint32_t i = 0; i < taille_octets; i++) {

        ajouter_octet(octets[i], true, fichier, buffer);
    }
}

void ajouter_marqueur(uint8_t code_marqueur, FILE *fichier, Buffer_ecriture *buffer) {

    completer_derniers_bits(fichier, buffer);
    ajouter_octet(OCTET_DEBUT_MARQUEUR, false, fichier, buffer);
    ajouter_octet(code_marqueur, false, fichier, buffer);
}

void vider_buffer_bits(FILE *fichier, Buffer_ecriture *buffer) {

    if (buffer->taille_bits < 8) return;

    for (uint8_t i = 0; i < (buffer->taille_bits / 8) ; i++) {

        uint8_t octet = (uint8_t)buffer->buffer_bits;
        buffer->buffer_bits = buffer->buffer_bits >> 8;
        buffer->taille_bits -= 8;

        ecrire_octet_buffer(octet, true, fichier, buffer);
    }
}

void ajouter_bits(uint16_t bits, uint8_t taille_bits, FILE *fichier, Buffer_ecriture *buffer) {

    buffer->buffer_bits = (buffer->buffer_bits << taille_bits) | bits;
    buffer->taille_bits += taille_bits;

    vider_buffer_bits(fichier, buffer);
}

void completer_derniers_bits(FILE *fichier, Buffer_ecriture *buffer) {

    if (buffer->taille_bits == 0) return;

    uint8_t complement = (1 << (8 - buffer->taille_bits)) - 1;

    ecrire_octet_buffer(complement | buffer->buffer_bits, true, fichier, buffer); // on ajoute des 1

    buffer->taille_bits = 0;
}

void fermer_fichier_sortie(FILE *fichier, Buffer_ecriture *buffer) {

    ajouter_marqueur(MARQUEUR_EOI_FIN_IMAGE, fichier, buffer);

    ecrire_donnees(fichier, buffer);

    fclose(fichier);
}


void ajouter_donnees_compressees(AC_DC *coefficients_ac_dc, FILE *fichier, Buffer_ecriture *buffer) {

    ajouter_bits(coefficients_ac_dc->DC.code, coefficients_ac_dc->DC.nb_bits, fichier, buffer);
    ajouter_bits(coefficients_ac_dc->DC.indice, coefficients_ac_dc->DC.classe_mag, fichier, buffer);

    for (uint8_t i = 0; i < coefficients_ac_dc->taille; i++) {

        ajouter_bits(coefficients_ac_dc->AC[i].code, coefficients_ac_dc->AC[i].nb_bits, fichier, buffer);
        ajouter_bits(coefficients_ac_dc->AC[i].indice, coefficients_ac_dc->AC[i].classe_mag, fichier, buffer);
    }
}