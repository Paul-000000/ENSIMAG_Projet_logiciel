
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lecture_ecriture.h"

#define TAILLE_ENTETE_PPM 8

Image* recupEntete (FILE *fichier){
    struct Image* image;
    char c1,c2;
    unsigned int l,h;
    if(fscanf(fichier, "%c%c\n %l %h", &c1,&c2, &l,&h)==4){
        if(c1=="P" && c2=="5") { 
            image->type=P5;
        }
        else if (c1=="P" && c2=="6"){
             image->type=P6;
            }
        image->largeur = l;
        image->hauteur = h;
    }
    image->tab = NULL;
    image->fichier = fichier;
    return image;
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

        FILE *fichier = fopen(nom_fichier, "r");
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

 struct Image* lireEblocs(struct Image *image_ppm){

        int taille_ligne;

        
        if (image_ppm->type==P5 && image_ppm->largeur>=64*8)
        {
        taille_ligne=64*8;
        }else
        {
            taille_ligne=image_ppm->largeur;
        }
        
        // else
        // {
        //     taille_ligne=image_ppm->largeur*3;
        // }

        for (int i = 0,index_tab=0; i < 8; index_tab+=taille_ligne,i++)
        {
            size_t lus = fread(&(image_ppm->tab[index_tab]), 1, taille_ligne, image_ppm->fichier);
            
            if (lus<taille_ligne)
            {
              printf("[ERREUR] lecture de ligne ");
            }
            

        }

        if (image_ppm->hauteur %8!=0 || image_ppm->largeur%8 !=0 )
        {
            completer_image(image_ppm->fichier,image_ppm,taille_ligne);
        }
    }

