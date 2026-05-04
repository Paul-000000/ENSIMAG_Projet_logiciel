
#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct NoeudHuffman {
    
    uint16_t code;  
    uint8_t nb_bits;
    uint8_t valide;
    
}Huffman;

//NoeudHuffman* creer_noeud ();

//void inserer_symbole (NoeudHuffman* arbre, uint16_t code,int taille,uint8_t symbole );
//NoeudHuffman* construire_arbre_complet( uint8_t lengths[16], uint8_t* symbols[]);
void construction_arbre ( Huffman tab[256], uint8_t* lengths, uint8_t* symbols );
#endif