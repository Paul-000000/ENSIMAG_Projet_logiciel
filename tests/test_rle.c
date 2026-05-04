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

void test_rle_simple(void) {

    Symboles_RLE symboles_rle_ac;
    int16_t bloc[64];

    for (uint8_t i = 0; i < 64; i++) bloc[i] = 0;
    bloc[0] = 1;
    bloc[1] = 3;
    bloc[2] = 0;
    bloc[3] = 2;

    int16_t dc_prec = 4;
    Magnitude bloc_enc[64];

    rle_magnitude(bloc, &dc_prec, bloc_enc, &symboles_rle_ac);

    TEST_ASSERT_EQUAL_UINT8(2, bloc_enc[0].class_mag); // -3
    TEST_ASSERT_EQUAL_UINT16(0, bloc_enc[0].indice);

    TEST_ASSERT_EQUAL_UINT8(2, bloc_enc[1].class_mag); // 3
    TEST_ASSERT_EQUAL_UINT16(3, bloc_enc[1].indice);

    TEST_ASSERT_EQUAL_UINT8(0, bloc_enc[2].class_mag); // 0
    TEST_ASSERT_EQUAL_UINT16(0, bloc_enc[2].indice);

    TEST_ASSERT_EQUAL_UINT8(2, bloc_enc[3].class_mag); // 2
    TEST_ASSERT_EQUAL_UINT16(2, bloc_enc[3].indice);

    TEST_ASSERT_EQUAL_UINT8(3, symboles_rle_ac.taille);

    TEST_ASSERT_EQUAL_UINT8(0x02, symboles_rle_ac.symboles[0].symbole); // 3
    TEST_ASSERT_EQUAL_UINT16(3, symboles_rle_ac.symboles[0].indice);

    TEST_ASSERT_EQUAL_UINT8(0x12, symboles_rle_ac.symboles[1].symbole); // 2
    TEST_ASSERT_EQUAL_UINT16(2, symboles_rle_ac.symboles[1].indice);

    TEST_ASSERT_EQUAL_UINT8(EOB, symboles_rle_ac.symboles[2].symbole); // EOB
}

void test_rle_milieu(void) {

    Symboles_RLE symboles_rle_ac;
    int16_t bloc[64];

    for (uint8_t i = 0; i < 64; i++) bloc[i] = 0;
    bloc[20] = -1;

    int16_t dc_prec = 0;
    Magnitude bloc_enc[64];

    rle_magnitude(bloc, &dc_prec, bloc_enc, &symboles_rle_ac);


    TEST_ASSERT_EQUAL_UINT8(1, bloc_enc[20].class_mag); // 1
    TEST_ASSERT_EQUAL_UINT16(0, bloc_enc[20].indice);

    TEST_ASSERT_EQUAL_UINT8(3, symboles_rle_ac.taille);

    TEST_ASSERT_EQUAL_UINT8(ZRL, symboles_rle_ac.symboles[0].symbole); // ZRL

    TEST_ASSERT_EQUAL_UINT8(0x31, symboles_rle_ac.symboles[1].symbole); // 1
    TEST_ASSERT_EQUAL_UINT16(0, symboles_rle_ac.symboles[1].indice);

    TEST_ASSERT_EQUAL_UINT8(EOB, symboles_rle_ac.symboles[2].symbole); // EOB
}


int main(void) {

    UNITY_BEGIN();
    RUN_TEST(test_rle_simple);
    RUN_TEST(test_rle_milieu);

    return UNITY_END();
}