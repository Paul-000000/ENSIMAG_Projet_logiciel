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
Image* lectureImage(char * nom_fichier );

Image* lireEblocs(Image *image,uint32_t x, uint32_t y);
void liberer_image(Image *image);



#endif // _LECTURE_ECRITURE_H_