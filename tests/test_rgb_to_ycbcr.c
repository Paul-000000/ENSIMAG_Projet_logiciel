#include "unity.h"
#include "rgb_to_ycbcr.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_valeurs_min_y(void) {

    Couleur_rgb couleur = {0,0,0};
    Couleur_ycbcr ycbcr = rgb_to_ycbcr(couleur);

    TEST_ASSERT_EQUAL_UINT8(0, ycbcr.y);
    TEST_ASSERT_EQUAL_UINT8(128, ycbcr.cb);
    TEST_ASSERT_EQUAL_UINT8(128, ycbcr.cr);
}

void test_valeurs_max_y(void) {

    Couleur_rgb couleur = {255,255,255};
    Couleur_ycbcr ycbcr = rgb_to_ycbcr(couleur);

    TEST_ASSERT_EQUAL_UINT8(255, ycbcr.y);
    TEST_ASSERT_EQUAL_UINT8(128, ycbcr.cb);
    TEST_ASSERT_EQUAL_UINT8(128, ycbcr.cr);
}

void test_valeurs_min_cb(void) {

    Couleur_rgb couleur = {255,255,0};
    Couleur_ycbcr ycbcr = rgb_to_ycbcr(couleur);

    TEST_ASSERT_UINT8_WITHIN(1, 226, ycbcr.y);
    TEST_ASSERT_EQUAL_UINT8(0, ycbcr.cb);
    TEST_ASSERT_UINT8_WITHIN(1, 149, ycbcr.cr);
}

void test_valeurs_max_cb(void) {

    Couleur_rgb couleur = {0,0,255};
    Couleur_ycbcr ycbcr = rgb_to_ycbcr(couleur);

    TEST_ASSERT_UINT8_WITHIN(1, 29, ycbcr.y);
    TEST_ASSERT_EQUAL_UINT8(255, ycbcr.cb);
    TEST_ASSERT_UINT8_WITHIN(1, 107, ycbcr.cr);
}

void test_valeurs_min_cr(void) {

    Couleur_rgb couleur = {0,255,255};
    Couleur_ycbcr ycbcr = rgb_to_ycbcr(couleur);

    TEST_ASSERT_UINT8_WITHIN(1, 179, ycbcr.y);
    TEST_ASSERT_UINT8_WITHIN(1, 171, ycbcr.cb);
    TEST_ASSERT_EQUAL_UINT8(0, ycbcr.cr);
}

void test_valeurs_max_cr(void) {

    Couleur_rgb couleur = {255,0,0};
    Couleur_ycbcr ycbcr = rgb_to_ycbcr(couleur);

    TEST_ASSERT_UINT8_WITHIN(1, 76, ycbcr.y);
    TEST_ASSERT_UINT8_WITHIN(1, 85, ycbcr.cb);
    TEST_ASSERT_EQUAL_UINT8(255, ycbcr.cr);
}


int main(void) {

    UNITY_BEGIN();

    RUN_TEST(test_valeurs_min_y);
    //RUN_TEST(test_valeurs_max_y);
    //RUN_TEST(test_valeurs_min_cb);
    RUN_TEST(test_valeurs_max_cb);
    RUN_TEST(test_valeurs_min_cr);
    RUN_TEST(test_valeurs_max_cr);
    
    return UNITY_END();
}
