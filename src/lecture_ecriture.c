
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "lecture_ecriture.h"

#define LARGEUR_BLOC 8          
#define NB_LIGNES_IMAGE_TAB 8   
#define LARGEUR_SUPER_BLOC (LARGEUR_BLOC * NB_BLOCS_LUS) 
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


Image *lectureImage(char *nom_fichier,uint32_t largeur_bloc_en_pixels,
    uint32_t nb_lignes_superbloc,uint32_t nb_blocs)
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
        for (int i = 0; i < nb_lignes_superbloc; i++) {
            image->tab[i] = (uint8_t *)malloc(largeur_bloc_en_pixels * nb_blocs * multiplicateur * sizeof(uint8_t));
        }
        return image;


}

void completer_sup_blocs(Image *image_ppm, uint32_t largeur_bloc_en_pixels,
    uint32_t nb_lignes_superbloc){

       
           while (image_ppm->taille_ligne%largeur_bloc_en_pixels!=0)
           {
            for (size_t i = 0; i < image_ppm->nb_lignes; i++)
            { 
                if (image_ppm->type==P5)
                {
                image_ppm->tab[i][image_ppm->taille_ligne]=image_ppm->tab[i][image_ppm->taille_ligne-1];
                }
                else
                {
                    image_ppm->tab[i][image_ppm->taille_ligne*3]=image_ppm->tab[i][(image_ppm->taille_ligne-1)*3];
                    image_ppm->tab[i][image_ppm->taille_ligne*3+1]=image_ppm->tab[i][(image_ppm->taille_ligne-1)*3+1];
                    image_ppm->tab[i][image_ppm->taille_ligne*3+2]=image_ppm->tab[i][(image_ppm->taille_ligne-1)*3+2];
                }         
            }
            image_ppm->taille_ligne++;

           }
           while (image_ppm->nb_lignes<nb_lignes_superbloc)
           {    uint32_t nb_octets_par_ligne= (image_ppm->type == P6) ? 3*image_ppm->taille_ligne : image_ppm->taille_ligne;

                for (size_t i = 0; i <nb_octets_par_ligne ; i++)
                {
                    image_ppm->tab[image_ppm->nb_lignes][i]=image_ppm->tab[image_ppm->nb_lignes-1][i];
                }
                image_ppm->nb_lignes++;
                
           }
}



Image *lireEblocs(Image *image_ppm, uint32_t x, uint32_t y,uint32_t largeur_bloc_en_pixels,
    uint32_t nb_lignes_superbloc,uint32_t nb_blocs)
{

   
    uint32_t nb_pixels_a_lire = largeur_bloc_en_pixels * nb_blocs;

    if (x + nb_pixels_a_lire > image_ppm->largeur) {
        nb_pixels_a_lire = image_ppm->largeur - x;
    }

    uint32_t unite = (image_ppm->type == P6) ? 3 : 1;
    


    for (size_t i = 0; i < nb_lignes_superbloc; i++)
    {
        positionner_curseur(image_ppm, x, y + i);
        if (y + i >= image_ppm->hauteur) break;

        size_t lus = fread(image_ppm->tab[i], unite, nb_pixels_a_lire, image_ppm->fichier);
        if (i == 0)
        {
            image_ppm->taille_ligne = lus ;
        }
        image_ppm->nb_lignes = i + 1;


    }
     completer_sup_blocs(image_ppm, largeur_bloc_en_pixels,
     nb_lignes_superbloc);

    return image_ppm;
}

void liberer_image(Image *image,uint32_t nb_lignes_superbloc) {

    if (image == NULL)
        return;

    if (image->tab != NULL) {
        for (int i = 0; i < nb_lignes_superbloc; i++) {
        
        free(image->tab[i]);

        }

        free(image->tab);
    
    
    }
    fclose(image->fichier);
    free(image);

}

MCU_Iterator * initialiser_iterateur( uint32_t largeur_mcu,uint32_t hauteur_mcu, Image *image){
    
   MCU_Iterator * it=(MCU_Iterator *)malloc(sizeof(MCU_Iterator));
   it->image=image;
   it->y_courant=it->x_courant=0;
   it->hauteur_mcu=hauteur_mcu;
   it->largeur_mcu=largeur_mcu;
    
    return it ;

}



bool extraire_mcu(MCU_Iterator *it, uint8_t **dest) {
    Image *img = it->image;
    uint32_t unite = (img->type == P6) ? 3 : 1;

    // Si on dépasse la largeur, on doit charger la bande suivante
    if (it->x_courant >= img->largeur) {
        it->x_courant = 0;
        it->y_courant += it->hauteur_mcu;
        if (it->y_courant >= img->hauteur) return false;

        // On charge une bande de la hauteur d'un MCU
        lireEblocs(img, 0, it->y_courant, it->largeur_mcu, it->hauteur_mcu, NB_BLOCS_SUP_BLOC);

    }else if(it->x_courant%(NB_BLOCS_SUP_BLOC*it->largeur_mcu)==0) // si on arrive a la fin du super bloc mais non pas la fin de la ligne de l'image 
                                                    // on charge le prochain super bloc
    {
        lireEblocs(img, it->x_courant, it->y_courant, it->largeur_mcu, it->hauteur_mcu, NB_BLOCS_SUP_BLOC);
    }
    
    uint32_t largeur_sup_bloc=it->largeur_mcu*NB_BLOCS_SUP_BLOC;
    for (uint32_t i = 0; i < it->hauteur_mcu; i++) {
        for (uint32_t j = 0; j < it->largeur_mcu; j++) {
         uint32_t index_colonne_sup_bloc=(it->x_courant+j)%(largeur_sup_bloc);

         if (it->image->type==P5)
         {
             dest[i][j]=it->image->tab[i][index_colonne_sup_bloc];
             
            }
            else if (it->image->type==P6)
            {
             dest[i][j*3]=it->image->tab[i][index_colonne_sup_bloc*3];
             dest[i][j*3+1]=it->image->tab[i][index_colonne_sup_bloc*3+1];
             dest[i][j*3+2]=it->image->tab[i][index_colonne_sup_bloc*3+2];
                
         }  
        
        }
    }
    it->x_courant+=it->largeur_mcu;
    return true;
}
