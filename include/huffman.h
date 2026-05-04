
#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct NoeudHuffman {
    struct Arbrehuffman * gauche;
    struct Arbrehuffman * droite;
    uint8_t est_feuille;  
    uint8_t symbole;
    
}NoeudHuffman;

NoeudHuffman* creer_noeud ();

void inserer_symbole (NoeudHuffman* arbre, uint16_t code,int taille,uint8_t symbole );
NoeudHuffman* construire_arbre_complet( uint8_t lengths[16], uint8_t* symbols[]);

#endif