#include <magnitude.h>
#include "unity.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_encoder_val(){

    Magnitude res=encoder_val(7);
    TEST_ASSERT_EQUAL_UINT8(3, res.class_mag);
    TEST_ASSERT_EQUAL_UINT16(0b111,res.indice);
}

void test_encoder_val_complet(void) {
    Magnitude res;


    res = encoder_val(0);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, res.class_mag, "Erreur Taille pour 0");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, res.indice, "Erreur Indice pour 0");


    res = encoder_val(7);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(3, res.class_mag, "Erreur Taille pour 7");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(7, res.indice, "Erreur Indice pour 7");

    res = encoder_val(12); 
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(4, res.class_mag, "Erreur Taille pour 12");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(12, res.indice, "Erreur Indice pour 12");


    res = encoder_val(-7);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(3, res.class_mag, "Erreur Taille pour -7");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, res.indice, "Erreur Indice pour -7"); 

    res = encoder_val(-12);
 

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(4, res.class_mag, "Erreur Taille pour -12");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(3, res.indice, "Erreur Indice pour -12");
}

void test_codage_magnitude_limites(void) {
    int16_t bloc[64] = {0}; 
    Magnitude bloc_enc[64];
    

    

    bloc[0] = 20;  
    bloc[1] = -5;  
    bloc[2] = 14;  
    
   
    bloc[62] = -1; 
    bloc[63] = 1;  


    int16_t dc_prec = 5;


    codage_magnitude(bloc, &dc_prec, bloc_enc);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(4, bloc_enc[0].class_mag, "Erreur DC - Taille");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(15, bloc_enc[0].indice, "Erreur DC - Indice");
    TEST_ASSERT_EQUAL_INT16_MESSAGE(20, dc_prec, "dc_prec n'a pas ete mis a jour !");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(3, bloc_enc[1].class_mag, "Erreur AC 1 - Taille");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(2, bloc_enc[1].indice, "Erreur AC 1 - Indice");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(4, bloc_enc[2].class_mag, "Erreur AC 2 - Taille");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(14, bloc_enc[2].indice, "Erreur AC 2 - Indice");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, bloc_enc[30].class_mag, "Erreur AC 30 (vide) - Taille");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, bloc_enc[30].indice, "Erreur AC 30 (vide) - Indice");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(1, bloc_enc[62].class_mag, "Erreur AC 62 (Avant-dernier) - Taille");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, bloc_enc[62].indice, "Erreur AC 62 (Avant-dernier) - Indice");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(1, bloc_enc[63].class_mag, "Erreur AC 63 (Dernier) - Taille");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(1, bloc_enc[63].indice, "Erreur AC 63 (Dernier) - Indice");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_encoder_val);
    RUN_TEST(test_encoder_val_complet);
    RUN_TEST(test_codage_magnitude_limites);

    return UNITY_END();
}