#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "commande.h"
#include "lecture.h"
#include "rgb_to_ycbcr.h"
#include "downsampler.h"
#include "dct.h"
#include "zigzag.h"
#include "quantification.h"
#include "huffman.h"
#include "ecriture.h"
#include "ecriture_entete.h"



int main(int argc, char **argv) {
    
    // commande
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres, true);
    if (!res) {
        return EXIT_FAILURE;
    }

    bool help = help_demande(&parametres);
    if (help) {
        liberer_parametres_commande(&parametres);
        return EXIT_SUCCESS;
    }

    // lecture
    IterateurMCU iterateur;
    bool init = initialiser_iterateur_mcu(&iterateur, parametres.chemin_entree, parametres.facteurs);
    if (!init) {
        perror("erreur d'initialisation de l'itérateur de lecture");
        liberer_parametres_commande(&parametres);
        return EXIT_FAILURE;
    }

    // ecriture 
    Flux_Ecriture flux;
    ouvrir_fichier_sortie(parametres.chemin_sortie, &flux);
    
    bool entete = ecrire_entete(
        flux.fichier_sortie, iterateur.image->hauteur, iterateur.image->largeur, iterateur.image->type == P6, parametres.facteurs, quantification_table_Y, quantification_table_CbCr,
        htables_nb_symb_per_lengths, htables_symbols, htables_nb_symbols
    );

    if (!entete) {
        perror("erreur d'initialisation de l'entete dans le fichier de sortie");
        fermer_fichier_sortie(&flux);
        remove(parametres.chemin_sortie);
        liberer_parametres_commande(&parametres);
        return EXIT_FAILURE;
    }

    bool reste_mcu;
    AC_DC ac_dc;
    Magnitude bloc_enc[64];
    Symboles_RLE symboles_rle_ac;

    initialise_huffman();

    if (image_couleur(&iterateur)) { // image couleur

        int16_t dc_prec_y_cb_cr[3] = {0, 0, 0};
        Couleur_rgb mcu_couleur[MCU_MAX][MCU_MAX];
        Couleur_ycbcr mcu_ycbcr[MCU_MAX][MCU_MAX];
        Vecteurs_ycbcr vecteurs;
        Dimensions_cbcr dim_cbcr = determiner_dimensions_cb_cr(parametres.facteurs);

        while (true) {

            reste_mcu = mcu_couleur_suivant(&iterateur, mcu_couleur);
            if (!reste_mcu) {
                break;
            }

            matrice_rgb_to_ycbcr(mcu_couleur, iterateur.largeur_mcu, iterateur.hauteur_mcu, mcu_ycbcr);
            decouper_matrices_couleur(mcu_ycbcr, iterateur.largeur_mcu, iterateur.hauteur_mcu, dim_cbcr, &vecteurs);

            for (uint8_t i = 0; i < vecteurs.nb_vecteurs; i++) {

                Vecteur vecteur = vecteurs.vecteurs[i];
                int16_t bloc_frequentiel[64];
                
                applique_dct(vecteur.valeur ,bloc_frequentiel);
                zigzag(bloc_frequentiel);
                quantification(bloc_frequentiel, vecteur.composante);
                
                codage_magnitude(bloc_frequentiel, &(dc_prec_y_cb_cr[vecteur.composante]), bloc_enc);
                rle(bloc_frequentiel, &symboles_rle_ac, bloc_enc);
                encoder_coefficients_huffman(bloc_enc, &symboles_rle_ac, vecteur.composante, &ac_dc);

                ajouter_donnees_compressees(&ac_dc, &flux);
            }   
        }

    } else { // image en niveaux de gris 
        
        int16_t dc_prec = 0;
        uint8_t mcu_gris[8][8];
        Vecteur vecteur;
        int16_t bloc_frequentiel[64];

        while (true) {

            reste_mcu = mcu_gris_suivant(&iterateur, mcu_gris);
            if (!reste_mcu) {
                break;
            }
            
            decouper_matrice_gris(mcu_gris, &vecteur);    
            applique_dct(vecteur.valeur ,bloc_frequentiel);
            zigzag(bloc_frequentiel);
            quantification(bloc_frequentiel, vecteur.composante);
            codage_magnitude(bloc_frequentiel, &dc_prec, bloc_enc);
            rle(bloc_frequentiel, &symboles_rle_ac, bloc_enc);
            huffman_y(bloc_enc, &symboles_rle_ac, &ac_dc);
            ajouter_donnees_compressees(&ac_dc, &flux);
        }

    }

    fermer_fichier_sortie(&flux);

    // lecture
    liberer_iterateur_mcu(&iterateur);

    // commande
    liberer_parametres_commande(&parametres);

    return EXIT_SUCCESS;
}
