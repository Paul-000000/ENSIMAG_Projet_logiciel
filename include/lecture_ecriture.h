#ifndef _LECTURE_ECRITURE_H_
#define _LECTURE_ECRITURE_H_
#include <stdio.h>
#include "rgb_to_ycbcr.h"

typedef enum{
    P5,
    P6,

}TypeImage;

typedef struct Image {
    int largeur;
    int hauteur;
    TypeImage type;
    couleur_rgb_t * tab;
    
};

Image*  recupEntete (FILE *fichier);
Image* lectureImage (char * nom_fichier );


#endif // _LECTURE_ECRITURE_H_