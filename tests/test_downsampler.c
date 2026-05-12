#include "unity.h"
#include "downsampler.h"
#include "lecture.h"



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_simple_2x2(void) {
    
    Facteurs_echantillonnage facteurs = {2,2,1,1,1,1};

    Dimensions_cbcr dim_cb_cr = determiner_dimensions_cb_cr(facteurs);

    uint8_t hauteur_mcu, largeur_mcu;
    determiner_facteurs_mcu(facteurs, &largeur_mcu, &hauteur_mcu);

    Couleur_ycbcr matrice[MCU_MAX][MCU_MAX];
    int val = 0;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {

                    Couleur_ycbcr couleur = {val, val * 2, val * 3};
                    matrice[i * 8 + k][j * 8 + l] = couleur;
                }
            }

            val++;
        }
    }

    Vecteurs_ycbcr vecteurs_sortie;
    decouper_matrices_couleur(matrice, largeur_mcu, hauteur_mcu, dim_cb_cr, &vecteurs_sortie);

    TEST_ASSERT_EQUAL_UINT8(6,vecteurs_sortie.nb_vecteurs);

    for (uint8_t i = 0; i < vecteurs_sortie.nb_vecteurs; i++) {
        
        if (i < 4) {

            uint8_t valeurs_attendues[64];
            for (uint8_t j = 0; j < 64; j++) valeurs_attendues[j] = i;

            TEST_ASSERT_EQUAL_UINT8_ARRAY(valeurs_attendues, vecteurs_sortie.vecteurs[i].valeur, 64);
            TEST_ASSERT_EQUAL_UINT8(Y,vecteurs_sortie.vecteurs[i].composante);
        
        } else if (i == 4) {

            uint8_t valeurs_attendues[64] = {
                0,0,0,0,2,2,2,2,
                0,0,0,0,2,2,2,2,
                0,0,0,0,2,2,2,2,
                0,0,0,0,2,2,2,2,
                4,4,4,4,6,6,6,6,
                4,4,4,4,6,6,6,6,
                4,4,4,4,6,6,6,6,
                4,4,4,4,6,6,6,6,
            };

            TEST_ASSERT_EQUAL_UINT8_ARRAY(valeurs_attendues, vecteurs_sortie.vecteurs[i].valeur, 64);
            TEST_ASSERT_EQUAL_UINT8(CB,vecteurs_sortie.vecteurs[i].composante);

        } else  if (i == 5) {
            
            uint8_t valeurs_attendues[64] = {
                0,0,0,0,3,3,3,3,
                0,0,0,0,3,3,3,3,
                0,0,0,0,3,3,3,3,
                0,0,0,0,3,3,3,3,
                6,6,6,6,9,9,9,9,
                6,6,6,6,9,9,9,9,
                6,6,6,6,9,9,9,9,
                6,6,6,6,9,9,9,9,
            };

            TEST_ASSERT_EQUAL_UINT8_ARRAY(valeurs_attendues, vecteurs_sortie.vecteurs[i].valeur, 64);
            TEST_ASSERT_EQUAL_UINT8(CR,vecteurs_sortie.vecteurs[i].composante);
        }
    }
}
 
void test_aucune_perte_1x1(void) {
    
    Facteurs_echantillonnage facteurs = {1,1,1,1,1,1};

    Dimensions_cbcr dim_cb_cr = determiner_dimensions_cb_cr(facteurs);

    uint8_t hauteur_mcu, largeur_mcu;
    determiner_facteurs_mcu(facteurs, &largeur_mcu, &hauteur_mcu);

    Couleur_ycbcr matrice[MCU_MAX][MCU_MAX];
    int val = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            
            Couleur_ycbcr couleur = {val, val * 2, val * 3};
            matrice[i][j] = couleur;
            val++;
        }
    }

    Vecteurs_ycbcr vecteurs_sortie;
    decouper_matrices_couleur(matrice, largeur_mcu, hauteur_mcu, dim_cb_cr, &vecteurs_sortie);

    TEST_ASSERT_EQUAL_UINT8(3,vecteurs_sortie.nb_vecteurs);

    for (uint8_t i = 0; i < vecteurs_sortie.nb_vecteurs; i++) { // test valeurs
        
        uint8_t valeurs_attendues[64];
        for (uint8_t j = 0; j < 64; j++) valeurs_attendues[j] = j*(i +1);

        TEST_ASSERT_EQUAL_UINT8_ARRAY(valeurs_attendues, vecteurs_sortie.vecteurs[i].valeur, 64);
        
        Composante c;
        switch (i) {
            case 0:
                c = Y;
                break;
            
            case 1:
                c = CB;
                break;
            
            default:
                c = CR;
                break;
        }
        
        TEST_ASSERT_EQUAL_UINT8(c, vecteurs_sortie.vecteurs[i].composante);
    }
}

void test_aucune_perte_2x2(void) {
    
    Facteurs_echantillonnage facteurs = {2,2,2,2,2,2};

    Dimensions_cbcr dim_cb_cr = determiner_dimensions_cb_cr(facteurs);

    uint8_t hauteur_mcu, largeur_mcu;
    determiner_facteurs_mcu(facteurs, &largeur_mcu, &hauteur_mcu);

    Couleur_ycbcr matrice[MCU_MAX][MCU_MAX];
    int val = 0;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {

                    Couleur_ycbcr couleur = {val, val * 2, val * 3};
                    matrice[i * 8 + k][j * 8 + l] = couleur;
                }
            }

            val++;
        }
    }

    Vecteurs_ycbcr vecteurs_sortie;
    decouper_matrices_couleur(matrice, largeur_mcu, hauteur_mcu, dim_cb_cr, &vecteurs_sortie);

    TEST_ASSERT_EQUAL_UINT8(12,vecteurs_sortie.nb_vecteurs);

    for (uint8_t i = 0; i < vecteurs_sortie.nb_vecteurs; i++) { // test valeurs
        
        uint8_t valeurs_attendues[64];
        for (uint8_t j = 0; j < 64; j++) valeurs_attendues[j] = (i % 4) * (i / 4 + 1);
    
        TEST_ASSERT_EQUAL_UINT8_ARRAY(valeurs_attendues, vecteurs_sortie.vecteurs[i].valeur, 64);
        TEST_ASSERT_EQUAL_UINT8(i / 4,vecteurs_sortie.vecteurs[i].composante);
    }
}
 
void test_3x3(void) {
    
    Facteurs_echantillonnage facteurs = {3,3,3,1,1,1};

    Dimensions_cbcr dim_cb_cr = determiner_dimensions_cb_cr(facteurs);

    uint8_t hauteur_mcu, largeur_mcu;
    determiner_facteurs_mcu(facteurs, &largeur_mcu, &hauteur_mcu);

    Couleur_ycbcr matrice[MCU_MAX][MCU_MAX];
    int val = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {

                    Couleur_ycbcr couleur = {val, val * 2, val * 3};
                    matrice[i * 8 + k][j * 8 + l] = couleur;
                }
            }
            val++;
        }
    }

    Vecteurs_ycbcr vecteurs_sortie;
    decouper_matrices_couleur(matrice, largeur_mcu, hauteur_mcu, dim_cb_cr, &vecteurs_sortie);

    TEST_ASSERT_EQUAL_UINT8(13, vecteurs_sortie.nb_vecteurs);

    for (uint8_t i = 0; i < 9; i++) { // tests Y
        
        uint8_t valeurs_attendues[64];
        for (uint8_t j = 0; j < 64; j++) valeurs_attendues[j] = i;
    
        TEST_ASSERT_EQUAL_UINT8_ARRAY(valeurs_attendues, vecteurs_sortie.vecteurs[i].valeur, 64);
        TEST_ASSERT_EQUAL_UINT8(Y, vecteurs_sortie.vecteurs[i].composante);
    }

    uint8_t valeurs_attendues_cb_1[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        2, 2, 2, 2, 2, 2, 2, 2,
        6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6,
        10, 10, 10, 10, 10, 10, 10, 10,
        12, 12, 12, 12, 12, 12, 12, 12,
        12, 12, 12, 12, 12, 12, 12, 12
    };

    uint8_t valeurs_attendues_cb_2[64] = {
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        4, 4, 4, 4, 4, 4, 4, 4,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        12, 12, 12, 12, 12, 12, 12, 12,
        14, 14, 14, 14, 14, 14, 14, 14,
        14, 14, 14, 14, 14, 14, 14, 14
    };

    uint8_t valeurs_attendues_cb_3[64] = {
        4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4,
        6, 6, 6, 6, 6, 6, 6, 6,
        10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10, 10, 10,
        14, 14, 14, 14, 14, 14, 14, 14,
        16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16
    };

    TEST_ASSERT_EQUAL_UINT8_ARRAY(valeurs_attendues_cb_1, vecteurs_sortie.vecteurs[9].valeur, 64);
    TEST_ASSERT_EQUAL_UINT8(CB, vecteurs_sortie.vecteurs[9].composante);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(valeurs_attendues_cb_2, vecteurs_sortie.vecteurs[10].valeur, 64);
    TEST_ASSERT_EQUAL_UINT8(CB, vecteurs_sortie.vecteurs[10].composante);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(valeurs_attendues_cb_3, vecteurs_sortie.vecteurs[11].valeur, 64);
    TEST_ASSERT_EQUAL_UINT8(CB, vecteurs_sortie.vecteurs[11].composante);

    uint8_t valeurs_attendues_cr[64] = {
        0, 0, 1, 3, 3, 5, 6, 6,
        0, 0, 1, 3, 3, 5, 6, 6,
        3, 3, 4, 6, 6, 8, 9, 9,
        9, 9, 10, 12, 12, 14, 15, 15,
        9, 9, 10, 12, 12, 14, 15, 15,
        15, 15, 16, 18, 18, 20, 21, 21,
        18, 18, 19, 21, 21, 23, 24, 24,
        18, 18, 19, 21, 21, 23, 24, 24
    };

    TEST_ASSERT_EQUAL_UINT8_ARRAY(valeurs_attendues_cr, vecteurs_sortie.vecteurs[12].valeur, 64);
    TEST_ASSERT_EQUAL_UINT8(CR, vecteurs_sortie.vecteurs[12].composante);
}   
 

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_simple_2x2);
    RUN_TEST(test_aucune_perte_1x1);
    RUN_TEST(test_aucune_perte_2x2);
    RUN_TEST(test_3x3);
    return UNITY_END();
}
