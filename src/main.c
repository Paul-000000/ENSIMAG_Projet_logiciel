#include <stdlib.h>
#include "htables.h"
#include "qtables.h"
#include "commande.h"
#include <lecture.h>


int main(int argc, char **argv) {

    Parametres_commande parametres;

    bool res = initialiser_parametres_commande(argc, argv, &parametres);
    if (!res) return EXIT_FAILURE;

    bool help = help_demande(&parametres);
    if (help) return EXIT_SUCCESS;

    // suite des fonctions

    liberer_parametres_commande(&parametres);

    return EXIT_SUCCESS;
}
