#ifndef __BOYER_MOOR_H
#define __BOYER_MOOR_H

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 64

struct liste_occ
{
    int occ;                 // indice de début d'une occurence
    struct liste_occ *suite; // occurences suivantes
};

struct liste_occ *append(struct liste_occ *liste, int occ);

int *occ_droite(const char *motif);

struct liste_occ *boyer_moore(
    FILE *fichier,     // fichier à disséquer
    const char *motif, // motif à chercher
);
#endif // __BOYER_MOOR_H