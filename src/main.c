#include <stdlib.h>
#include "commande.h"
#include "lecture.h"
#include "rgb_to_ycbcr.h"
#include "downsampler.h"
#include "dct.h"
#include "zz_quant.h"


int main(int argc, char **argv) {

    /*
    // commande
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres);
    if (!res) return EXIT_FAILURE;

    bool help = help_demande(&parametres);
    if (help) return EXIT_SUCCESS;

    // lecture
    IterateurMCU iterateur;
    
    bool init = initialiser_iterateur_mcu(&iterateur, parametres.chemin_entree, parametres.facteurs);
    if (!init) perror("erreur d'initialisation de l'itérateur de lecture\n");

    //printf("dimensions d'une mcu (%dx%d)\ndimensions de l'image (%dx%d) (%dx%d mcu)\n",iterateur.largeur_mcu, iterateur.hauteur_mcu, iterateur.image->largeur,iterateur.image->hauteur, iterateur.largeur_image_mcu, iterateur.hauteur_image_mcu);

    bool reste_mcu;
    Couleur_rgb mcu[MCU_MAX][MCU_MAX];
    Couleur_ycbcr mcu_ycbcr[MCU_MAX][MCU_MAX];
    Vecteurs_ycbcr vecteurs;
    //uint32_t nb_mcu = 0;
    while (true) {

        //printf("mcu: %d x: %d y: %d\n", nb_mcu, iterateur.x, iterateur.y);
        reste_mcu = mcu_suivant(&iterateur, mcu);
        if (!reste_mcu) break;

        // for (int i = 0; i < iterateur.largeur_mcu; i++) {
        //     for (int j = 0; j < iterateur.hauteur_mcu; j++) {

        //         printf("%02x%02x%02x ",mcu[i][j].r, mcu[i][j].g ,mcu[i][j].b);
        //         //printf("(%3d,%3d,%3d) ",mcu[i][j].r, mcu[i][j].g ,mcu[i][j].b);
        //     }
        //     printf("\n");
        // }
        // nb_mcu++;

        Dimensions_cbcr dim_cbcr = determiner_dimensions_cb_cr(parametres.facteurs);
        matrice_rgb_to_ycbcr(mcu, iterateur.largeur_mcu, iterateur.hauteur_mcu, mcu_ycbcr);

        decouper_matrices(mcu_ycbcr, iterateur.largeur_mcu, iterateur.hauteur_mcu, dim_cbcr, &vecteurs);

        init_table_cosinus();

        for (uint8_t i = 0; i < vecteurs.nb_vecteurs; i++) {

            Vecteur vec = vecteurs.vecteurs[i];
            int16_t bloc_spatial[64], bloc_frequentiel[64];
            
            for (uint8_t j = 0; j <64; j++) bloc_spatial[j] = vec.valeur[j];
            applique_dct(bloc_spatial ,bloc_frequentiel);

            zigzag(bloc_frequentiel);

            quantification(bloc_frequentiel, vec.composante);
        }
    }
    
    
    // lecture
    liberer_iterateur_mcu(&iterateur);

    // commande
    liberer_parametres_commande(&parametres);

    */

    return EXIT_SUCCESS;
}
