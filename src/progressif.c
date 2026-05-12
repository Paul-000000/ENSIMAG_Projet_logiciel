#include <progressif.h>
#include <stdlib.h>
#include <stdbool.h>
#include <downsampler.h>

Fichier_tmp * creer_fichier_tmp(bool couleur){

    Fichier_tmp *fichier=(Fichier_tmp*)malloc(sizeof(Fichier_tmp));
    if (fichier==NULL) return NULL;

    fichier->fichier_Y=tmpfile();
    if (fichier->fichier_Y==NULL)
    {
        perror("ERREUR lors de la creation du fichier temporaire");  
        return NULL  ;
    }
    
    if (couleur)
    {
        fichier->fichier_Cb=tmpfile();
        if (fichier->fichier_Cb==NULL)
        {
            perror("ERREUR lors de la creation du fichier temporaire");  
            return NULL  ;
        }
       
        fichier->fichier_Cr=tmpfile();
        if (fichier->fichier_Cr==NULL)
        {
            perror("ERREUR lors de la creation du fichier temporaire");  
            return NULL  ;
        }
        
        fichier->couleur=true;
        return fichier;

    }
    
    fichier->fichier_Cb=NULL;
    fichier->fichier_Cr=NULL;
    fichier->couleur=false;
    return fichier;
   
 }

 void ecrire_bloc(Fichier_tmp *tmp,uint16_t bloc[64],Composante type_bloc){

    switch (type_bloc)
    {
    case Y:
        if (fwrite(bloc,2,64,tmp->fichier_Y)!=64)
        {
            perror("ERREUR lors de l'ecriture dans le fichier temporaire");
        }
        
        break;
    case CB:
        if (fwrite(bloc,2,64,tmp->fichier_Cb)!=64)
        {
            perror("ERREUR lors de l'ecriture dans le fichier temporaire");
        }
        break;
    
    default:
        if (fwrite(bloc,2,64,tmp->fichier_Cr)!=64)
        {
            perror("ERREUR lors de l'ecriture dans le fichier temporaire");
        }
        break;
    }

 
}

void lire_bloc(Fichier_tmp *tmp,uint16_t bloc[64],Composante type_bloc){

    switch (type_bloc)
    {
    case Y:
        if (fread(bloc,2,64,tmp->fichier_Y)!=64)
        {
            perror("ERREUR lors de lecture a partir du fichier temporaire");
        }
        
        break;
    case CB:
        if (fread(bloc,2,64,tmp->fichier_Cb)!=64)
        {
            perror("ERREUR lors de lecture a partir du fichier temporaire");
        }
        break;
    
    default:
        if (fread(bloc,2,64,tmp->fichier_Cr)!=64)
        {
            perror("ERREUR lors de lecture a partir du fichier temporaire");
        }
        break;
    }
}

void liberer_fichiers_tmp(Fichier_tmp *f){
    if (f == NULL) return;

    if (f->fichier_Y != NULL) fclose(f->fichier_Y);
    
    if (f->couleur)
    {
        if (f->fichier_Cb != NULL) fclose(f->fichier_Cb);
        if (f->fichier_Cr != NULL) fclose(f->fichier_Cr);
    }
    free(f);
}