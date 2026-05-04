
#include "unity.h"
#include <stdint.h>
#include "huffman.h"
#include "qtables.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_huffman_construction_table_encodage(void) {
  
    uint8_t longueurs[16] = {0, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t symboles[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

    Huffman table_test[256];

    construction_arbre(table_test, longueurs, symboles);

  

    // symbole 0x00 (longueur 2, code binaire 00 -> décimal 0)
    TEST_ASSERT_EQUAL_UINT8(1, table_test[0x00].valide);
    TEST_ASSERT_EQUAL_UINT8(2, table_test[0x00].nb_bits);
    TEST_ASSERT_EQUAL_UINT16(0, table_test[0x00].code);

    // symbole 0x01 (longueur 3, code binaire 010 -> décimal 2)
    TEST_ASSERT_EQUAL_UINT8(1, table_test[0x01].valide);
    TEST_ASSERT_EQUAL_UINT8(3, table_test[0x01].nb_bits);
    TEST_ASSERT_EQUAL_UINT16(2, table_test[0x01].code);

    // symbole 0x05 (longueur 3, code binaire 110 -> décimal 6)
    TEST_ASSERT_EQUAL_UINT8(1, table_test[0x05].valide);
    TEST_ASSERT_EQUAL_UINT8(3, table_test[0x05].nb_bits);
    TEST_ASSERT_EQUAL_UINT16(6, table_test[0x05].code);

    //symbole 0x06 (longueur 4, code binaire 1110 -> décimal 14)
    TEST_ASSERT_EQUAL_UINT8(1, table_test[0x06].valide);
    TEST_ASSERT_EQUAL_UINT8(4, table_test[0x06].nb_bits);
    TEST_ASSERT_EQUAL_UINT16(14, table_test[0x06].code);

    // exemple de cas de non appartenance
    TEST_ASSERT_EQUAL_UINT8(0, table_test[0x42].valide);
}

int main(void) {
    UNITY_BEGIN();
    
    // On lance notre super test !
    RUN_TEST(test_huffman_construction_table_encodage);
    
    return UNITY_END();
}