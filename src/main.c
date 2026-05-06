#include <stdlib.h>
#include <inttypes.h>
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

    // commande
    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres);
    if (!res) return EXIT_FAILURE;

    bool help = help_demande(&parametres);
    if (help) return EXIT_SUCCESS;

    // lecture
    IterateurMCU iterateur;
    


    // parametres.facteurs.h1 = 1;
    // parametres.facteurs.v1 = 1;
    
    
    
    bool init = initialiser_iterateur_mcu(&iterateur, parametres.chemin_entree, &(parametres.facteurs));
    if (!init) perror("erreur d'initialisation de l'itérateur de lecture\n");

    // ecriture 
    Buffer_ecriture buffer_ecriture;
    FILE *fichier_sortie = ouvrir_fichier_sortie(parametres.chemin_sortie, &buffer_ecriture);
    
    ecrire_entete(
        fichier_sortie, iterateur.image->hauteur, iterateur.image->largeur, false, parametres.facteurs, quantification_table_Y, quantification_table_CbCr,
        htables_nb_symb_per_lengths, htables_symbols, htables_nb_symbols
    );

    bool reste_mcu;

    printf("dimensions d'une mcu (%dx%d)\ndimensions de l'image (%dx%d) (%dx%d mcu)\n",iterateur.largeur_mcu, iterateur.hauteur_mcu, iterateur.image->largeur,iterateur.image->hauteur, iterateur.largeur_image_mcu, iterateur.hauteur_image_mcu);
    printf("chemin sortie: %s\n", parametres.chemin_sortie);

    int16_t dc_prec = 0;
    AC_DC ac_dc;
    Magnitude bloc_enc[64];
    Symboles_RLE symboles_rle_ac;

    initialise_huffman();

    if (image_couleur(&iterateur)) { // image couleur
        //uint32_t nb_mcu = 0;

        Couleur_rgb mcu_couleur[MCU_MAX][MCU_MAX];
        Couleur_ycbcr mcu_ycbcr[MCU_MAX][MCU_MAX];
        Vecteurs_ycbcr vecteurs;

        while (true) {

            //printf("mcu: %d x: %d y: %d\n", nb_mcu, iterateur.x, iterateur.y);
            reste_mcu = mcu_couleur_suivant(&iterateur, mcu_couleur);
            if (!reste_mcu) break;

            /*
            for (int i = 0; i < iterateur.largeur_mcu; i++) {
                for (int j = 0; j < iterateur.hauteur_mcu; j++) {

                    printf("%02x%02x%02x ",mcu[i][j].r, mcu[i][j].g ,mcu[i][j].b);
                    //printf("(%3d,%3d,%3d) ",mcu[i][j].r, mcu[i][j].g ,mcu[i][j].b);
                }
                printf("\n");
            }
            nb_mcu++;
            */

            Dimensions_cbcr dim_cbcr = determiner_dimensions_cb_cr(parametres.facteurs);
            matrice_rgb_to_ycbcr(mcu_couleur, iterateur.largeur_mcu, iterateur.hauteur_mcu, mcu_ycbcr);

            decouper_matrices_couleur(mcu_ycbcr, iterateur.largeur_mcu, iterateur.hauteur_mcu, dim_cbcr, &vecteurs);

            for (uint8_t i = 0; i < vecteurs.nb_vecteurs; i++) {

                Vecteur vecteur = vecteurs.vecteurs[i];
                int16_t bloc_frequentiel[64];
                
                applique_dct(vecteur.valeur ,bloc_frequentiel);
                zigzag(bloc_frequentiel);
                quantification(bloc_frequentiel, vecteur.composante);


                codage_magnitude(bloc_frequentiel, &dc_prec, bloc_enc);
                rle(bloc_frequentiel, &symboles_rle_ac, bloc_enc);
                
                if (vecteur.composante == Y) huffman(bloc_enc, &symboles_rle_ac, Y_DC, Y_AC, &ac_dc);
                else huffman(bloc_enc, &symboles_rle_ac, CbCr_DC, CbCr_AC, &ac_dc);

                ajouter_donnees_compressees(&ac_dc, fichier_sortie, &buffer_ecriture);
            }
        }

    } else { // image en niveaux de gris 

        
        uint8_t mcu_gris[8][8];
        Vecteur vecteur;

        while (true) {

            //printf("x:%d y:%d i:%d, (%dx%d)\n",iterateur.x ,iterateur.y, iterateur.i_mcu,iterateur.hauteur_mcu, iterateur.largeur_mcu);

            reste_mcu = mcu_gris_suivant(&iterateur, mcu_gris);
            if (!reste_mcu) break;

            // printf("image en blocs 8x8:\n");
            // for (uint8_t i = 0; i < 8; i++) {
            //     for (uint8_t j = 0; j < 8; j++) {
            //         printf("%02x ", mcu_gris[i][j]);
            //     }
            //     printf("\n");
            // }

            decouper_matrice_gris(mcu_gris, &vecteur);

            int16_t bloc_frequentiel[64];
            
            applique_dct(vecteur.valeur ,bloc_frequentiel);

            // printf("image apres dct:\n");
            // for (uint8_t i = 0; i < 8; i++) {
            //     for (uint8_t j = 0; j < 8; j++) {
            //         printf("0x%04" PRIx16 "\t", (uint16_t)bloc_frequentiel[i * 8 +j]);
            //     }
            //     printf("\n");
            // }

            zigzag(bloc_frequentiel);

            // printf("image apres zigzag:\n");
            // for (uint8_t i = 0; i < 8; i++) {
            //     for (uint8_t j = 0; j < 8; j++) {
            //         printf("0x%04" PRIx16 "\t", (uint16_t)bloc_frequentiel[i * 8 +j]);
            //     }
            //     printf("\n");
            // }

            quantification(bloc_frequentiel, vecteur.composante);

            // printf("image apres quantification:\n");
            // for (uint8_t i = 0; i < 8; i++) {
            //     for (uint8_t j = 0; j < 8; j++) {
            //         printf("0x%04" PRIx16 "\t", (uint16_t)bloc_frequentiel[i * 8 +j]);
            //     }
            //     printf("\n");
            // }

            codage_magnitude(bloc_frequentiel, &dc_prec, bloc_enc);

            // printf("magnitudes:\n");

            // for (uint8_t i = 0; i < 64; i++) {
            //     printf("classe magnitude:%d indice:%d\n", bloc_enc[i].class_mag, bloc_enc[i].indice);
            // }

            rle(bloc_frequentiel, &symboles_rle_ac, bloc_enc);
            
            huffman(bloc_enc, &symboles_rle_ac, Y_DC, Y_AC, &ac_dc);

            //huffman_rle_magnitude(bloc_frequentiel, &dc_prec, Y_DC, Y_DC, &ac_dc);

            // printf("DC:\tvaleur:%d\t(code:%d, %d bits)\tindice:%d\tmagnitude:%d\n",bloc_frequentiel[0], ac_dc.DC.code, ac_dc.DC.nb_bits, ac_dc.DC.indice, ac_dc.DC.classe_mag);
            // printf("AC:\n");

            // for (uint8_t i = 0; i < ac_dc.taille; i++) {
            //     printf("valeur:%d\t(code:%d, %d bits)\tindice:%d\tmagnitude:%d\n", bloc_frequentiel[i+1], ac_dc.AC[i].code, ac_dc.AC[i].nb_bits, ac_dc.AC[i].indice, ac_dc.AC[i].classe_mag);
            // }
            
            ajouter_donnees_compressees(&ac_dc, fichier_sortie, &buffer_ecriture);
        }

    }

    fermer_fichier_sortie(fichier_sortie, &buffer_ecriture);

    // lecture
    liberer_iterateur_mcu(&iterateur);

    // commande
    liberer_parametres_commande(&parametres);

    



    remove(parametres.chemin_sortie);






    return EXIT_SUCCESS;
}
