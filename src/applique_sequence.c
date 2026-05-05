#include "huffman.h"
#include "rle.h"
#include "magnitude.h"
#include "htables.h"


Huffman Y_DC[256];
Huffman Y_AC[256];
Huffman CbCr_DC[256];
Huffman CbCr_AC[256];

void initialise_huffman(){
    construction_arbre_huffman(Y_DC,
         htables_nb_symb_per_lengths[0][0],htables_symbols[0][0]);
    construction_arbre_huffman(Y_AC,
         htables_nb_symb_per_lengths[1][0],htables_symbols[1][0]);
    construction_arbre_huffman(CbCr_DC,
         htables_nb_symb_per_lengths[0][1],htables_symbols[0][1]);
    construction_arbre_huffman(CbCr_AC,
         htables_nb_symb_per_lengths[1][1],htables_symbols[1][1]);
}


AC_DC huffman_rle_magnitude(int16_t bloc[64], int16_t *dc_prec, Huffman tab_dc[256], Huffman tab_ac[256]) {
    
    AC_DC resultat; 
    Magnitude bloc_enc[64];
    Symboles_RLE symboles_rle_ac;

 
    rle_magnitude(bloc, dc_prec, bloc_enc, &symboles_rle_ac);

    uint8_t symbole_dc = bloc_enc[0].class_mag;
    
    
    resultat.DC.indice = bloc_enc[0].indice;
    
    
    if (tab_dc[symbole_dc].valide) {
        resultat.DC.code = tab_dc[symbole_dc].code;
        resultat.DC.nb_bits = tab_dc[symbole_dc].nb_bits;
    } else {
        resultat.DC.code = 0;
        resultat.DC.nb_bits = 0;
    }

    
    resultat.taille = symboles_rle_ac.taille; 

    for (int i = 0; i < symboles_rle_ac.taille; i++) {
        
        uint8_t symbole_ac = symboles_rle_ac.symboles[i].symbole;
        
        resultat.AC[i].indice = symboles_rle_ac.symboles[i].indice;

        
        if (tab_ac[symbole_ac].valide) {
            resultat.AC[i].code = tab_ac[symbole_ac].code;
            resultat.AC[i].nb_bits = tab_ac[symbole_ac].nb_bits;
        } else {
            resultat.AC[i].code = 0;
            resultat.AC[i].nb_bits = 0;
        }
    }

    return resultat;
}