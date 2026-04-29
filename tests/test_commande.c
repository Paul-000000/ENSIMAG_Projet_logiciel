#include "unity.h"
#include "commande.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_commande_simple(void) {

    int argc = 2;
    char *argv[] = {"./ppm2jpeg","shaun_the_sheep.ppm"};

    struct parametres_commande_t parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres);

    TEST_ASSERT_EQUAL(true,res);
    TEST_ASSERT_EQUAL_STRING("shaun_the_sheep.ppm", parametres.chemin_entree);
    TEST_ASSERT_EQUAL_STRING("shaun_the_sheep.jpg",parametres.chemin_sortie);
    TEST_ASSERT_EQUAL(false,parametres.help);

    TEST_ASSERT_EQUAL_UINT8(2, parametres.facteurs.h1);
    TEST_ASSERT_EQUAL_UINT8(2, parametres.facteurs.v1);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.h2);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.v2);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.h3);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.v3);

    liberer_parametres_commande(&parametres);
}

void test_commande_outfile_arguments_inverses(void) {

    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--outfile=../chemin/complique/mouton.jpg","./images/shaun_the_sheep.ppm"};

    struct parametres_commande_t parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres);

    TEST_ASSERT_EQUAL(true,res);
    TEST_ASSERT_EQUAL_STRING("./images/shaun_the_sheep.ppm", parametres.chemin_entree);
    TEST_ASSERT_EQUAL_STRING("../chemin/complique/mouton.jpg", parametres.chemin_sortie);
    TEST_ASSERT_EQUAL(false,parametres.help);

    liberer_parametres_commande(&parametres);
}

void test_commande_help_infile(void) {

    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--help","shaun_the_sheep.ppm"};

    struct parametres_commande_t parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres);

    TEST_ASSERT_EQUAL(true,res);
    TEST_ASSERT_EQUAL(true,parametres.help);

    liberer_parametres_commande(&parametres);
} 

void test_commande_help(void) {

    int argc = 2;
    char *argv[] = {"./ppm2jpeg","--help"};

    struct parametres_commande_t parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres);

    TEST_ASSERT_EQUAL(true,res);
    TEST_ASSERT_EQUAL(true,parametres.help);

    liberer_parametres_commande(&parametres);
}

void test_commande_vide(void) {

    int argc = 1;
    char *argv[] = {"./ppm2jpeg"};

    struct parametres_commande_t parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres);

    TEST_ASSERT_EQUAL(false,res);

    liberer_parametres_commande(&parametres);
}

void test_commande_invalide(void) {

    int argc = 2;
    char *argv[] = {"./ppm2jpeg", "--truc"};

    struct parametres_commande_t parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres);

    TEST_ASSERT_EQUAL(false,res);

    liberer_parametres_commande(&parametres);
}

void test_commande_sample(void) {

    int argc = 3;
    char *argv[] = {"./ppm2jpeg","--sample=2x2,2x1,1x2","shaun_the_sheep.ppm"};

    struct parametres_commande_t parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres);

    TEST_ASSERT_EQUAL(true,res);
    TEST_ASSERT_EQUAL_STRING("shaun_the_sheep.ppm", parametres.chemin_entree);
    TEST_ASSERT_EQUAL_STRING("shaun_the_sheep.jpg", parametres.chemin_sortie);
    TEST_ASSERT_EQUAL(false,parametres.help);

    TEST_ASSERT_EQUAL_UINT8(2, parametres.facteurs.h1);
    TEST_ASSERT_EQUAL_UINT8(2, parametres.facteurs.v1);
    TEST_ASSERT_EQUAL_UINT8(2, parametres.facteurs.h2);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.v2);
    TEST_ASSERT_EQUAL_UINT8(1, parametres.facteurs.h3);
    TEST_ASSERT_EQUAL_UINT8(2, parametres.facteurs.v3);

    liberer_parametres_commande(&parametres);
}


int main(void) {

    UNITY_BEGIN();

    RUN_TEST(test_commande_simple);
    // RUN_TEST(test_commande_outfile_arguments_inverses);
    // RUN_TEST(test_commande_help_infile);
    // RUN_TEST(test_commande_help);
    // RUN_TEST(test_commande_vide);
    // RUN_TEST(test_commande_invalide);
    // RUN_TEST(test_commande_sample);
    
    return UNITY_END();
}
