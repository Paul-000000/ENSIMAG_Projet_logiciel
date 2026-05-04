#include "unity.h"
#include "lecture.h"
#include <stdlib.h>
<<<<<<< HEAD:tests/test_lecture_ecriture.c
#include <stdbool.h>
=======



>>>>>>> 705765ad79afa5a2b3f30518625fc9f274e2ebd9:tests/test_lecture.c
void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}
void test_recupEntete_P5(void) {

    char* filename = "images/etu/gris.pgm";
 
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

void test_lire_image(void) {
    
    char* filename = "images/etu/gris.pgm";
    
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

    char* filename = "images/etu/invader.pgm";
    
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

<<<<<<< HEAD:tests/test_lecture_ecriture.c
void test_lireEblocs_avec_padding_P5(void) {
    uint32_t L = 5; // Largeur image
    uint32_t H = 5; // Hauteur image
    char* filename = "test_padding_p5.pgm";
    
    uint32_t L_bloc = 8;
    uint32_t H_super = 8;
    uint32_t nb_blocs = 1; // On teste sur 1 seul bloc (8 pixels de large) pour faire simple


    FILE *f = fopen(filename, "wb");
    fprintf(f, "P5\n%u %u\n255\n", L, H);
    for (uint32_t y = 0; y < H; y++) {
        for (uint32_t x = 0; x < L; x++) {
            fputc((uint8_t)(y * 10 + x), f); 
        }
    }
    fclose(f);


    Image *img = lectureImage(filename, L_bloc, H_super, nb_blocs);
    TEST_ASSERT_NOT_NULL(img);


    lireEblocs(img, 0, 0, L_bloc, H_super, nb_blocs);

    // --- VÉRIFICATIONS ---


    TEST_ASSERT_EQUAL_HEX8_MESSAGE(44, img->tab[4][4], "Erreur zone valide");


    TEST_ASSERT_EQUAL_HEX8_MESSAGE(4, img->tab[0][5], "Erreur Padding Horizontal 1");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(4, img->tab[0][7], "Erreur Padding Horizontal 2");


    TEST_ASSERT_EQUAL_HEX8_MESSAGE(40, img->tab[5][0], "Erreur Padding Vertical 1");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(40, img->tab[7][0], "Erreur Padding Vertical 2");



    TEST_ASSERT_EQUAL_HEX8_MESSAGE(44, img->tab[7][7], "Erreur Padding Coin Inferieur Droit");

    // 4. Nettoyage
    liberer_image(img, H_super);
    remove(filename);
}

void test_lireEblocs_avec_padding_P6(void) {
    uint32_t L = 5; // Largeur image
    uint32_t H = 5; // Hauteur image
    char* filename = "test_padding_p6.ppm"; 
    
    uint32_t L_bloc = 8;
    uint32_t H_super = 8;
    uint32_t nb_blocs = 1; // 1 bloc de 8x8


    FILE *f = fopen(filename, "wb");
    fprintf(f, "P6\n%u %u\n255\n", L, H);
    for (uint32_t y = 0; y < H; y++) {
        for (uint32_t x = 0; x < L; x++) {
            fputc((uint8_t)(y * 10 + x), f);       // Canal Rouge
            fputc((uint8_t)(y * 10 + x + 100), f); // Canal Vert
            fputc((uint8_t)(y * 10 + x + 200), f); // Canal Bleu
        }
    }
    fclose(f);

    Image *img = lectureImage(filename, L_bloc, H_super, nb_blocs);
    TEST_ASSERT_NOT_NULL(img);
    TEST_ASSERT_EQUAL_INT(P6, img->type);

    lireEblocs(img, 0, 0, L_bloc, H_super, nb_blocs);





    TEST_ASSERT_EQUAL_HEX8_MESSAGE(44, img->tab[4][4 * 3 + 0], "Erreur Pixel (4,4) - R");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(144, img->tab[4][4 * 3 + 1], "Erreur Pixel (4,4) - G");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(244, img->tab[4][4 * 3 + 2], "Erreur Pixel (4,4) - B");

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(4, img->tab[0][7 * 3 + 0], "Erreur Pad Horiz - R");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(104, img->tab[0][7 * 3 + 1], "Erreur Pad Horiz - G");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(204, img->tab[0][7 * 3 + 2], "Erreur Pad Horiz - B");

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(40, img->tab[7][0 * 3 + 0], "Erreur Pad Vert - R");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(140, img->tab[7][0 * 3 + 1], "Erreur Pad Vert - G");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(240, img->tab[7][0 * 3 + 2], "Erreur Pad Vert - B");

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(44, img->tab[7][7 * 3 + 0], "Erreur Pad Coin - R");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(144, img->tab[7][7 * 3 + 1], "Erreur Pad Coin - G");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(244, img->tab[7][7 * 3 + 2], "Erreur Pad Coin - B");


    liberer_image(img, H_super);
    remove(filename);
}


void test_extraire_mcu_P5(void) {
    uint32_t L = 16; // 2 blocs de large
    uint32_t H = 8;  // 1 bloc de haut
    char* filename = "test_extract_mcu.pgm";
    
    uint32_t L_mcu = 8;
    uint32_t H_mcu = 8;

    // 1. Création de l'image de test mathématique
    // Pixel(x,y) = y * largeur + x
    FILE *f = fopen(filename, "wb");
    fprintf(f, "P5\n%u %u\n255\n", L, H);
    for (uint32_t y = 0; y < H; y++) {
        for (uint32_t x = 0; x < L; x++) {
            fputc((uint8_t)(y * L + x), f); 
        }
    }
    fclose(f);

    // 2. Initialisation de l'image et de l'itérateur
    Image *img = lectureImage(filename, L_mcu, H_mcu, NB_BLOCS_SUP_BLOC);
    TEST_ASSERT_NOT_NULL(img);
    
    MCU_Iterator *it = initialiser_iterateur(L_mcu, H_mcu, img);
    TEST_ASSERT_NOT_NULL(it);

    // 3. Préparation du buffer de destination (Matrice 8x8)
    uint8_t **dest = (uint8_t **)malloc(H_mcu * sizeof(uint8_t *));
    for(uint32_t i = 0; i < H_mcu; i++) {
        dest[i] = (uint8_t *)malloc(L_mcu * sizeof(uint8_t));
    }

    // --- TEST DU PREMIER MCU (Moitié gauche : x de 0 à 7) ---
    bool has_next1 = extraire_mcu(it, dest);
    TEST_ASSERT_TRUE(has_next1);
    
    // Vérification des 4 coins du MCU 1
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0, dest[0][0], "MCU1 - Coin Haut Gauche (0,0)");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(7, dest[0][7], "MCU1 - Coin Haut Droit (7,0)");
    // y=7, x=0 -> 7 * 16 + 0 = 112
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(112, dest[7][0], "MCU1 - Coin Bas Gauche (0,7)"); 
    // y=7, x=7 -> 7 * 16 + 7 = 119
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(119, dest[7][7], "MCU1 - Coin Bas Droit (7,7)");


    // --- TEST DU DEUXIÈME MCU (Moitié droite : x de 8 à 15) ---
    bool has_next2 = extraire_mcu(it, dest);
    TEST_ASSERT_TRUE(has_next2);
    
    // Vérification des 4 coins du MCU 2
    // y=0, x=8 -> 8
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(8, dest[0][0], "MCU2 - Coin Haut Gauche (8,0)");
    // y=0, x=15 -> 15
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(15, dest[0][7], "MCU2 - Coin Haut Droit (15,0)");
    // y=7, x=8 -> 7 * 16 + 8 = 120
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(120, dest[7][0], "MCU2 - Coin Bas Gauche (8,7)");
    // y=7, x=15 -> 7 * 16 + 15 = 127
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(127, dest[7][7], "MCU2 - Coin Bas Droit (15,7)");


    // --- TEST DE FIN D'IMAGE ---
    // Le troisième appel doit échouer car l'image est terminée
    bool has_next3 = extraire_mcu(it, dest);
    TEST_ASSERT_FALSE_MESSAGE(has_next3, "L'iterateur aurait du renvoyer false (fin d'image)");

    // 4. Nettoyage mémoire
    for(uint32_t i = 0; i < H_mcu; i++) {
        free(dest[i]);
    }
    free(dest);
    free(it);
    liberer_image(img, H_mcu);
    remove(filename);
}
=======
void test_iterateur_mcu_invader(void) {

    uint8_t mcu_attendu[64] = {
        0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
        0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
        0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00,
        0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0x00,
        0xff, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff
    };

    char* chemin = "images/etu/invader.pgm";

    Facteurs_echantillonnage facteurs = {1, 1, 1, 1, 1, 1};
    IterateurMCU iterateur;
    Couleur_rgb mcu[MCU_MAX][MCU_MAX];

    bool init = initialiser_iterateur_mcu(&iterateur, chemin, facteurs);

    TEST_ASSERT_EQUAL(true, init);

    bool reste_mcu = mcu_suivant(&iterateur, mcu);
    
    TEST_ASSERT_EQUAL(true, reste_mcu);
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            TEST_ASSERT_EQUAL_UINT8(mcu_attendu[i*8 + j], mcu[i][j].r);
            TEST_ASSERT_EQUAL_UINT8(mcu_attendu[i*8 + j], mcu[i][j].g);
            TEST_ASSERT_EQUAL_UINT8(mcu_attendu[i*8 + j], mcu[i][j].b);
        }
    }

    liberer_iterateur_mcu(&iterateur);
}

void test_iterateur_mcu_complexe(void) {

    Couleur_rgb mcu_attendu_1[64] = {
    };

    Couleur_rgb mcu_attendu_2[64] = {
    };

    // fichier image ppm complexe
    uint32_t largeur = 15;
    uint32_t hauteur = 10;
    char* chemin = "complexe.ppm";
    
    uint32_t L_bloc = 8;
    uint32_t H_super = 8;
    uint32_t nb_blocs = 64;

    FILE *f = fopen(chemin, "wb");
    fprintf(f, "P6\n%u %u\n255\n", largeur, hauteur);

    for (uint32_t i = 0; i < largeur * hauteur; i++) {
        fputc((uint8_t)(i % 256), f);
        fputc((uint8_t)((i + 1) % 256), f);
        fputc((uint8_t)((i + 2) % 256), f);
    }
    fclose(f);

    Facteurs_echantillonnage facteurs = {1, 1, 1, 1, 1, 1};
    IterateurMCU iterateur;
    Couleur_rgb mcu[MCU_MAX][MCU_MAX];

    bool init = initialiser_iterateur_mcu(&iterateur, chemin, facteurs);
    TEST_ASSERT_EQUAL(true, init);
    TEST_ASSERT_EQUAL_UINT32(2, iterateur.largeur_image_mcu);
    TEST_ASSERT_EQUAL_UINT32(2, iterateur.hauteur_image_mcu);
    
    bool reste_mcu = mcu_suivant(&iterateur, mcu);
    TEST_ASSERT_EQUAL(true, reste_mcu);
    
    for (uint32_t y = 0; y < 8; y++) {
        for (uint32_t x = 0; x < 8; x++) {

            uint32_t val_pixel = y * largeur + x;
            TEST_ASSERT_EQUAL_UINT8(val_pixel % 256, mcu[y][x].r);
            TEST_ASSERT_EQUAL_UINT8((val_pixel + 1) % 256, mcu[y][x].g);
            TEST_ASSERT_EQUAL_UINT8((val_pixel + 2) % 256, mcu[y][x].b);
        }
    }

    liberer_iterateur_mcu(&iterateur);


    remove(chemin);
}



>>>>>>> 705765ad79afa5a2b3f30518625fc9f274e2ebd9:tests/test_lecture.c

int main(void) {

    UNITY_BEGIN();
    RUN_TEST(test_lire_image);
    RUN_TEST(test_recupEntete_P5);
    RUN_TEST(test_lectureEblocs);
    RUN_TEST(test_lireEblocs_grande_image);
    RUN_TEST(test_lireEblocs_grande_image_P6);
<<<<<<< HEAD:tests/test_lecture_ecriture.c
    RUN_TEST(test_lireEblocs_avec_padding_P5);
    RUN_TEST(test_lireEblocs_avec_padding_P6);
    RUN_TEST(test_extraire_mcu_P5);    
=======
    RUN_TEST(test_iterateur_mcu_complexe);

>>>>>>> 705765ad79afa5a2b3f30518625fc9f274e2ebd9:tests/test_lecture.c
    return UNITY_END();
}