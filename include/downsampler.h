#ifndef _DOWNSAMPLER_H_
#define _DOWNSAMPLER_H_

#define MCU_MAX 32

#include "rgb_to_ycbcr.h"
#include "commande.h"

enum composante_t {
    Y,
    CB,
    CR
};

struct vecteur_t {
    
    uint8_t valeur[64];
    enum composante_t composante;
};

struct matrices_ycbcr_t {
    
    uint8_t matrice_y[MCU_MAX][MCU_MAX];
    uint8_t matrice_cb[MCU_MAX][MCU_MAX];
    uint8_t matrice_cr[MCU_MAX][MCU_MAX];
};

struct vecteurs_ycbcr_t {
    
    uint8_t nb_vecteurs;
    struct vecteur_t vecteurs[16];
};

struct dimensions_cbcr_t {
    
    uint8_t largeur_mcu_cb, hauteur_mcu_cb, largeur_mcu_cr, hauteur_mcu_cr;
};

struct dimensions_cbcr_t determiner_dimensions_cb_cr(struct facteurs_echantillonnage_t facteurs);

void decouper_matrices(struct couleur_ycbcr_t matrice[MCU_MAX][MCU_MAX], uint8_t largeur_mcu, uint8_t hauteur_mcu, struct dimensions_cbcr_t dimensions_sortie, struct matrices_ycbcr_t *matrices_sortie);

void determiner_facteurs_mcu(struct facteurs_echantillonnage_t facteurs, uint8_t *largeur_mcu, uint8_t *hauteur_mcu);

#endif // _DOWNSAMPLER_H_