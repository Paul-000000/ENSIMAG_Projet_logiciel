#ifndef _DOWNSAMPLER_H_
#define _DOWNSAMPLER_H_

#define MCU_MAX 32

#include "rgb_to_ycbcr.h"
#include "commande.h"

typedef enum composante {
    Y,
    CB,
    CR
} Composante;

typedef struct vecteur {
    
    uint8_t valeur[64];
    Composante composante;
} Vecteur;

typedef struct vecteurs_ycbcr {
    
    uint8_t nb_vecteurs;
    Vecteur vecteurs[16];
} Vecteurs_ycbcr;

typedef struct dimensions_cbcr {
    
    uint8_t largeur_mcu_cb, hauteur_mcu_cb, largeur_mcu_cr, hauteur_mcu_cr;
} Dimensions_cbcr;



Dimensions_cbcr determiner_dimensions_cb_cr(Facteurs_echantillonnage facteurs);

void decouper_matrices(Couleur_ycbcr matrice[MCU_MAX][MCU_MAX], uint8_t largeur_mcu, uint8_t hauteur_mcu, Dimensions_cbcr dimensions_sortie, Vecteurs_ycbcr *vecteurs_sortie);

void determiner_facteurs_mcu(Facteurs_echantillonnage facteurs, uint8_t *largeur_mcu, uint8_t *hauteur_mcu);


#endif // _DOWNSAMPLER_H_