#ifndef _LECTURE_ECRITURE_H_
#define _LECTURE_ECRITURE_H_
#include <stdio.h>
#include "rgb_to_ycbcr.h"

typedef enum{
    P5,
    P6,

}TypeImage;

typedef struct Image {
    u_int32_t largeur;
    u_int32_t hauteur;
    TypeImage type;
    uint8_t * tab;
    FILE * fichier;
    

    
}Image;

Image* recupEntete (FILE *fichier);
Image* lectureImage(char * nom_fichier );
Image* lireEblocs(Image *image);


#endif // _LECTURE_ECRITURE_H_