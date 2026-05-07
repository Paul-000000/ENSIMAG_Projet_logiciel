#include <stdio.h>
#include "dct.h"
#include <math.h>
#include "fast-dct-8.h"
# define M_PI		3.14159265358979323846	/* pi */



static double tab_cos[8][8] = {

    { 1.0, 0.98078528040323043,  0.92387953251128674,    0.83146961230254524,    0.70710678118654757,    0.55557023301960229,    0.38268343236508984,    0.19509032201612833    }, 
    { 1.0, 0.83146961230254524,  0.38268343236508984,    -0.19509032201612819,   -0.70710678118654746,   -0.98078528040323043,   -0.92387953251128685,   -0.55557023301960218   }, 
    { 1.0, 0.55557023301960229,  -0.38268343236508973,   -0.98078528040323043,   -0.70710678118654768,   0.1950903220161283,     0.92387953251128652,    0.83146961230254546    }, 
    { 1.0, 0.19509032201612833,  -0.92387953251128674,   -0.55557023301960218,   0.70710678118654735,    0.83146961230254546,    -0.38268343236508989,   -0.98078528040323065   }, 
    { 1.0, -0.19509032201612819, -0.92387953251128685,   0.55557023301960184,    0.70710678118654768,    -0.83146961230254512,   -0.38268343236509056,   0.98078528040323043    }, 
    { 1.0, -0.55557023301960196, -0.38268343236509034,   0.98078528040323043,    -0.70710678118654668,   -0.19509032201612803,   0.92387953251128674,    -0.83146961230254501   }, 
    { 1.0, -0.83146961230254535, 0.38268343236509,       0.19509032201612878,    -0.70710678118654713,   0.98078528040323065,    -0.92387953251128641,   0.55557023301960151    }, 
    { 1.0, -0.98078528040323043, 0.92387953251128652,    -0.83146961230254512,   0.70710678118654657,    -0.55557023301960151,   0.38268343236508956,    -0.19509032201612858   }
};

void init_table_cosinus() {

    for (int x=0;x<8; x++) {
        for (int i=0;i<8;i++) {

            tab_cos[x][i] = cos(((2.0*x+1.0)*i*M_PI)/16.0);
        }
    }
}

// void applique_dct(const uint8_t bloc_spatial[64], int16_t bloc_frequentiel[64]){

//     double somme;
//     double raci = 1.0/sqrt(2.0);
//     double c_i,c_j;

//     for(int i=0;i<8;i++){
//         for(int j=0;j<8;j++){

//             somme = 0.0;

//             for (int x=0;x<8;x++){

//                 double cos_x = tab_cos[x][i];

//                 for(int y=0;y<8;y++){

//                     int valeur_decal = (int16_t)(bloc_spatial[x*8 + y]) - 128;

//                     double cos_y = tab_cos[y][j];
//                     somme += valeur_decal*cos_x*cos_y;
//                 }
//             }
//             c_i = (i == 0) ? raci : 1.0;
//             c_j = (j == 0) ? raci : 1.0;

//             double freq = 0.25*c_i*c_j*somme;
//             bloc_frequentiel[i*8 + j] = (int16_t)round(freq);


//         }
//     }
// }


/*void applique_dct(const uint8_t bloc_spatial[64], int16_t bloc_frequentiel[64]) {
    double temp[64];
    double raci = 1.0 / sqrt(2.0);


    for (int x = 0; x < 8; x++) {
        for (int j = 0; j < 8; j++) {
            double somme_y = 0.0;
            double c_j = (j == 0) ? raci : 1.0;
            for (int y = 0; y < 8; y++) {
                int valeur_decal = (int16_t)bloc_spatial[x * 8 + y] - 128;
                somme_y += valeur_decal * tab_cos[y][j];
            }
            temp[x * 8 + j] = somme_y * c_j;
        }
    }


    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            double somme_x = 0.0;
            double c_i = (i == 0) ? raci : 1.0;
            for (int x = 0; x < 8; x++) {
                somme_x += temp[x * 8 + j] * tab_cos[x][i];
            }
            // Le facteur 0.25 (1/4) vient de (1/2 * 1/2) des deux DCT-1D
            bloc_frequentiel[i * 8 + j] = (int16_t)(0.25 * c_i * somme_x);
        }
    }



}*/
/*void applique_dct(const int16_t bloc_spatial[64], int16_t bloc_frequentiel[64]){
    
    int16_t temp[8][8];
    double somme_x, somme_y;
    double raci = 1.0/sqrt(2.0);
    double c_i,c_j;

    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){

            somme = 0.0;

            for (int x=0;x<8;x++){

                double cos_x = tab_cos[x][i];

                for(int y=0;y<8;y++){

                    int valeur_decal = bloc_spatial[x*8 + y] - 128;

                    double cos_y = tab_cos[y][j];
                    somme += valeur_decal*cos_x*cos_y;
                }
            }
            c_i = (i == 0) ? raci : 1.0;
            c_j = (j == 0) ? raci : 1.0;

            double freq = 0.25*c_i*c_j*somme;
            bloc_frequentiel[i*8 + j] = (int16_t)(freq);


        }
    }
} */





void applique_dct(const uint8_t bloc_spatial[64], int16_t bloc_frequentiel[64]) {
    
    double bloc[8][8];

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            bloc[i][j] = (double)bloc_spatial[i * 8 + j] - 128.0;
        }
    }

   // applique ligne 
    for (int i = 0; i < 8; i++) {
        FastDct8_transform(bloc[i]);
    }

  
    for (int j = 0; j < 8; j++) {
        double colonne[8];
        
        for (int i = 0; i < 8; i++) {
            colonne[i] = bloc[i][j];
        }
        
        // applique colonne
        FastDct8_transform(colonne);
        
        // stock val
        for (int i = 0; i < 8; i++) {
            bloc[i][j] = colonne[i];
        }
    }

    // reporte frequentiel 
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            
            bloc_frequentiel[i * 8 + j] = (int16_t)round(bloc[i][j]);
        }
    }
}