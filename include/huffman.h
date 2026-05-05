
#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "rle.h"

typedef struct contenu {
    uint16_t indice;
    uint16_t code;
    uint8_t nb_bits;
    

}contenu;

typedef struct AC_DC {
    contenu AC[63];
    contenu DC;
    uint8_t taille;

}AC_DC;

typedef struct NoeudHuffman {
    
    uint16_t code;  
    uint8_t nb_bits;
    uint8_t valide;
    
}Huffman;

//NoeudHuffman* creer_noeud ();

//void inserer_symbole (NoeudHuffman* arbre, uint16_t code,int taille,uint8_t symbole );
//NoeudHuffman* construire_arbre_complet( uint8_t lengths[16], uint8_t* symbols[]);

void construction_arbre_huffman ( Huffman tab[256], uint8_t* lengths, uint8_t* symbols );

//---------------------Fonctions pour l'ensemblement de Huffman, rle et magnitude-----------
AC_DC huffman_rle_magnitude(int16_t bloc[64], int16_t *dc_prec, Huffman tab_dc[256], Huffman tab_ac[256]);
void initialise_huffman();
#endif