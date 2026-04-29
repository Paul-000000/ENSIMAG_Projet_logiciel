#include <stdlib.h>
#include "htables.h"
#include "qtables.h"
#include "commande.h"
#include <lecture_ecriture.h>

void test_recupEntete_P5(void) {

    const char* filename = "/Users/test/Desktop/1A/S2/Projet_C/2_bahag_becharam_bottnerp/images/etu/gris.pgm";
 
    FILE *f_test = fopen(filename, "rb");
    if (f_test!=NULL)
    {
        printf("chemin trouve\n");
        Image* img = recupEntete(f_test);

    if (img!=NULL)
    {
        printf("pass1\n");
    }else
    {
        printf("NULL\n");
    }
    

    if (img->type==P5)
    {
        printf("pass2\n");
    }
    
    if (img->largeur==320)
    {
         printf("pass3\n");
    }

    if (img->hauteur==320)
    {
        printf("pass4\n");
    }
    fseek(f_test, img->debut_pixels, SEEK_SET);
    int first_pixel = fgetc(f_test);
    // TEST_ASSERT_EQUAL_INT(42, first_pixel);
    
         printf("debut pixels %d\n",img->debut_pixels);

    fclose(f_test);

    free(img); 


    }else
    {
        printf("erreur\n");

    }
    
    

   
    
    // TEST_ASSERT_NOT_NULL(img);
    // TEST_ASSERT_EQUAL_INT(P5, img->type);
    // TEST_ASSERT_EQUAL_UINT32(320, img->largeur);
    // TEST_ASSERT_EQUAL_UINT32(240, img->hauteur);
    

    
    
    
    

    
   
}


int main(int argc, char **argv)
{

    test_recupEntete_P5();
   
    // struct parametres_commande_t parametres;

    // bool res = initialiser_parametres_commande(argc, argv, &parametres);
    // if (!res) return EXIT_FAILURE;

    // bool help = help_demande(&parametres);
    // if (help) return EXIT_SUCCESS;
    

    // // suite des fonctions

    // liberer_parametres_commande(&parametres);

    return EXIT_SUCCESS;
}
