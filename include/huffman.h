
#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "rle.h"
#include "magnitude.h"
#include "htables.h"
#include "downsampler.h"



typedef struct codageHuffman // coefficient compressé avec Huffman
{
    uint16_t indice; // indice par rapport à la classe de magnitude
    uint8_t classe_mag; // classe de magnitude
    uint16_t code; // code de Huffman
    uint8_t nb_bits; // longueur du code en bits

} CodageHuffman;

typedef struct AC_DC // bloc de compressé avec Huffman
{
    CodageHuffman AC[63]; // les encodages Huffman des 63 coefficients AC
    CodageHuffman DC; // l'encodage Huffman de DC
    uint8_t taille; // le nombre de coefficients AC encodés

} AC_DC;

typedef struct NoeudHuffman // noeud de l'abre d'Huffman
{
    uint16_t code; // code binaire du noeud
    uint8_t nb_bits; // nombre de bits de ce code binaire
    bool valide; // indique si l'entrée est valide ou non

} Huffman;



// construit un tableau à 256 entrées qui pour chaque entrée valide donne l'encodage Huffman correspondant
void construction_arbre_huffman(Huffman tab[256], const uint8_t *lengths, const uint8_t *symbols);

// initialise les 4 tableaux contenant la convertion octet à code huffman
void initialise_huffman();

// encode les AC et le DC avec Huffman
void huffman(const Magnitude bloc_enc[64], const Symboles_RLE *symboles_rle_ac, const Huffman tab_dc[256], const Huffman tab_ac[256], AC_DC *resultat);

// encode les AC et le DC ayant la composante Y avec Huffman
void huffman_y(const Magnitude bloc_enc[64], const Symboles_RLE *symboles_rle_ac, AC_DC *resultat);

// encode les AC et le DC ayant la composante CbCr avec Huffman
void huffman_cbcr(const Magnitude bloc_enc[64], const Symboles_RLE *symboles_rle_ac, AC_DC *resultat);

// encode les AC et le DC avec Huffman en fonction de sa composante
void encoder_coefficients_huffman(const Magnitude bloc_enc[64], const Symboles_RLE *symboles_rle_ac, Composante composante, AC_DC *resultat);



#endif 