
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lecture_ecriture.h"

#define TAILLE_ENTETE_PPM 8


Image* recupEntete (FILE *fichier){

    struct Image* image = (struct Image *)malloc(sizeof(Image));
    if (image == NULL) return NULL;

    char c1,c2;
    unsigned int l,h, profondeur;

    if(fscanf(fichier, "%c%c\n%u %u\n%u", &c1, &c2, &l, &h, &profondeur) == 5){
        
        if(c1=="P" && c2=="5") { 
            image->type=P5;
        }
        else if (c1=="P" && c2=="6"){
            image->type=P6;
        }
        else {
            return NULL;
        }

        image->largeur = l;
        image->hauteur = h;
    
    } else {
        return NULL;
    }

    image->tab = NULL;
    image->debut_pixels = ftell(fichier);
    image->fichier = fichier;
    return image;
}


void positionner_curseur(Image *img, uint32_t x, uint32_t y) {
    // La formule magique pour retrouver n'importe quel pixel (x,y) :
    long position = img->debut_pixels + ( (long)y * img->largeur ) + (long)x;
    fseek(img->fichier, position, SEEK_SET);
}


void completer_image(FILE *fichier,Image *image_ppm,int taille_ligne){

    if (image_ppm->largeur%8 !=0)
    {
       for (int i = 0,index_tab=taille_ligne; i < image_ppm->hauteur; index_tab+=taille_ligne,i++){
        if (image_ppm->type==P5)
        {
            uint8_t dernierPixel=image_ppm->tab[index_tab-1];
            int j=index_tab;
            while (j%8!=0)
            {
                image_ppm->tab[j]=dernierPixel;
                j++;
            }    
        }
        else
        { uint8_t dernierB=image_ppm->tab[index_tab-1];
          uint8_t dernierG=image_ppm->tab[index_tab-2];
          uint8_t dernierR=image_ppm->tab[index_tab-3];
          int j=index_tab;
          while (j%8!=0)
          {
            image_ppm->tab[index_tab]=dernierR;
            image_ppm->tab[index_tab+1]=dernierG;
            image_ppm->tab[index_tab+2]=dernierB;
            j+=3;
          }
            
        }
       }
    }else
    {
        int index_tab=image_ppm->hauteur;
        while (index_tab%8==0)
        {
        }       
    }   

}

Image* lectureImage(char * nom_fichier ){

        FILE *fichier = fopen(nom_fichier, "rb");
        if (fichier==NULL)
        {
            perror("[ERREUR] ouverture du fichier");
            return NULL;
        }
        fseek(fichier, 0, SEEK_END);
        long taille_fichier = ftell(fichier);
        if (taille_fichier<TAILLE_ENTETE_PPM)
        {
        printf("[ERREUR] taille de fichier inferieur a l'entete");
        return NULL;  
        }


        rewind(fichier);

        return recupEntete(fichier); 

}


 Image* lireEblocs(Image *image_ppm,uint32_t x, uint32_t y){


        int taille_ligne=image_ppm->largeur;
        int nb_octets_lire;
    
        if (image_ppm->type==P5)
        {
           nb_octets_lire = 512; 
        if (x + nb_octets_lire > taille_ligne) {
            nb_octets_lire = taille_ligne - x;
        }
        }

       int index_tab =0; 
       for (size_t i = 0; i < 8; i++)
       {
          positionner_curseur(image_ppm, x, y + i);

        size_t lus = fread(&(image_ppm->tab[index_tab]), 1, nb_octets_lire, image_ppm->fichier);
        if (i==0)
        {
            image_ppm->taille_ligne=lus;
        }
        if (lus<nb_octets_lire)
        {
            break;
        }
        
    
         index_tab+=nb_octets_lire;
       }    
        
       return image_ppm;

}



