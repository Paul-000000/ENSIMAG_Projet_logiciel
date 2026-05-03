#include <stdlib.h>
#include "htables.h"
#include "qtables.h"
#include "commande.h"
#include <lecture.h>


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
    Couleur_rgb mcu[MCU_MAX][MCU_MAX];

    bool init = initialiser_iterateur_mcu(&iterateur, parametres.chemin_entree, parametres.facteurs);
    if (!init) perror("erreur d'initialisation de l'itérateur de lecture\n");

    printf("dimensions d'une mcu (%dx%d)\ndimensions de l'image (%dx%d mcu)\n",iterateur.largeur_mcu, iterateur.hauteur_mcu, iterateur.largeur_image_mcu, iterateur.hauteur_image_mcu);

    uint32_t nb_mcu = 0;
    while (mcu_suivant(&iterateur, mcu)) {

        printf("mcu: %d x: %d y: %d\n", nb_mcu, iterateur.x, iterateur.y);

        for (int i = 0; i < iterateur.largeur_mcu; i++) {
            for (int j = 0; j < iterateur.hauteur_mcu; j++) {

                printf("%02x%02x%02x ",mcu[i][j].r, mcu[i][j].g ,mcu[i][j].b);
                //printf("(%3d,%3d,%3d) ",mcu[i][j].r, mcu[i][j].g ,mcu[i][j].b);
            }
            nb_mcu++;
            printf("\n");
        }
    }
    
    // lecture
    liberer_iterateur_mcu(&iterateur);

    // commande
    liberer_parametres_commande(&parametres);
    */
    return EXIT_SUCCESS;
}
