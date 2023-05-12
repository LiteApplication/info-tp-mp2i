#ifndef __BOYER_MOOR_H
#define __BOYER_MOOR_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 16

struct liste_occ
{
    int occ;                 // indice de début d'une occurence
    struct liste_occ *suite; // occurences suivantes
};

struct liste_occ *append(struct liste_occ *liste, int occ);

int min(int, int);
bool shift_buffer(char buffer[BUFFER_SIZE], int keep_left, FILE *fichier);

int *occ_droite(const char *motif);

struct liste_occ *boyer_moore(
    FILE *fichier,    // fichier à disséquer
    const char *motif // motif à chercher
);
#endif // __BOYER_MOOR_H