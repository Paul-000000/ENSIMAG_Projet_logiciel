#ifndef _LECTURE_ECRITURE_H_
#define _LECTURE_ECRITURE_H_



#include <stdio.h>
#include "rgb_to_ycbcr.h"



typedef enum{ // P6 : image couleur, P5 : image en niveaux de gris
    P5,
    P6,

} TypeImage;

typedef struct Image { // les propriétés de l'image ouverte

    uint32_t largeur;
    uint32_t hauteur;
    uint32_t debut_pixels;

    TypeImage type;
    uint8_t ** tab;
    FILE * fichier;
    uint32_t taille_ligne;
    uint32_t nb_lignes;

} Image;

typedef struct IterateurMCU { // itérateur sur la MCU en cours

    Image *image;
    uint8_t largeur_mcu, hauteur_mcu;
    uint32_t largeur_image_mcu, hauteur_image_mcu, nb_mcu_lus;
    uint32_t x, y, i_mcu;

} IterateurMCU;



Image* recupEntete (FILE *fichier);

Image *lectureImage(char *nom_fichier);

Image *allouer_image(Image *image, uint32_t largeur_bloc_en_pixels, uint32_t nb_lignes_superbloc,uint32_t nb_blocs);

Image *lireEblocs(Image *image_ppm, uint32_t x, uint32_t y,uint32_t largeur_bloc_en_pixels, uint32_t nb_lignes_superbloc,uint32_t nb_blocs);
    
void liberer_image(Image *image,uint32_t nb_lignes_superbloc);


// permet d'obtenir les dimensions du MCU en pixels à partir des facteurs
void determiner_facteurs_mcu(Facteurs_echantillonnage facteurs, uint8_t *largeur_mcu, uint8_t *hauteur_mcu); 

bool initialiser_iterateur_mcu(IterateurMCU *iterateur, char *nom_fichier, Facteurs_echantillonnage facteurs);

bool mcu_couleur_suivant(IterateurMCU *iterateur, Couleur_rgb mcu[MCU_MAX][MCU_MAX]);

bool mcu_gris_suivant(IterateurMCU *iterateur, uint8_t mcu[8][8]);

void liberer_iterateur_mcu(IterateurMCU *iterateur);

bool image_couleur(IterateurMCU *iterateur);



#endif // _LECTURE_ECRITURE_H_