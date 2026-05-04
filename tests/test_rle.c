#include "unity.h"
#include "rle.h"
#include <stdlib.h>
#include <stdbool.h>



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_rle(void) {

    Symboles_RLE symboles_rle_ac;
    int16_t bloc[64];

    for (uint8_t i = 0; i < 64; i++) {

        if (i < 4) bloc[i] = 0;
        else if (i < 8) bloc[i] = 12;
        else if (i < 12) bloc[i] = 0;
        else if (i < 20) bloc[i] = 7;
        else if (i < 40) bloc[i] = 3;
        else if (i < 56) bloc[i] = 1;
        else bloc[i] = 0;
    }

    int16_t dc_prec = 4;
    Magnitude bloc_enc[64];

    rle_magnitude(bloc, &dc_prec, bloc_enc, &symboles_rle_ac);

    
	uint8_t taille = 53;
    Symbole_RLE symboles[taille] = {

        
    };

    TEST_ASSERT_EQUAL_UINT8(taille, symboles_rle_ac.taille);

    TEST_ASSERT_EQUAL_UINT8(0 ,bloc_enc[0].class_mag);
    TEST_ASSERT_EQUAL_UINT16(0 ,bloc_enc[0].indice);

    for (uint8_t i = 0; i < taille; i++) {

        TEST_ASSERT_EQUAL_UINT8(symboles[i].symbole ,symboles_rle_ac.symboles[i].symbole);
        if (symboles[i].symbole != EOB && symboles[i].symbole != ZRL) {
            TEST_ASSERT_EQUAL_UINT8(symboles[i].indice ,symboles_rle_ac.symboles[i].indice);
        }
    }
}


int main(void) {

    UNITY_BEGIN();
    RUN_TEST(test_rle);

    return UNITY_END();
}