
#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "rle.h"
#include "magnitude.h"
#include "htables.h"



typedef struct contenu
{
    uint16_t indice;
    uint8_t classe_mag;
    uint16_t code;
    uint8_t nb_bits;

} contenu;

typedef struct AC_DC
{
    contenu AC[63];
    contenu DC;
    uint8_t taille;

} AC_DC;

typedef struct NoeudHuffman
{
    uint16_t code;
    uint8_t nb_bits; 
    uint8_t valide;

} Huffman;



extern Huffman Y_DC[256];
extern Huffman Y_AC[256];
extern Huffman CbCr_DC[256];
extern Huffman CbCr_AC[256];



/*
NoeudHuffman* creer_noeud ();
void inserer_symbole (NoeudHuffman* arbre, uint16_t code,int taille,uint8_t symbole );
NoeudHuffman* construire_arbre_complet( uint8_t lengths[16], uint8_t* symbols[]);
*/

void construction_arbre_huffman(Huffman tab[256], const uint8_t *lengths, const uint8_t *symbols);

// Fonctions pour l'assemblage de Huffman, rle et magnitude
void initialise_huffman();

void huffman(Magnitude bloc_enc[64], Symboles_RLE *symboles_rle_ac, Huffman tab_dc[256], Huffman tab_ac[256], AC_DC *resultat);

#endif