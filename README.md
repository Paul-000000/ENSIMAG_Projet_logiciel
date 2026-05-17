# État de l'encodeur JPEG

![Coverage](https://gitlab.ensimag.fr/formationc/projet/jpeg/2026/2_bahag_becharam_bottnerp/badges/main/coverage.svg)

[Tableau de bord](https://formationc.pages.ensimag.fr/projet/jpeg/2026/2_bahag_becharam_bottnerp)

# Diagramme de Gantt

![diagramme](diagrammes/Diagramme_semaine_2.png)

# Enchaînement du pipeline (Flux de données)

Dans cette partie, nous présentons un résumé de notre découpage modulaire et l'enchaînement de l'encodage de l'image source jusqu'au résultat final (l'image compressée au format JPEG).

-   **1) Parsing de la commande d'exécution (module `commande`) :**
    -   *Entrée :* Toute la chaîne de la ligne de commande.
    -   *Sortie :* Tous les paramètres importants pour l'encodage (fichier d'entrée et de sortie, facteurs d'échantillonnage).
-   **2) Lecture du fichier (module `lecture`) :**
    -   *Entrée :* Les paramètres nécessaires (fichier d'entrée et facteurs d'échantillonnage).
    -   *Sortie :* Un itérateur sur les MCU de l'image avec la dimension voulue.
-   **3) Découpage en blocs 8x8 YCbCr (module `downsampler`) :**
    -   *Entrée :* Une MCU RGB ou en niveaux de gris, et les facteurs d'échantillonnage.
    -   *Sortie :* La décomposition de la MCU en blocs 8x8 convertis en YCbCr.
-   **4) Calcul de la DCT (module `dct`) :**
    -   *Entrée :* Un bloc 8x8 spatial d'une composante donnée (Y, Cb ou Cr).
    -   *Sortie :* Un bloc 8x8 fréquentiel.
-   **5) Quantification et ordonnancement Zigzag (module `zigzag_quantification`) :**
    -   *Entrée :* Un bloc fréquentiel 8x8 d'une composante donnée.
    -   *Sortie :* Un vecteur de taille 64 quantifié et réordonné sous forme de zigzag.
-   **6) Encodage des valeurs (modules `magnitude`, `rle` et `huffman`) :**
    -   *Entrée :* Un vecteur de taille 64 quantifié et ordonné en zigzag.
    -   *Sortie :* Le bloc entièrement encodé (bitstream).
-   **7) Écriture dans le fichier de sortie (modules `ecriture_entete` et `ecriture`) :**
    -   *Entrée :* Les informations essentielles pour l'en-tête (tables de Huffman, facteurs d'échantillonnage) et les blocs encodés.
    -   *Sortie :* Une image compressée valide au format `.jpg`.

**Schéma récapitulatif :**

![ ](diagrammes/pipeline_1.png)
![Schéma](diagrammes/pipeline_2.png)

# Structures de données utilisées

![structures](structures%20de%20donnees/structures%20de%20donnees.svg)


# Répartition des tâches et organisation

Concernant notre méthodologie de travail, nous avons commencé par définir un diagramme de Gantt couvrant toute la durée de la campagne. Ce diagramme a été ajusté lors de la deuxième semaine pour coller à la réalité de notre avancement. Nous avons ensuite adopté une approche itérative en répartissant les tâches chaque matin entre les trois membres de l'équipe. 

Le détail des responsabilités s'organise ainsi :

*   **Paul Bottner :** Parsing des arguments de commande, découpage en MCU et sous-échantillonnage, écriture du flux de bits (bitstream), et orchestration générale dans le `main`.
*   **Abdoul Goudouss Bah :** Calcul de la DCT, quantification, ordonnancement en zigzag, et algorithme d'encodage de Huffman.
*   **Mohamed Khalil Becharai :** Lecture du fichier `.ppm` en superblocs de MCU, encodage des magnitudes, et écriture de l'en-tête du fichier `.jpg`.

> **Validation :** Chaque membre de l'équipe est responsable de la rédaction et de l'exécution des tests unitaires associés à ses modules pour en garantir la robustesse.

# Optimisations et évaluation des performances

Afin d'évaluer objectivement les performances de notre encodeur, nous avons utilisé deux approches, en effectuant nos tests principalement sur l'image `biiiig.ppm` pour mettre en évidence les gains de temps.

*   **Mesures internes :** Calcul du temps d'exécution de chaque grande étape dans le `main` à l'aide des fonctions de la bibliothèque `<time.h>`.
*   **Outils externes :** Utilisation de la commande `time` pour le temps global, et de `gprof` pour générer un rapport détaillé du temps passé dans chaque fonction.

Voici le détail des optimisations techniques appliquées à chaque étape de l'encodage :

**Calcul de la DCT :**
Nous sommes passés d'une implémentation initiale naïve en $O(N^3)$ à une version optimisée en $O(N^2)$.
*   **Séparabilité & Algorithme rapide :** Nous utilisons l'algorithme de DCT rapide 1D de *Arai, Agui et Nakajima (1988)*. Au lieu d'un calcul 2D lourd, on applique cet algorithme ultra-rapide sur les lignes, puis sur les colonnes.
*   **Notre optimisation (le `pad`) :** Nous avons modifié l'algorithme pour intégrer un pas de lecture en mémoire (`pad`). Cela permet d'appliquer la DCT sur les colonnes en sautant de 8 cases en 8 cases, ce qui nous évite totalement de devoir transposer la matrice en mémoire.

**Lecture du fichier :**
Afin d'éviter de charger l'image complète en mémoire (ce qui est critique pour les fichiers volumineux), nous avons mis en place une lecture par lots. L'image est lue par morceaux fixes de 64 MCU, optimisant ainsi l'empreinte mémoire (RAM).

**Conversion RGB vers YCbCr :**
*   **Tableaux statiques :** Pour éviter de recalculer les formules à chaque pixel, nous avons créé des tableaux statiques précalculés.
*   **Opérations bit-à-bit :** Nous avons remplacé les multiplications/divisions par 2 par des opérations de décalage de bits (`>>` et `<<`), beaucoup plus rapides pour le processeur.

**Encodage (Magnitude, RLE, Huffman) :**
*   **Réduction des branches :** Pour le calcul des magnitudes et des indices, nous avons éliminé quelques conditions (`if`) en utilisant des masques de bits sur les signes, évitant ainsi les pénalités de prédiction de branchement. Les multiplications par une puissance de 2 ont là aussi été remplacées par des décalages.
*   **Accès en $O(1)$ pour Huffman :** Les arbres de Huffman originaux ont été transformés en tableaux. Cela permet de remplacer le parcours de l'arbre en $O(\log n)$ par un simple accès mémoire direct en $O(1)$.
*   **Fusion du pipeline :** Nous avons fusionné les trois étapes (Magnitude, RLE, Huffman) en une seule grande fonction. Cela évite l'allocation de tableaux intermédiaires et supprime le coût d'appel des fonctions à chaque bloc.

**Échantillonnage et découpage en blocs :**
*   **Extraction des invariants :** Factorisation des boucles en sortant les calculs répétitifs à l'extérieur. 
*   **Nettoyage des boucles internes :** Les conditions (`if`) ont été sorties des boucles les plus profondes.

**Écriture du fichier de sortie :**
*   *(À rajouter après)*

# Notre encodeur JPEG à nous

Bienvenue sur la page d'accueil de *votre* projet JPEG, un grand espace de liberté, sous le regard bienveillant de vos enseignants préférés. Le sujet sera disponible dès lundi à l'adresse suivante : <https://formationc.pages.ensimag.fr/projet/jpeg/jpeg/>.

Vous pouvez reprendre cette page d'accueil comme bon vous semble, mais elle devra au moins comporter les infos suivantes **avant la fin de la première semaine** :

1.  des informations sur le découpage des fonctionnalités du projet en modules, en spécifiant les données en entrée et sortie de chaque étape ;
2.  (au moins) un dessin des structures de données de votre projet (format libre, ça peut être une photo d'un dessin manuscrit par exemple) ;
3.  une répartition des tâches au sein de votre équipe de développement, comportant une estimation du temps consacré à chacune d'elle (là encore, format libre, du truc cracra fait à la main, au joli Gantt chart).

Rajouter **régulièrement** des informations sur l'avancement de votre projet est aussi **une très bonne idée** (prendre 10 min tous les trois chaque matin pour résumer ce qui a été fait la veille, établir un plan d'action pour la journée qui commence et reporter tout ça ici, par exemple).

# Proposition de CI pour les élèves

## Makefile

-   Gère la génération de code : exécutable, debug et tests
-   Inclusion de sanitize par défaut (au détriment de Valgrind)
-   Gère la couverture de code
-   Intègre une cible pour lancer les tests en local
-   Intègre une cible perf pour faire de l'analyse de performance

## Unity pour faire des tests unitaires

-   Fonctionnement simplifié à l'extrême et ultra portable
-   [Le guide de démarrage](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityGettingStartedGuide.md)
-   [La liste des assertions possibles](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md)
-   exemples fournis dans tests/test\_\*.c : à vous d'en ajouter et de les complèter

## Pytest pour faire les tests d'intégrations

### Pourquoi ?

Pytest, c'est un des framework de test python les plus utilisés.

### Où

`tests/test_all.py`

### Contenu

-   Intégration des tests unity pour avoir un résumé de tests uniformes

-   Vérification fonctionnelle organisée en catégorie ("cli","gris","couleur"..) pour vérifier que le programme fonctionne et produit un fichier comme il devrait en validant la qualité d'image générée selon 3 métriques

    -   SSIM : une métrique de proximité Comparaison réalisée par rapport à l'outil convert
    -   PAE : une métrique (erreur absolu pic) pour détecter les pixels foireux
    -   AE : une métrique complémentaire pour détecter les images foireuses (plus de 2% de pixels à plus de 10% de l'original). Pour info, `convert` reste en dessous de 0,1% sur cette métrique !

-   test de performance sur les images "couleur" via callgrind et normalisation selon la métrique (instructions/pixel). C'est une métrique imparfaite qui ne capturé ni l'ILP du processeur ni les défauts de cache. Mais dans un parc info hétérogène (intra-Ensimag et PC étudiant), le temps n'est pas une métrique de comparaison fiable. Inst/pixel est indépendant de la machine et invariant selon l'image.

-   Test sur la mémoire : pile, tas, sections importantes et RSS

### Fonctionnalités

-   Génération d'une synthèse dans le terminal
-   Génération d'un xml pour une intégration dans la CI gitlab

## CI Gitlab

-   Pipeline à 4 étages :
    -   étage de compilation
    -   étage de vérif rapide (tests unitaires et CLI) pour éviter les tests inutiles
    -   étage d'évaluation pour faire :
        -   les tests d'intégration
        -   la couverture de code
        -   l'évaluation de performance
        -   l'évaluation mémoire
    -   étage de déploiement pour faire la page de résultats
-   Syntèse des tests : visible depuis build:jobs ou en cliquant sur le résultat du job
-   Intégration de la couverture de code :
    -   Résumé et suivi visible dans build:jobs
    -   Badge utilisable dans le README : ![Coverage](https://gitlab.ensimag.fr/formationc/projet/jpeg/2026/2_bahag_becharam_bottnerp/badges/main/coverage.svg)
    -   Page de couverture consultable dans depuis le tableau de bord
-   Génération d'un [tableau de bord incluant les stats(qualité, performance) par scénario, les infos mémoires, un lien vers les rapports et le profilage de Biiiiiiig](https://formationc.pages.ensimag.fr/projet/jpeg/2026/2_bahag_becharam_bottnerp)

# Liens utiles

-   Bien former ses messages de commits : <https://www.conventionalcommits.org/en/v1.0.0/> ;
-   Besoin de prendre l'air ? Le [Mont Rachais](https://fr.wikipedia.org/wiki/Mont_Rachais) est accessible à pieds depuis la salle E301 !
-   Un peu juste sur le projet à quelques heures de la deadline ? Le [Montrachet](https://www.vinatis.com/achat-vin-puligny-montrachet) peut faire passer l'envie à vos profs de vous mettre une tôle !
