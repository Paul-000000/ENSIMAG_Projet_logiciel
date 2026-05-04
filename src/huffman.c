
#include <stdio.h>
#include "huffman.h"


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

NoeudHuffman* construire_arbre_complet( uint8_t lengths[16], uint8_t* symbols[]){

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

void construction_arbre ( Huffman tab[256], uint8_t* lengths, uint8_t* symbols ){
    for (int i=0; i<256;i++){
        tab[i].valide = 0;

    }

    uint16_t code  = 0;
    int index_symbole = 0;

    for ( int longeur = 1; longeur <= 16; longeur++){
        uint8_t  nb_code = lengths[longeur-1];

        for ( int j = 0; j<nb_code;j++ ){
            uint8_t symbol = symbols[ index_symbole];

            tab[symbol].code = code;
            tab[symbol].nb_bits = longeur;
            tab[symbol].valide =  1;

            
            code++;
            index_symbole++;
        }
        // decaler le code vers la gauche avant de passer à un autre niveau 
        code = code << 1;

    }

}

