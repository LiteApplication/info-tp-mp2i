/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -g boyer_moor.c -o boyer_moor
Date	: 21.04.2023
____________________
*/
#include "boyer_moor.h"
// #package boyer_moor.h

/*
prend une chaîne de caractères en argument et renvoie un tableau de la taille
le nombre de valeurs du type char, qui contient la dernière occurrence de (char)
i dans la case d’indice i, et -1 si (char) i n’apparaît pas dans le motif
*/
int *occ_droite(const char *motif)
{
    int *tab = malloc(255 * sizeof(int));
    for (int i = 0; i < 255; i++)
        tab[i] = -1;

    for (int i = 0; motif[i + 1] != '\0'; i++)
        tab[(int)motif[i]] = i;

    return tab;
}

struct liste_occ *append(struct liste_occ *liste, int occ)
{
    struct liste_occ *new = malloc(sizeof(struct liste_occ));
    new->occ = occ;
    new->suite = liste;

    return new;
}

struct liste_occ *boyer_moore(FILE *fichier, const char *motif)
{
    struct liste_occ *liste = NULL;
    char buffer[BUFFER_SIZE];
    int *occ_lettres = occ_droite(motif);

    int taille_motif = strlen(motif);

    return liste;
}
