#include "rgb_to_ycbcr.h"


Couleur_ycbcr rgb_to_ycbcr(Couleur_rgb couleur) {

    Couleur_ycbcr resultat = {
                tab_Y_R[couleur.r]  + tab_Y_G[couleur.g]  + tab_Y_B[couleur.b],
                tab_Cb_R[couleur.r] + tab_Cb_G[couleur.g] + (couleur.b >> 1) + 128,
                (couleur.r >> 1)    + tab_Cr_G[couleur.g] + tab_Cr_B[couleur.b] + 128
            };
    return resultat;
}



void matrice_rgb_to_ycbcr(const Couleur_rgb matrice[MCU_MAX][MCU_MAX], uint8_t largeur, uint8_t hauteur, Couleur_ycbcr matrice_sortie[MCU_MAX][MCU_MAX]) {

    for (uint8_t i = 0; i < hauteur; i++) {
        
        for (uint8_t j = 0; j < largeur; j++) {
            
            matrice_sortie[i][j] = rgb_to_ycbcr(matrice[i][j]);
        }
    }
}