#include <stdio.h>
#include "magnitude_rle_huffman.h"
#include "magnitude.h"



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

        tab[i].valide = false;
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
            tab[symbol].valide = true;

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



bool magnitude_rle_huffman_composante(int16_t *dc_prec, const int16_t vecteur[64], const Huffman tab_dc[256], const Huffman tab_ac[256], AC_DC *resultat) {

    // coefficient DC
    Magnitude magnitude_dc = encoder_magnitude(vecteur[0] - (*dc_prec));

    uint8_t symbole_dc = magnitude_dc.classe;    
    
    resultat->DC.magnitude.indice = magnitude_dc.indice;
    resultat->DC.magnitude.classe = magnitude_dc.classe;
    
    if (!tab_dc[symbole_dc].valide) {
        return false;
    }

    resultat->DC.code = tab_dc[symbole_dc].code;
    resultat->DC.nb_bits = tab_dc[symbole_dc].nb_bits;
    *dc_prec = vecteur[0];



    // coefficients AC
    resultat->taille = 0;
    uint8_t indice_fin = 63;
    
    while (indice_fin > 0 && vecteur[indice_fin] == 0) {
        indice_fin--;
    }

    uint8_t nb_zero = 0;
    for(uint8_t i = 1; i <= indice_fin; i++) {

        if (vecteur[i] != 0) {

            Magnitude magnitude = encoder_magnitude(vecteur[i]);
            uint8_t symbole_ac = ((nb_zero << 4) + magnitude.classe);

            if (!tab_ac[symbole_ac].valide) {
                return false;
            }

            resultat->AC[resultat->taille].code = tab_ac[symbole_ac].code;
            resultat->AC[resultat->taille].nb_bits = tab_ac[symbole_ac].nb_bits;
            resultat->AC[resultat->taille].magnitude = magnitude;

            nb_zero = 0;
            resultat->taille += 1;
        
        } else {
            
            nb_zero++;

            if (nb_zero == 16) {

                uint8_t symbole_ac = ZRL;

                if (!tab_ac[symbole_ac].valide) {
                    return false;
                }

                resultat->AC[resultat->taille].code = tab_ac[symbole_ac].code;
                resultat->AC[resultat->taille].nb_bits = tab_ac[symbole_ac].nb_bits;
                resultat->AC[resultat->taille].magnitude.indice = 0;
                resultat->AC[resultat->taille].magnitude.classe = 0;

                nb_zero = 0;
                resultat->taille += 1;
            }
        }
    }

    if (indice_fin < 63) { // ajout des derniers 0

        uint8_t symbole_ac = EOB;

        if (!tab_ac[symbole_ac].valide) {
            return false;
        }

        resultat->AC[resultat->taille].code = tab_ac[symbole_ac].code;
        resultat->AC[resultat->taille].nb_bits = tab_ac[symbole_ac].nb_bits;
        resultat->AC[resultat->taille].magnitude.indice = 0;
        resultat->AC[resultat->taille].magnitude.classe = 0;
        resultat->taille += 1;
    }

    return true;
}

bool magnitude_rle_huffman_y(int16_t *dc_prec, const int16_t vecteur[64], AC_DC *resultat) {

    return magnitude_rle_huffman_composante(dc_prec, vecteur, Y_DC, Y_AC, resultat);
}

bool magnitude_rle_huffman_cbcr(int16_t *dc_prec, const int16_t vecteur[64], AC_DC *resultat) {

    return magnitude_rle_huffman_composante(dc_prec, vecteur, CbCr_DC, CbCr_AC, resultat);
}

bool magnitude_rle_huffman(int16_t *dc_prec, const int16_t vecteur[64], Composante composante, AC_DC *resultat) {

    return (composante == Y) ? magnitude_rle_huffman_y(dc_prec, vecteur, resultat) : magnitude_rle_huffman_cbcr(dc_prec, vecteur, resultat);
}
