#include "unity.h"
#include "ecriture.h"
#include <stdlib.h>
#include <stdbool.h>

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_ecriture_vide(void) {

    char *chemin_sortie = "test.jpeg";

    Buffer_ecriture buffer;
    FILE *fichier = ouvrir_fichier_sortie(chemin_sortie, &buffer);
    TEST_ASSERT_NOT_NULL(fichier);

    fermer_fichier_sortie(fichier, &buffer);
    
    fichier = fopen(chemin_sortie, "rb");
    TEST_ASSERT_NOT_NULL(fichier);

    uint8_t octets[4];
    int octets_lus = fread(octets, sizeof(uint8_t), 4, fichier);
    
    TEST_ASSERT_EQUAL_UINT8(4, octets_lus);
    TEST_ASSERT_EQUAL_UINT8(OCTET_DEBUT_MARQUEUR, octets[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_SOI_DEBUT_IMAGE, octets[1]);
    TEST_ASSERT_EQUAL_UINT8(OCTET_DEBUT_MARQUEUR, octets[2]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_EOI_FIN_IMAGE, octets[3]);

    remove(chemin_sortie);
}

void test_ecriture_octets_bits(void) {

    char *chemin_sortie = "test.jpeg";

    Buffer_ecriture buffer;
    FILE *fichier = ouvrir_fichier_sortie(chemin_sortie, &buffer);
    TEST_ASSERT_NOT_NULL(fichier);

    uint8_t octets_ecrire[6] = {0,1,2,3,4, OCTET_BYTE_STUFFING};
    ajouter_octets(octets_ecrire, 6, fichier, &buffer);

    uint8_t bits = OCTET_BYTE_STUFFING;
    ajouter_bits(&bits, 8, fichier, &buffer);

    uint8_t bits_2[2] = {0b10101010, 0b11000000};
    ajouter_bits(bits_2, 10, fichier, &buffer);
    ajouter_bits(bits_2, 4, fichier, &buffer);

    fermer_fichier_sortie(fichier, &buffer);
    


    fichier = fopen(chemin_sortie, "rb");
    TEST_ASSERT_NOT_NULL(fichier);

    uint8_t octets[15];
    int octets_lus = fread(octets, sizeof(uint8_t), 15, fichier);
    
    TEST_ASSERT_EQUAL_UINT8(15, octets_lus);
    TEST_ASSERT_EQUAL_UINT8(OCTET_DEBUT_MARQUEUR, octets[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_SOI_DEBUT_IMAGE, octets[1]);

    TEST_ASSERT_EQUAL_UINT8(0, octets[2]);
    TEST_ASSERT_EQUAL_UINT8(1, octets[3]);
    TEST_ASSERT_EQUAL_UINT8(2, octets[4]);
    TEST_ASSERT_EQUAL_UINT8(3, octets[5]);
    TEST_ASSERT_EQUAL_UINT8(4, octets[6]);
    TEST_ASSERT_EQUAL_UINT8(OCTET_BYTE_STUFFING, octets[7]);
    TEST_ASSERT_EQUAL_UINT8(OCTET_AJOUT_BYTE_STUFFING, octets[8]);
    TEST_ASSERT_EQUAL_UINT8(OCTET_BYTE_STUFFING, octets[9]);
    TEST_ASSERT_EQUAL_UINT8(OCTET_AJOUT_BYTE_STUFFING, octets[10]);
    TEST_ASSERT_EQUAL_UINT8(0b10101010, octets[11]);
    TEST_ASSERT_EQUAL_UINT8(0b11101011, octets[12]);
    
    TEST_ASSERT_EQUAL_UINT8(OCTET_DEBUT_MARQUEUR, octets[13]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_EOI_FIN_IMAGE, octets[14]);

    remove(chemin_sortie);
}

void test_ecriture_deccalage(void) {

    char *chemin_sortie = "test.jpeg";

    Buffer_ecriture buffer;
    FILE *fichier = ouvrir_fichier_sortie(chemin_sortie, &buffer);
    TEST_ASSERT_NOT_NULL(fichier);

    uint8_t bits = 0b00011111;
    uint8_t octets_ecrire[3] = {0,1,2};

    ajouter_bits(&bits, 3, fichier, &buffer);
    ajouter_octets(octets_ecrire, 3, fichier, &buffer);

    fermer_fichier_sortie(fichier, &buffer);
    


    fichier = fopen(chemin_sortie, "rb");
    TEST_ASSERT_NOT_NULL(fichier);

    uint8_t octets[8];
    int octets_lus = fread(octets, sizeof(uint8_t), 8, fichier);
    
    TEST_ASSERT_EQUAL_UINT8(8, octets_lus);
    TEST_ASSERT_EQUAL_UINT8(OCTET_DEBUT_MARQUEUR, octets[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_SOI_DEBUT_IMAGE, octets[1]);

    TEST_ASSERT_EQUAL_UINT8(0b00000000, octets[2]);
    TEST_ASSERT_EQUAL_UINT8(0b00000000, octets[3]);
    TEST_ASSERT_EQUAL_UINT8(0b00100000, octets[4]);
    TEST_ASSERT_EQUAL_UINT8(0b01011111, octets[5]);
    
    TEST_ASSERT_EQUAL_UINT8(OCTET_DEBUT_MARQUEUR, octets[6]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_EOI_FIN_IMAGE, octets[7]);

    remove(chemin_sortie);
}

void test_ecriture_ac_dc(void) {

    char *chemin_sortie = "test.jpeg";

    Buffer_ecriture buffer;
    FILE *fichier = ouvrir_fichier_sortie(chemin_sortie, &buffer);
    TEST_ASSERT_NOT_NULL(fichier);

    AC_DC ac_dc;
    contenu dc = {2,2,64,7};
    ac_dc.DC = dc;
    contenu ac_1 = {1,3,0,0};
    contenu ac_2 = {22,5,2,9};
    ac_dc.AC[0] = ac_1;
    ac_dc.AC[1] = ac_2;
    ac_dc.taille = 2;

    ajouter_donnees_compressees(&ac_dc, fichier, &buffer);

    fermer_fichier_sortie(fichier, &buffer);
    


    fichier = fopen(chemin_sortie, "rb");
    TEST_ASSERT_NOT_NULL(fichier);

    uint8_t octets[8];
    int octets_lus = fread(octets, sizeof(uint8_t), 8, fichier);
    
    TEST_ASSERT_EQUAL_UINT8(8, octets_lus);
    TEST_ASSERT_EQUAL_UINT8(OCTET_DEBUT_MARQUEUR, octets[0]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_SOI_DEBUT_IMAGE, octets[1]);

    // DC = 100000010
    // AC[0] = 001
    // AC[1] = 10110000000010

    TEST_ASSERT_EQUAL_UINT8(0b10000001, octets[2]);
    TEST_ASSERT_EQUAL_UINT8(0b00011011, octets[3]);
    TEST_ASSERT_EQUAL_UINT8(0b00000000, octets[4]);
    TEST_ASSERT_EQUAL_UINT8(0b10111111, octets[5]);
    
    TEST_ASSERT_EQUAL_UINT8(OCTET_DEBUT_MARQUEUR, octets[6]);
    TEST_ASSERT_EQUAL_UINT8(MARQUEUR_EOI_FIN_IMAGE, octets[7]);

    remove(chemin_sortie);
}

int main(void) {

    UNITY_BEGIN();
    RUN_TEST(test_ecriture_vide);
    RUN_TEST(test_ecriture_octets_bits);
    RUN_TEST(test_ecriture_deccalage);
    RUN_TEST(test_ecriture_ac_dc);
    
    return UNITY_END();
}