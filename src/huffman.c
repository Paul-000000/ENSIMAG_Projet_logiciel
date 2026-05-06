
#include <stdio.h>
#include "huffman.h"
#include "rle.h"



Huffman Y_DC[256];
Huffman Y_AC[256];
Huffman CbCr_DC[256];
Huffman CbCr_AC[256];



/* NoeudHuffman* creer_noeud (){
   NoeudHuffman * noeud = (NoeudHuffman*)malloc(sizeof(NoeudHuffman));
   noeud->gauche = NULL;
   noeud->droite = NULL;
   noeud->est_feuille = 0;
   noeud->symbole = 0;
   return noeud;
}


void inserer_symbole (NoeudHuffman* arbre, uint16_t code,int taille,uint8_t symbole ){
   NoeudHuffman *courant = arbre;

   for (int i= taille -1; i>=0; i--){
       uint8_t bit = (code >> i) & 1; // recuperer le ieme bit de code

       if (bit == 0){ // gauche
           if(courant->gauche == NULL){
               courant-> gauche = creer_noeud();
           }
           courant = courant->gauche;
       }else { // droite
           if (courant ->droite == NULL){
               courant->droite = creer_noeud();
           }
           courant = courant->droite;
       }

   }
   courant -> est_feuille = 1; // creer une feuille puis inserer la donnée
   courant -> symbole = symbole;
}

NoeudHuffman* construire_arbre_complet( uint8_t lengths[16], uint8_t symbols[]){

   NoeudHuffman* arbre = creer_noeud();

   uint16_t code = 0; // le code en cours de construction
   int index_symbole =0; // position dans tab symboles

   for (int taille=1; taille <= 16; taille++){
       uint8_t nb_code = lengths [taille -1 ];

       // pour chaque symbole de cette longeur
       for (int j=0; j<nb_code; j++){
           uint8_t symbole = symbols [index_symbole];

           inserer_symbole(arbre,code,taille,symbole);

           code++;
           index_symbole++;
       }
       // decaler le code vers la gauche avant de passer à un autre niveau
       code = code << 1;
   }

   return arbre;
}
*/

void construction_arbre_huffman(Huffman tab[256], const uint8_t *lengths, const uint8_t *symbols)
{

    for (int i = 0; i < 256; i++)
    {

        tab[i].valide = 0;
    }

    uint16_t code = 0;
    int index_symbole = 0;

    for (int longeur = 1; longeur <= 16; longeur++)
    {
        uint8_t nb_code = lengths[longeur - 1];

        for (int j = 0; j < nb_code; j++)
        {
            uint8_t symbol = symbols[index_symbole];

            tab[symbol].code = code;
            tab[symbol].nb_bits = longeur;
            tab[symbol].valide = 1;

            code++;
            index_symbole++;
        }
        // decaler le code vers la gauche avant de passer à un autre niveau
        code = code << 1;
    }
}

void initialise_huffman() {

    construction_arbre_huffman(Y_DC, htables_nb_symb_per_lengths[0][0], htables_symbols[0][0]);
    construction_arbre_huffman(Y_AC, htables_nb_symb_per_lengths[1][0], htables_symbols[1][0]);
    construction_arbre_huffman(CbCr_DC, htables_nb_symb_per_lengths[0][1], htables_symbols[0][1]);
    construction_arbre_huffman(CbCr_AC, htables_nb_symb_per_lengths[1][1], htables_symbols[1][1]);
}

void huffman(Magnitude bloc_enc[64], Symboles_RLE *symboles_rle_ac, Huffman tab_dc[256], Huffman tab_ac[256], AC_DC *resultat) {
    
    uint8_t symbole_dc = bloc_enc[0].class_mag;    
    
    resultat->DC.indice = bloc_enc[0].indice;
    resultat->DC.classe_mag = bloc_enc[0].class_mag;
    
    if (tab_dc[symbole_dc].valide) {
        resultat->DC.code = tab_dc[symbole_dc].code;
        resultat->DC.nb_bits = tab_dc[symbole_dc].nb_bits;

    } else {
        resultat->DC.code = 0;
        resultat->DC.nb_bits = 0;
    }

    
    resultat->taille = symboles_rle_ac->taille; 

    for (int i = 0; i < symboles_rle_ac->taille; i++) {
        
        uint8_t symbole_ac = symboles_rle_ac->symboles[i].symbole;
        
        resultat->AC[i].indice = symboles_rle_ac->symboles[i].indice;
        resultat->AC[i].classe_mag = symboles_rle_ac->symboles[i].classe_mag;
        
        if (tab_ac[symbole_ac].valide) {
            resultat->AC[i].code = tab_ac[symbole_ac].code;
            resultat->AC[i].nb_bits = tab_ac[symbole_ac].nb_bits;
        } else {
            resultat->AC[i].code = 0;
            resultat->AC[i].nb_bits = 0;
        }
    }
}