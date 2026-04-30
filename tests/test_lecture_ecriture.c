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

    const char* filename = "images/etu/gris.pgm";
 
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
    const char* filename = "images/etu/gris.pgm";
    
    // Paramètres pour 64 blocs de 8x8
    uint32_t L_bloc = 8;
    uint32_t H_super = 8;
    uint32_t nb_blocs = 64;

    Image* img = lectureImage(filename, L_bloc, H_super, nb_blocs);

    TEST_ASSERT_NOT_NULL(img);
    TEST_ASSERT_NOT_NULL(img->fichier);
    TEST_ASSERT_EQUAL_INT(P5, img->type);
    TEST_ASSERT_EQUAL_UINT32(320, img->largeur);
    TEST_ASSERT_EQUAL_UINT32(320, img->hauteur);

    // On utilise ta fonction de libération qui prend le paramètre de lignes
    liberer_image(img, H_super);
}

void test_lectureEblocs() {
    const char* filename = "images/etu/invader.pgm";
    uint32_t L_bloc = 8;
    uint32_t H_super = 8;
    uint32_t nb_blocs = 64;

    Image* img = lectureImage(filename, L_bloc, H_super, nb_blocs);
    
    // Appel avec la nouvelle signature
    lireEblocs(img, 0, 0, L_bloc, H_super, nb_blocs);
    
    for (int i = 0; i < 8; i++) {
        char msg[50];
        snprintf(msg, 50, "Erreur de lecture à la ligne %d", i);
        // On compare les 8 premiers pixels de chaque ligne du super-bloc
        TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(motif_attendu + (i * 8), img->tab[i], 8, msg);
    }

    liberer_image(img, H_super);
}

void test_lireEblocs_grande_image(void) {

    uint32_t L = 1024;
    uint32_t H = 1024;
    char* filename = "test_large.pgm";
    
    uint32_t L_bloc = 8;
    uint32_t H_super = 8;
    uint32_t nb_blocs = 64;

    // 1. Création de l'image de test
    FILE *f = fopen(filename, "wb");
    fprintf(f, "P5\n%u %u\n255\n", L, H);
    for (uint32_t i = 0; i < L * H; i++) {
        fputc((uint8_t)(i % 256), f);
    }
    fclose(f);

    // 2. Chargement
    Image *img = lectureImage(filename, L_bloc, H_super, nb_blocs);
    TEST_ASSERT_NOT_NULL(img);

    // 3. Lecture au milieu
    uint32_t tx = 128;
    uint32_t ty = 200;
    lireEblocs(img, tx, ty, L_bloc, H_super, nb_blocs);

    // 4. Vérification mathématique
    for (uint32_t i = 0; i < 8; i++) {
        uint8_t attendu = (uint8_t)(((ty + i) * L + tx) % 256);
        char msg[50];
        snprintf(msg, 50, "Erreur à la ligne locale %u", i);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu, img->tab[i][0], msg);
    }

    liberer_image(img, H_super);
    remove(filename); // Très important pour ne pas laisser de fichiers traîner
}

void test_lireEblocs_grande_image_P6(void) {
    uint32_t L = 1024;
    uint32_t H = 1024;
    char* filename = "test_large_p6.ppm";
    
    uint32_t L_bloc = 8;
    uint32_t H_super = 8;
    uint32_t nb_blocs = 64;

    // 1. Création de l'image P6 (RGB)
    FILE *f = fopen(filename, "wb");
    fprintf(f, "P6\n%u %u\n255\n", L, H);
    for (uint32_t y = 0; y < H; y++) {
        for (uint32_t x = 0; x < L; x++) {
            fputc((uint8_t)(x % 256), f);       // Rouge
            fputc((uint8_t)(y % 256), f);       // Vert
            fputc((uint8_t)((x + y) % 256), f); // Bleu
        }
    }
    fclose(f);

    // 2. Chargement de l'image (doit allouer 3 octets par pixel)
    Image *img = lectureImage(filename, L_bloc, H_super, nb_blocs);
    TEST_ASSERT_NOT_NULL(img);
    TEST_ASSERT_EQUAL_INT(P6, img->type);

    // 3. Lecture d'un Super-Bloc au milieu (ex: x=100, y=50)
    uint32_t tx = 100;
    uint32_t ty = 50;
    lireEblocs(img, tx, ty, L_bloc, H_super, nb_blocs);

    // 4. Vérification du premier pixel du bloc (tx, ty)
    uint8_t attendu_R = (uint8_t)(tx % 256);
    uint8_t attendu_G = (uint8_t)(ty % 256);
    uint8_t attendu_B = (uint8_t)((tx + ty) % 256);

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_R, img->tab[0][0], "Erreur Canal Rouge");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_G, img->tab[0][1], "Erreur Canal Vert");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_B, img->tab[0][2], "Erreur Canal Bleu");

    // 5. Vérification du deuxième pixel de la première ligne (tx+1, ty)
    uint8_t attendu_R1 = (uint8_t)((tx + 1) % 256);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_R1, img->tab[0][3], "Erreur Canal Rouge Pixel suivant");

    // 4. Vérification mathématique sur les 3 premiers pixels de chaque ligne
for (uint32_t i = 0; i < 8; i++) { // Pour chaque ligne du bloc
    for (uint32_t j = 0; j < 3; j++) { // Pour les 3 premiers pixels
        
        uint32_t x_actuel = tx + j;
        uint32_t y_actuel = ty + i;

        // Calcul des valeurs attendues selon ta règle de création du fichier
        uint8_t attendu_R = (uint8_t)(x_actuel % 256);
        uint8_t attendu_G = (uint8_t)(y_actuel % 256);
        uint8_t attendu_B = (uint8_t)((x_actuel + y_actuel) % 256);

        // Index dans la ligne : chaque pixel j occupe 3 cases (R, G, B)
        uint32_t base_index = j * 3;

        char msg[100];
        // Test du Rouge
        snprintf(msg, 100, "Erreur R: Ligne %u, Pixel %u", i, j);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_R, img->tab[i][base_index], msg);

        // Test du Vert
        snprintf(msg, 100, "Erreur G: Ligne %u, Pixel %u", i, j);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_G, img->tab[i][base_index + 1], msg);

        // Test du Bleu
        snprintf(msg, 100, "Erreur B: Ligne %u, Pixel %u", i, j);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(attendu_B, img->tab[i][base_index + 2], msg);
    }
}

    // 6. Nettoyage
    liberer_image(img, H_super);
    remove(filename);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_lire_image);
    RUN_TEST(test_recupEntete_P5);
    RUN_TEST(test_lectureEblocs);
    RUN_TEST(test_lireEblocs_grande_image);
    RUN_TEST(test_lireEblocs_grande_image_P6);

    return UNITY_END();
}



