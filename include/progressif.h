#ifndef _PROGRESSIF_H_
#define _PROGRESSIF_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct Fichier_tmp
{
    FILE * fichier_Y;
    FILE * fichier_Cb;
    FILE * fichier_Cr;
    bool couleur;
    
}Fichier_tmp;


Fichier_tmp * creer_fichier_tmp();
void ecrire_bloc(Fichier_tmp *tmp,uint16_t bloc[64],Composante type_bloc);
void lire_bloc(Fichier_tmp *tmp,uint16_t bloc[64],Composante type_bloc);


#endif /* _PROGRESSIF_H_ */