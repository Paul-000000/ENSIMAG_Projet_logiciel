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
    
    for (int i = 0; i < 8; i++) {


        
        char msg[50];
        sprintf(msg, "Erreur de lecture à la ligne %d", i);
        
        TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(motif_attendu + (i * 8), img->tab[i], 8, msg);
    }

    liberer_image(img);


}
void test_lireEblocs_grande_image(void) {
    uint32_t L = 1024;
    uint32_t H = 1024;
    const char* filename = "test_large.pgm";

    // 1. Créer une image mathématique : Pixel(x,y) = (y*L + x) % 256
    FILE *f = fopen(filename, "wb");
    fprintf(f, "P5\n%u %u\n255\n", L, H);
    for (uint32_t i = 0; i < L * H; i++) {
        fputc((uint8_t)(i % 256), f);
    }
    fclose(f);

    // 2. Charger l'image (alloue la structure et la matrice 8x512)
    Image *img = lectureImage(filename);
    TEST_ASSERT_NOT_NULL(img);

    // 3. Lire un Super-Bloc au milieu
    uint32_t test_x = 128;
    uint32_t test_y = 200;
    lireEblocs(img, test_x, test_y);

    // 4. Vérification mathématique sur plusieurs lignes du bloc
    for (uint32_t i = 0; i < 8; i++) {
        // Valeur attendue pour le premier pixel de la ligne 'i' du bloc
        // Coordonnées réelles dans le fichier : (test_x, test_y + i)
        uint8_t attendu = (uint8_t)(((test_y + i) * L + test_x) % 256);
        
        char msg[50];
        sprintf(msg, "Erreur à la ligne locale %u", i);
        
        // NOUVEAU FORMAT : tab[i][0] au lieu de tab[index]
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu, img->tab[i][0], msg);
    }

    // 5. Nettoyage utilisant TA fonction liberer_image
    // C'est plus sûr car elle gère la boucle de free pour les lignes
    liberer_image(img);

}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_lire_image);
    RUN_TEST(test_recupEntete_P5);
    RUN_TEST(test_lectureEblocs);
    RUN_TEST(test_lireEblocs_grande_image);
  
    return UNITY_END();
}



