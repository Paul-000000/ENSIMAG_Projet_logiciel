#include "ecriture.h"
#include "ecriture_entete.h"
#include <math.h>



FILE *ouvrir_fichier_sortie(char *chemin_sortie, Buffer_ecriture *buffer) {

    FILE *fichier = fopen(chemin_sortie, "wb");
    if (fichier == NULL) return NULL;

    buffer->taille_bits = 0;
    buffer->taille_octets = 0;

    if (!ecrire_SOI(fichier)) return NULL;

    return fichier;
}

void ecrire_donnees(FILE *fichier, Buffer_ecriture *buffer) {

    fwrite(buffer->buffer_octets, sizeof(uint8_t), buffer->taille_octets, fichier);

    buffer->taille_octets = 0;
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

void vider_buffer_bits(FILE *fichier, Buffer_ecriture *buffer) {

    while (buffer->taille_bits >= 8) {

        uint8_t octet = (buffer->buffer_bits >> (buffer->taille_bits - 8));
        buffer->taille_bits -= 8;

        ecrire_octet_buffer(octet, fichier, buffer);
    }
}

void ajouter_bits(uint16_t bits, uint8_t taille_bits, FILE *fichier, Buffer_ecriture *buffer) {

    if (taille_bits == 0) return;

    buffer->buffer_bits = (buffer->buffer_bits << taille_bits) | bits;
    buffer->taille_bits += taille_bits;

    vider_buffer_bits(fichier, buffer);
}

void completer_derniers_bits(FILE *fichier, Buffer_ecriture *buffer) {
    
    if (buffer->taille_bits == 0) return;

    if ((buffer->taille_bits % 8 ) == 0) {
        
        vider_buffer_bits(fichier, buffer);
        return;
    }

    uint8_t taille_complement = 8 - (buffer->taille_bits % 8);
    uint16_t complement = (1 << taille_complement) - 1;
    ajouter_bits(complement, taille_complement, fichier, buffer);

    buffer->taille_bits = 0;
}

void fermer_fichier_sortie(FILE *fichier, Buffer_ecriture *buffer) {

    completer_derniers_bits(fichier, buffer);
    ecrire_donnees(fichier, buffer);
    ecrire_EOI(fichier);

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