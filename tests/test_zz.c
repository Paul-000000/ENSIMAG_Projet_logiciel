#include "unity.h"
#include "zz.h"
#include "dct.h"
#include "qtables.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test2(void){
	
    TEST_ASSERT(1 > 0);
	TEST_ASSERT_FALSE(2+2==5);
    // TEST_FAIL_MESSAGE("Parce que la vie est injuste!");
}

void test_position(void) {
    int16_t identite[64] = {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31,
        32, 33, 34, 35, 36, 37, 38, 39,
        40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61, 62, 63
    };
    int16_t zzi[64] = {
	0, 1, 8, 16, 9, 2, 3, 10, 
	17, 24, 32, 25, 18, 11, 4, 5, 
	12, 19, 26, 33, 40, 48, 41, 34, 
	27, 20, 13, 6, 7, 14, 21, 28, 
	35, 42, 49, 56, 57, 50, 43, 36, 
	29, 22, 15, 23, 30, 37, 44, 51, 
	58, 59, 52, 45, 38, 31, 39, 46, 
	53, 60, 61, 54, 47, 55, 62, 63 
    };

    appli_zigzag(identite);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(zzi,identite,64);
}

void test_quantification ( void ){

    uint8_t table_test[64] = {
         7,  10,  10,  10,  50,  50, 100, 100, // Ligne 0
        100, 100, 100, 100, 100, 100, 100, 100, // Ligne 1
        100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100  // Ligne 7
    };

    int16_t bloc_entree[64] = {
         45,  // 45 / 10 = 4.5 -> Doit s'arrondir à 5
        -45,  // -45 / 10 = -4.5 -> Doit s'arrondir à -5
         44,  // 44 / 10 = 4.4 -> Doit s'arrondir à 4 (tronqué vers le bas)
        -44,  // -44 / 10 = -4.4 -> Doit s'arrondir à -4
        125,  // 125 / 50 = 2.5 -> Doit s'arrondir à 3
       -125,  // -125 / 50 = -2.5 -> Doit s'arrondir à -3
         49,  // 49 / 100 = 0.49 -> Doit s'arrondir à 0 (haute fréquence écrasée)
        -49,  // -49 / 100 = -0.49 -> Doit s'arrondir à 0 (haute fréquence écrasée)
          0   // Le reste sera à 0 (initialisé automatiquement par le C pour les valeurs non précisées)
    };
    int16_t bloc_attendu[64] = {
          6, 
         -5, 
          4, 
         -4, 
          3, 
         -3, 
          0, 
          0,
          0 // Le reste est 0
    };
    applique_quant(bloc_entree, table_test);
    TEST_ASSERT_EQUAL_INT16_ARRAY(bloc_attendu, bloc_entree, 64);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_position);
    RUN_TEST(test_quantification);
    RUN_TEST(test2);
    return UNITY_END();
}
