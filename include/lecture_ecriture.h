#ifndef _LECTURE_ECRITURE_H_
#define _LECTURE_ECRITURE_H_
#include <stdio.h>
#include "rgb_to_ycbcr.h"

typedef enum{
    P5,
    P6,

}TypeImage;

typedef struct Image {

    uint32_t largeur;
    uint32_t hauteur;
    uint32_t debut_pixels;

    TypeImage type;
    uint8_t ** tab;
    FILE * fichier;
    uint32_t taille_ligne;
    uint32_t nb_lignes;

}Image;


Image* recupEntete (FILE *fichier);

Image *lectureImage(char *nom_fichier,uint32_t largeur_bloc_en_pixels, uint32_t nb_lignes_superbloc,uint32_t nb_blocs);

Image *lireEblocs(Image *image_ppm, uint32_t x, uint32_t y,uint32_t largeur_bloc_en_pixels, uint32_t nb_lignes_superbloc,uint32_t nb_blocs);
    
void liberer_image(Image *image,uint32_t nb_lignes_superbloc);

// permet d'obtenir les dimensions du MCU en pixels à partir des facteurs
void determiner_facteurs_mcu(Facteurs_echantillonnage facteurs, uint8_t *largeur_mcu, uint8_t *hauteur_mcu); 



#endif // _LECTURE_ECRITURE_H_