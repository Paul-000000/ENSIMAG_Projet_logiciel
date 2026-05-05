#include <ecriture_entete.h>
#include <string.h>



#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <zz_quant.h>

#define MARQUEUR_APP0 0xe0
#define SOI 0xd8
#define MARQEUR_DQT 0xdb


bool ecrire_SOI(FILE *f){
    if (f==NULL)
    {
       return false;
    }
    
    uint8_t soi[2];
    soi[0]=0xff;
    soi[1]=SOI;
    if (fwrite(&soi, 1, 2, f) != 2) {
        return false;
    }
    return true;
}

bool ecrire_appx(FILE *f) {
    if (f == NULL) {
        return false;
    }
    
    uint8_t buf[18];
    buf[0] = 0xff;
    buf[1] = MARQUEUR_APP0;

    buf[2] = 0;
    buf[3] = 16;

    memcpy(&buf[4], "JFIF\0", 5);

    buf[9] = 1;
    buf[10] = 1;

    for (int i = 11; i < 18; i++) {
        buf[i] = 0;
    }

    if (fwrite(buf, 1, 18, f) != 18) {
        return false;
    }

    return true;
}

// bool ecrire_commentaire()

bool ecrire_DQT(FILE * f,uint8_t tab_quantif[64],bool tab_est_CbCr){
    

    if (f == NULL) {
        return false;
    } 
    uint8_t buf[5];
    buf[0]=0xff;
    buf[1]=MARQEUR_DQT;
    buf[2]=0;
    buf[3]=67; 
    if (tab_est_CbCr)
    {
       buf[4]=1;
    }else
    {
        buf[4]=0;
    }

    if (fwrite(buf,1,5,f)!=5)
    {
        return false;
    }
    if (fwrite(tab_quantif,1,64,f)!=64)
    {
        return false;
    }    
    return true;

}

bool ecrire_SOFx(FILE *f){



    
}