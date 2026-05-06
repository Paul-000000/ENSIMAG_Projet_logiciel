#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "commande.h"
#include "lecture.h"
#include "rgb_to_ycbcr.h"
#include "downsampler.h"
#include "dct.h"
#include "zz_quant.h"
#include "huffman.h"
#include "ecriture.h"
#include "ecriture_entete.h"



int main(int argc, char **argv) {
    
    time_t debut = time(NULL);

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
    bool init = initialiser_iterateur_mcu(&iterateur, parametres.chemin_entree, &(parametres.facteurs));
    if (!init) {
        perror("erreur d'initialisation de l'itérateur de lecture");
        liberer_parametres_commande(&parametres);
        return EXIT_FAILURE;
    }

    // ecriture 
    Buffer_ecriture buffer_ecriture;
    FILE *fichier_sortie = ouvrir_fichier_sortie(parametres.chemin_sortie, &buffer_ecriture);
    
    bool entete = ecrire_entete(
        fichier_sortie, iterateur.image->hauteur, iterateur.image->largeur, iterateur.image->type == P6, parametres.facteurs, quantification_table_Y, quantification_table_CbCr,
        htables_nb_symb_per_lengths, htables_symbols, htables_nb_symbols
    );

    if (!entete) {
        perror("erreur d'initialisation de l'entete dans le fichier de sortie");
        fermer_fichier_sortie(fichier_sortie, &buffer_ecriture);
        remove(parametres.chemin_sortie);
        liberer_parametres_commande(&parametres);
        return EXIT_FAILURE;
    }

    bool reste_mcu;

    // affichages de débug
    printf("dimensions d'une mcu (%dx%d)\ndimensions de l'image (%dx%d) (%dx%d mcu)\n",iterateur.largeur_mcu, iterateur.hauteur_mcu, iterateur.image->largeur,iterateur.image->hauteur, iterateur.largeur_image_mcu, iterateur.hauteur_image_mcu);
    printf("chemin sortie: %s\n", parametres.chemin_sortie);

    AC_DC ac_dc;
    Magnitude bloc_enc[64];
    Symboles_RLE symboles_rle_ac;

    initialise_huffman();

    if (image_couleur(&iterateur)) { // image couleur

        int16_t dc_prec_y_cb_cr[3] = {0, 0, 0};
        Couleur_rgb mcu_couleur[MCU_MAX][MCU_MAX];
        Couleur_ycbcr mcu_ycbcr[MCU_MAX][MCU_MAX];
        Vecteurs_ycbcr vecteurs;

        while (true) {

            reste_mcu = mcu_couleur_suivant(&iterateur, mcu_couleur);
            if (!reste_mcu) break;

            Dimensions_cbcr dim_cbcr = determiner_dimensions_cb_cr(parametres.facteurs);
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
                
                if (vecteur.composante == Y) huffman(bloc_enc, &symboles_rle_ac, Y_DC, Y_AC, &ac_dc);
                else huffman(bloc_enc, &symboles_rle_ac, CbCr_DC, CbCr_AC, &ac_dc);

                ajouter_donnees_compressees(&ac_dc, fichier_sortie, &buffer_ecriture);
            }
        }

    } else { // image en niveaux de gris 
        
        int16_t dc_prec = 0;
        uint8_t mcu_gris[8][8];
        Vecteur vecteur;
        int16_t bloc_frequentiel[64];

        while (true) {

            reste_mcu = mcu_gris_suivant(&iterateur, mcu_gris);
            if (!reste_mcu) break;

            decouper_matrice_gris(mcu_gris, &vecteur);    
            applique_dct(vecteur.valeur ,bloc_frequentiel);
            zigzag(bloc_frequentiel);
            quantification(bloc_frequentiel, vecteur.composante);
            codage_magnitude(bloc_frequentiel, &dc_prec, bloc_enc);
            rle(bloc_frequentiel, &symboles_rle_ac, bloc_enc);
            huffman(bloc_enc, &symboles_rle_ac, Y_DC, Y_AC, &ac_dc);
            ajouter_donnees_compressees(&ac_dc, fichier_sortie, &buffer_ecriture);
        }

    }

    fermer_fichier_sortie(fichier_sortie, &buffer_ecriture);

    // lecture
    liberer_iterateur_mcu(&iterateur);

    // commande
    liberer_parametres_commande(&parametres);

    printf("effectué en : %.3lds\n", time(NULL) - debut);
    return EXIT_SUCCESS;
}
