
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lecture.h"


         
#define NB_BLOCS_SUPERBLOC 64
#define TAILLE_ENTETE_PPM_MIN 9



void ignore_commentaires(FILE *fichier)
{

    int c;
    while ((c = fgetc(fichier)) != EOF)
    {

        if (c == '#')
        {

            while ((c = fgetc(fichier)) != '\n' && c != EOF)
            {
            }
        }
        else
        {

            ungetc(c, fichier);
            break;
        }
    }
}

Image *recupEntete(FILE *fichier)
{

    struct Image *image = (struct Image *)malloc(sizeof(Image));
    if (image == NULL)
        return NULL;

    image->tab = NULL;

    char c1, c2;
    unsigned int l, h, profondeur;

    ignore_commentaires(fichier);

    if (fscanf(fichier, "%c%c\n", &c1, &c2) != 2)
    { // P5 ou P6
        return NULL;
    }
    else
    {

        if (c1 == 'P' && c2 == '5')
        {
            image->type = P5;
        }
        else if (c1 == 'P' && c2 == '6')
        {
            image->type = P6;
        }
        else
        {
            return NULL;
        }
    }

    ignore_commentaires(fichier);

    if (fscanf(fichier, "%u %u\n", &l, &h) != 2)
    { // largeur et hauteur
        return NULL;
    }

    image->largeur = l;
    image->hauteur = h;

    ignore_commentaires(fichier);

    if (fscanf(fichier, "%u\n", &profondeur) != 1)
    {
        return NULL;
    }

    ignore_commentaires(fichier);

    image->debut_pixels = ftell(fichier);
    image->fichier = fichier;
    return image;
}

void positionner_curseur(Image *img, uint32_t x, uint32_t y)
{
    int multiplicateur = (img->type == P6) ? 3 : 1;
    long position = img->debut_pixels + (((long)y * img->largeur) + (long)x) * multiplicateur;
    fseek(img->fichier, position, SEEK_SET);
}

Image *lectureImage(char *nom_fichier,uint32_t largeur_bloc_en_pixels, uint32_t nb_lignes_superbloc,uint32_t nb_blocs)
{

    FILE *fichier = fopen(nom_fichier, "rb");
    if (fichier == NULL)
    {
        perror("[ERREUR] ouverture du fichier");
        return NULL;
    }
    fseek(fichier, 0, SEEK_END);
    long taille_fichier = ftell(fichier);
    if (taille_fichier < TAILLE_ENTETE_PPM_MIN)
    {
        printf("[ERREUR] taille de fichier inferieur a l'entete");
        return NULL;
    }

    rewind(fichier);

        rewind(fichier);

        Image * image = recupEntete(fichier); 
        image->tab = (uint8_t **)malloc(nb_lignes_superbloc * sizeof(uint8_t *));
        int multiplicateur = (image->type == P6) ? 3 : 1;
        for (uint32_t i = 0; i < nb_lignes_superbloc; i++) {
            image->tab[i] = (uint8_t *)malloc(largeur_bloc_en_pixels * nb_blocs * multiplicateur * sizeof(uint8_t));
        }
        return image;


}

Image *lireEblocs(Image *image_ppm, uint32_t x, uint32_t y, uint32_t largeur_bloc_en_pixels, uint32_t nb_lignes_superbloc,uint32_t nb_blocs)
{
    uint32_t nb_pixels_a_lire = largeur_bloc_en_pixels * nb_blocs;

    if (x + nb_pixels_a_lire > image_ppm->largeur) {
        nb_pixels_a_lire = image_ppm->largeur - x;
    }

    uint32_t unite = (image_ppm->type == P6) ? 3 : 1;
    
    for (size_t i = 0; i < nb_lignes_superbloc; i++)
    {
        positionner_curseur(image_ppm, x, y + i);

        size_t lus = fread(image_ppm->tab[i], unite, nb_pixels_a_lire, image_ppm->fichier);
        if (i == 0)
        {
            image_ppm->taille_ligne = lus;
        }
        if (lus < nb_pixels_a_lire)
        {
            break;
        }
        image_ppm->nb_lignes = i + 1;
    }

    return image_ppm;
}

void liberer_image(Image *image,uint32_t nb_lignes_superbloc) {

    if (image == NULL)
        return;

    if (image->tab != NULL) {
        for (uint32_t i = 0; i < nb_lignes_superbloc; i++) {
        
        free(image->tab[i]);

        }

        free(image->tab);
    
    fclose(image->fichier);
    free(image);
    }
}



void determiner_facteurs_mcu(Facteurs_echantillonnage facteurs, uint8_t *largeur_mcu, uint8_t *hauteur_mcu) {

	*largeur_mcu = 8 * facteurs.h1;
	*hauteur_mcu = 8 * facteurs.v1;
}


//void superbloc_suivant(Image *image_ppm) {

    //NB_BLOCS_SUPERBLOC
//}
