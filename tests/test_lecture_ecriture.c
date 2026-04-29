#include "unity.h"
#include "lecture_ecriture.h"
#include <stdlib.h>

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}
void test_recupEntete_P5(void) {

    const char* filename = "/Users/test/Desktop/1A/S2/Projet_C/2_bahag_becharam_bottnerp/images/etu/gris.pgm";
 
    FILE *f_test = fopen(filename, "rb");
    TEST_ASSERT_NOT_NULL(f_test);
    Image* img = recupEntete(f_test);

    
    TEST_ASSERT_NOT_NULL(img);
    TEST_ASSERT_EQUAL_INT(P5, img->type);
    TEST_ASSERT_EQUAL_UINT32(320, img->largeur);
    TEST_ASSERT_EQUAL_UINT32(320, img->hauteur);
    TEST_ASSERT_EQUAL_INT(15, img->debut_pixels);
    fclose(f_test);

    free(img); 
   
}
uint8_t motif_attendu[64] = {
        0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
        0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00,
        0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0x00,
        0xff, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff
    };

void test_lire_image(void) {

    const char* filename = "/Users/test/Desktop/1A/S2/Projet_C/2_bahag_becharam_bottnerp/images/etu/gris.pgm";
 
   
    Image* img = lectureImage(filename);

    
    TEST_ASSERT_NOT_NULL(img);
    TEST_ASSERT_NOT_NULL(img->fichier);

    TEST_ASSERT_EQUAL_INT(P5, img->type);
    TEST_ASSERT_EQUAL_UINT32(320, img->largeur);
    TEST_ASSERT_EQUAL_UINT32(320, img->hauteur);
    TEST_ASSERT_EQUAL_INT(15, img->debut_pixels);

    if (img)
    {
        if (img->fichier)
    {
        fclose(img->fichier);
    }
        free(img); 
    }
}

void test_lectureEblocs(){
    const char* filename = "/Users/test/Desktop/1A/S2/Projet_C/2_bahag_becharam_bottnerp/images/etu/invader.pgm";

    Image* img = lectureImage(filename);
    lireEblocs(img, 0, 0);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(motif_attendu, img->tab, 64);
}

void test_lireEblocs_grande_image(void) {
    uint32_t L = 1024;
    uint32_t H = 1024;
    const char* filename = "test_large.pgm";

    // 1. Créer une image de 1024x1024 en mémoire et l'écrire sur disque
    FILE *f = fopen(filename, "wb");
    fprintf(f, "P5\n%u %u\n255\n", L, H);
    
    for (uint32_t i = 0; i < L * H; i++) {
        uint8_t pixel = (uint8_t)(i % 256); 
        fputc(pixel, f);
    }
    fclose(f);

    // 2. Charger l'image avec tes fonctions
    Image *img = lectureImage(filename);
    
    // 3. Tester la lecture d'un bloc au MILIEU de l'image
    // On veut lire 64 blocs (512px) à partir de x=128, y=200
    uint32_t test_x = 128;
    uint32_t test_y = 200;
    lireEblocs(img, test_x, test_y);

    // 4. Vérification mathématique
    // Le premier pixel du buffer (index 0) doit correspondre au pixel (128, 200) du fichier
    // Valeur attendue = (y * Largeur + x) % 256
    uint8_t attendu = (uint8_t)((test_y * L + test_x) % 256);
    TEST_ASSERT_EQUAL_HEX8(attendu, img->tab[0]);

    // Nettoyage
     if (img)
    {
        if (img->fichier)
    {
        fclose(img->fichier);
    }
        free(img); 
    }
    
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_lire_image);
    RUN_TEST(test_recupEntete_P5);
    RUN_TEST(test_lectureEblocs);
    RUN_TEST(test_lireEblocs_grande_image);

    return UNITY_END();
}



