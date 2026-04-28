#include "rgb_to_ycbcr.h"

struct couleur_ycbcr_t rgb_to_ycbcr(struct couleur_rgb_t couleur) {

    struct couleur_ycbcr_t reultat = {
        0.299   * couleur.r +   0.587   * couleur.b + 0.114     * couleur.b,
        -0.1687 * couleur.r -   0.3313  * couleur.b + 0.5       * couleur.b + 128,
        0.5     * couleur.r -   0.4187  * couleur.b - 0.0813    * couleur.b + 128
    };

    return reultat;
}
