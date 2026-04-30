#include "rgb_to_ycbcr.h"


struct couleur_ycbcr_t rgb_to_ycbcr(struct couleur_rgb_t couleur) {

    struct couleur_ycbcr_t resultat = {
        0.299   * couleur.r +   0.587   * couleur.g + 0.114     * couleur.b,
        -0.1687 * couleur.r -   0.3313  * couleur.g + 0.5       * couleur.b + 128,
        0.5     * couleur.r -   0.4187  * couleur.g - 0.0813    * couleur.b + 128
    };

    return resultat;
}

void matrice_rgb_to_ycbcr(struct couleur_rgb_t matrice[MCU_MAX][MCU_MAX], uint8_t largeur, uint8_t hauteur, struct couleur_ycbcr_t matrice_sortie[MCU_MAX][MCU_MAX]) {

    for (uint8_t i = 0; i < hauteur; i++) {
        
        for (uint8_t j = 0; j < largeur; j++) {
            
            matrice_sortie[i][j] = rgb_to_ycbcr(matrice[i][j]);
        }
    }
}