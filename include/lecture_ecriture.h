#ifndef _LECTURE_ECRITURE_H_
#define _LECTURE_ECRITURE_H_
#include <stdio.h>
#include "rgb_to_ycbcr.h"
#include <stdbool.h>

#define NB_BLOCS_SUP_BLOC 64         

typedef enum{
    P5,
    P6,

}TypeImage;


typedef struct Image {


    uint32_t largeur;
    uint32_t hauteur;
    uint32_t debut_pixels;
    TypeImage type;
    FILE * fichier;

    // information sur le superbloc
    uint8_t ** tab;
    uint32_t taille_ligne; // en pixel
    uint32_t nb_lignes; // en pixel

}Image;

typedef struct {
    uint32_t x_courant;
    uint32_t y_courant;      
    

    uint32_t largeur_mcu;     
    uint32_t hauteur_mcu;     

    Image *image;             
} MCU_Iterator;


Image* recupEntete (FILE *fichier);

Image *lectureImage(char *nom_fichier,uint32_t largeur_bloc_en_pixels,
    uint32_t nb_lignes_superbloc,uint32_t nb_blocs);

Image *lireEblocs(Image *image_ppm, uint32_t x, uint32_t y,uint32_t largeur_bloc_en_pixels,
    uint32_t nb_lignes_superbloc,uint32_t nb_blocs);

void liberer_image(Image *image,uint32_t nb_lignes_superbloc);
bool extraire_mcu(MCU_Iterator *it, uint8_t **dest) ;
MCU_Iterator * initialiser_iterateur( uint32_t largeur_mcu,uint32_t hauteur_mcu, Image *image);

#endif // _LECTURE_ECRITURE_H_