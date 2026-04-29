
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lecture_ecriture.h"

#define TAILLE_ENTETE_PPM 8

Image *recupEntete(FILE *fichier)
{


    struct Image *image = (struct Image *)malloc(sizeof(Image));
    if (image == NULL)
        return NULL;


    char c1, c2;
    unsigned int l, h, profondeur;

   

    if(fscanf(fichier, "%c%c\n%u %u\n%u\n", &c1, &c2, &l, &h, &profondeur) == 5){
        image->tab=NULL;
        if(c1=='P' && c2=='5') { 
            image->type=P5;
        }
        else if (c1=='P' && c2=='6'){
            image->type=P6;
   }
        else
        {
            return NULL;
        }

        image->largeur = l;
        image->hauteur = h;
    }

    // image->tab = NULL;
    image->debut_pixels = ftell(fichier);
    image->fichier = fichier;
    return image;
}

void positionner_curseur(Image *img, uint32_t x, uint32_t y)
{
    // La formule magique pour retrouver n'importe quel pixel (x,y) :
    long position = img->debut_pixels + ((long)y * img->largeur) + (long)x;
    fseek(img->fichier, position, SEEK_SET);
}


Image *lectureImage(char *nom_fichier)
{

    FILE *fichier = fopen(nom_fichier, "rb");
    if (fichier == NULL)
    {
        perror("[ERREUR] ouverture du fichier");
        return NULL;
    }
    fseek(fichier, 0, SEEK_END);
    long taille_fichier = ftell(fichier);
    if (taille_fichier < TAILLE_ENTETE_PPM)
    {
        printf("[ERREUR] taille de fichier inferieur a l'entete");
        return NULL;
    }


        rewind(fichier);

        Image * image = recupEntete(fichier); 
        image->tab = (uint8_t **)malloc(8 * sizeof(uint8_t *));
    for (int i = 0; i < 8; i++) {
        // Chaque ligne peut contenir jusqu'à 512 pixels (64 blocs)
        image->tab[i] = (uint8_t *)malloc(512 * sizeof(uint8_t));
    }
    return image;


}

Image *lireEblocs(Image *image_ppm, uint32_t x, uint32_t y)
{

    int taille_ligne = image_ppm->largeur;
    int nb_octets_lire;

    if (image_ppm->type == P5)
    {
        nb_octets_lire = 512;
        if (x + nb_octets_lire > taille_ligne)
        {
            nb_octets_lire = taille_ligne - x;
        }
    }

    int index_tab = 0;
    for (size_t i = 0; i < 8; i++)
    {
        positionner_curseur(image_ppm, x, y + i);

        size_t lus = fread(image_ppm->tab[i], 1, nb_octets_lire, image_ppm->fichier);
        if (i == 0)
        {
            image_ppm->taille_ligne = lus;
        }
        if (lus < nb_octets_lire)
        {
            break;
        }

        index_tab += nb_octets_lire;
    }

    return image_ppm;
}

void liberer_image(Image *image) {

    if (image == NULL) return;

    if (image->tab != NULL) {
        for (int i = 0; i < 8; i++) {
        
        free(image->tab[i]);

        }

        free(image->tab);
    }
    fclose(image->fichier);
    free(image);
}