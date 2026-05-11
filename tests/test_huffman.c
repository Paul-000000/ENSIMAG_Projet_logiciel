
#include "unity.h"
#include <stdint.h>
#include "huffman.h"
#include "qtables.h"
#include "string.h"
#include "magnitude.h"



void setUp(void) {

}

void tearDown(void) {

}

void test_huffman_construction_table_encodage(void) {
  
    uint8_t longueurs[16] = {0, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t symboles[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

    Huffman table_test[256];

    construction_arbre_huffman(table_test, longueurs, symboles);

  

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

void test_huffman_rle_magnitude(void) {
    
    int16_t bloc[64] = {0}; 
    
    bloc[0] = 15;  // DC
    bloc[1] = 0;  
    bloc[2] = -2;  // AC non nul
    
    int16_t dc_prec = 10; // Le DC précédent. DPCM = 15 - 10 = 5.

    Huffman tab_dc[256];
    Huffman tab_ac[256];
    memset(tab_dc, 0, sizeof(tab_dc)); 
    memset(tab_ac, 0, sizeof(tab_ac));

  
    // Différence = 5. La magnitude de 5 est 3. Indice = 5.
    tab_dc[3].valide = 1;
    tab_dc[3].code = 0b101;
    tab_dc[3].nb_bits = 3;

    tab_ac[0x12].valide = 1;
    tab_ac[0x12].code = 0b1100; 
    tab_ac[0x12].nb_bits = 4;  
   
    tab_ac[0x00].valide = 1;
    tab_ac[0x00].code = 0b10; 
    tab_ac[0x00].nb_bits = 2;

   
    AC_DC resultat;
    Symboles_RLE symboles_rle_ac;
    Magnitude bloc_enc[64];

    codage_magnitude(bloc, &dc_prec, bloc_enc);
    rle(bloc, &symboles_rle_ac, bloc_enc);
    initialise_huffman();
    huffman(bloc_enc, &symboles_rle_ac, tab_dc, tab_ac, &resultat);


    
    TEST_ASSERT_EQUAL_INT16(15, dc_prec);

    
    TEST_ASSERT_EQUAL_UINT16(0b101, resultat.DC.code);
    TEST_ASSERT_EQUAL_UINT8(3, resultat.DC.nb_bits);
    TEST_ASSERT_EQUAL_UINT16(5, resultat.DC.indice);

    TEST_ASSERT_EQUAL_UINT8(2, resultat.taille);

    
    TEST_ASSERT_EQUAL_UINT16(0b1100, resultat.AC[0].code);
    TEST_ASSERT_EQUAL_UINT8(4, resultat.AC[0].nb_bits);
    TEST_ASSERT_EQUAL_UINT16(1, resultat.AC[0].indice); 


    TEST_ASSERT_EQUAL_UINT16(0b10, resultat.AC[1].code);
    TEST_ASSERT_EQUAL_UINT8(2, resultat.AC[1].nb_bits);
}

void test_huffman_long_zero_run(void) {
    int16_t bloc[64] = {0};
    bloc[0] = 10;   // DC
    // 16 zéros entre l'indice 1 et 16
    bloc[17] = 3;   // Un AC après 16 zéros
    
    int16_t dc_prec = 10; // Différence = 0 (Magnitude 0)
    
    Huffman tab_dc[256] = {0};
    Huffman tab_ac[256] = {0};

    // Configuration DC (Magnitude 0)
    tab_dc[0].valide = 1;
    tab_dc[0].code = 0x00;
    tab_dc[0].nb_bits = 2;

    // Configuration ZRL (0xF0) : Indique 16 zéros
    tab_ac[0xF0].valide = 1;
    tab_ac[0xF0].code = 0b11111111001; // Code standard
    tab_ac[0xF0].nb_bits = 11;

    // Configuration AC (Run de 0 après le ZRL, Magnitude de '3' est 2) -> Symbole 0x02
    tab_ac[0x02].valide = 1;
    tab_ac[0x02].code = 0b01;
    tab_ac[0x02].nb_bits = 2;

    // EOB (Fin de bloc)
    tab_ac[0x00].valide = 1;
    tab_ac[0x00].code = 0b10;
    tab_ac[0x00].nb_bits = 2;

    AC_DC resultat;
    Symboles_RLE symboles_rle_ac;
    Magnitude bloc_enc[64];

    codage_magnitude(bloc, &dc_prec, bloc_enc);
    rle(bloc, &symboles_rle_ac, bloc_enc);
    initialise_huffman();
    huffman(bloc_enc, &symboles_rle_ac, tab_dc, tab_ac, &resultat);

    // On s'attend à 3 symboles AC : ZRL, le "3", et l'EOB
    TEST_ASSERT_EQUAL_UINT8(3, resultat.taille);
    TEST_ASSERT_EQUAL_UINT16(0b11111111001, resultat.AC[0].code); // ZRL
    TEST_ASSERT_EQUAL_UINT16(0b01, resultat.AC[1].code);          // Le chiffre 3
    TEST_ASSERT_EQUAL_UINT16(0b10, resultat.AC[2].code);          // EOB
}

void test_huffman_indices_negatifs(void) {
    int16_t bloc[64] = {0};
    bloc[0] = -5; // DC
    int16_t dc_prec = 0; // Différence = -5. Magnitude = 3. Indice = 2 (en binaire: 010)
    
    Huffman tab_dc[256] = {0};
    Huffman tab_ac[256] = {0};

    tab_dc[3].valide = 1;
    tab_dc[3].code = 0b100;
    tab_dc[3].nb_bits = 3;
    
    tab_ac[0x00].valide = 1; // EOB

    AC_DC resultat;
    Symboles_RLE symboles_rle_ac;
    Magnitude bloc_enc[64];

    codage_magnitude(bloc, &dc_prec, bloc_enc);
    rle(bloc, &symboles_rle_ac, bloc_enc);
    initialise_huffman();
    huffman(bloc_enc, &symboles_rle_ac, tab_dc, tab_ac, &resultat);

    // Vérification de la magnitude du DC
    TEST_ASSERT_EQUAL_UINT16(3, resultat.DC.nb_bits); 
    // L'indice pour -5 sur 3 bits doit être 2 (0b010)
    TEST_ASSERT_EQUAL_UINT16(2, resultat.DC.indice); 
}

void test_huffman_bloc_vide(void) {
    int16_t bloc[64] = {0}; // Tout à zéro sauf potentiellement le DC
    bloc[0] = 120;
    int16_t dc_prec = 120; // Différence = 0
    
    Huffman tab_dc[256] = {0};
    Huffman tab_ac[256] = {0};

    tab_dc[0].valide = 1; 
    tab_ac[0x00].valide = 1; // EOB
    tab_ac[0x00].code = 0b1010;

    AC_DC resultat;
    Symboles_RLE symboles_rle_ac;
    Magnitude bloc_enc[64];

    codage_magnitude(bloc, &dc_prec, bloc_enc);
    rle(bloc, &symboles_rle_ac, bloc_enc);
    initialise_huffman();
    huffman(bloc_enc, &symboles_rle_ac, tab_dc, tab_ac, &resultat);

    // Taille doit être 1 (uniquement le symbole EOB)
    TEST_ASSERT_EQUAL_UINT8(1, resultat.taille);
    TEST_ASSERT_EQUAL_UINT16(0b1010, resultat.AC[0].code);
}

void test_huffman_y(void) {

    int16_t dc_prec = 0;
    int16_t bloc[64];
    Magnitude bloc_enc[64];
    Symboles_RLE symboles_rle_ac;
    AC_DC resultat;

    for (uint8_t i = 0; i < 64; i++) {
        bloc[i] = 0;
    }

    bloc[0] = 1;
    bloc[1] = 3;
    bloc[2] = 0;
    bloc[3] = 2;

    initialise_huffman();
    codage_magnitude(bloc, &dc_prec, bloc_enc);
    rle(bloc, &symboles_rle_ac, bloc_enc);

    encoder_coefficients_huffman(bloc_enc, &symboles_rle_ac, Y, &resultat);

    TEST_ASSERT_EQUAL_UINT8(1, resultat.DC.classe_mag);
    TEST_ASSERT_EQUAL_UINT16(1, resultat.DC.indice);

    TEST_ASSERT_EQUAL_UINT16(2, resultat.DC.code);
    TEST_ASSERT_EQUAL_UINT8(3, resultat.DC.nb_bits);

    TEST_ASSERT_EQUAL_UINT8(3, resultat.taille);

    TEST_ASSERT_EQUAL_UINT8(2, resultat.AC[0].classe_mag);
    TEST_ASSERT_EQUAL_UINT16(3, resultat.AC[0].indice);
    TEST_ASSERT_EQUAL_UINT16(1, resultat.AC[0].code);
    TEST_ASSERT_EQUAL_UINT8(2, resultat.AC[0].nb_bits);

    TEST_ASSERT_EQUAL_UINT8(2, resultat.AC[1].classe_mag);
    TEST_ASSERT_EQUAL_UINT16(2, resultat.AC[1].indice);
    TEST_ASSERT_EQUAL_UINT16(27, resultat.AC[1].code);
    TEST_ASSERT_EQUAL_UINT8(5, resultat.AC[1].nb_bits);

    TEST_ASSERT_EQUAL_UINT8(0, resultat.AC[2].classe_mag);
    TEST_ASSERT_EQUAL_UINT16(0, resultat.AC[2].indice);
    TEST_ASSERT_EQUAL_UINT16(10, resultat.AC[2].code);
    TEST_ASSERT_EQUAL_UINT8(4, resultat.AC[2].nb_bits);
}

void test_huffman_cbcr(void) {

    int16_t dc_prec = 0;
    int16_t bloc[64];
    Magnitude bloc_enc[64];
    Symboles_RLE symboles_rle_ac;
    AC_DC resultat;

    for (uint8_t i = 0; i < 64; i++) {
        bloc[i] = 0;
    }

    bloc[0] = 1;
    bloc[1] = 3;
    bloc[2] = 0;
    bloc[3] = 2;

    initialise_huffman();
    codage_magnitude(bloc, &dc_prec, bloc_enc);
    rle(bloc, &symboles_rle_ac, bloc_enc);

    encoder_coefficients_huffman(bloc_enc, &symboles_rle_ac, CB, &resultat);

    TEST_ASSERT_EQUAL_UINT8(1, resultat.DC.classe_mag);
    TEST_ASSERT_EQUAL_UINT16(1, resultat.DC.indice);
    TEST_ASSERT_EQUAL_UINT16(1, resultat.DC.code);
    TEST_ASSERT_EQUAL_UINT8(2, resultat.DC.nb_bits);

    TEST_ASSERT_EQUAL_UINT8(3, resultat.taille);

    TEST_ASSERT_EQUAL_UINT8(2, resultat.AC[0].classe_mag);
    TEST_ASSERT_EQUAL_UINT16(3, resultat.AC[0].indice);
    TEST_ASSERT_EQUAL_UINT16(4, resultat.AC[0].code);
    TEST_ASSERT_EQUAL_UINT8(3, resultat.AC[0].nb_bits);

    TEST_ASSERT_EQUAL_UINT8(2, resultat.AC[1].classe_mag);
    TEST_ASSERT_EQUAL_UINT16(2, resultat.AC[1].indice);
    TEST_ASSERT_EQUAL_UINT16(57, resultat.AC[1].code);
    TEST_ASSERT_EQUAL_UINT8(6, resultat.AC[1].nb_bits);

    TEST_ASSERT_EQUAL_UINT8(0, resultat.AC[2].classe_mag);
    TEST_ASSERT_EQUAL_UINT16(0, resultat.AC[2].indice);
    TEST_ASSERT_EQUAL_UINT16(0, resultat.AC[2].code);
    TEST_ASSERT_EQUAL_UINT8(2, resultat.AC[2].nb_bits);
}


int main(void) {
    
    UNITY_BEGIN();
    
    // On lance notre super test !
    RUN_TEST(test_huffman_construction_table_encodage);

    // pour huffman rle magnitude
    RUN_TEST(test_huffman_rle_magnitude);
    RUN_TEST(test_huffman_long_zero_run);
    RUN_TEST(test_huffman_bloc_vide);
    RUN_TEST(test_huffman_indices_negatifs);
    RUN_TEST(test_huffman_y);
    //RUN_TEST(test_huffman_cbcr);
    
    return UNITY_END();
}