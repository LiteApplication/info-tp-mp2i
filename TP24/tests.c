/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -g tests.c boyer_moor.c -o tests
Date	: 21.04.2023
____________________
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "boyer_moor.h"
// #files: boyer_moor.c

void test_occ_droite()
{
    int *occ = occ_droite("abracadabra");
    assert(occ['a'] == 7);
    assert(occ['b'] == 8);
    assert(occ['c'] == 4);
    assert(occ['d'] == 6);
    assert(occ['e'] == -1);

    free(occ);
}

void test_shift_buffer()
{
    FILE *f = fopen("test.txt", "w");
    assert(f != NULL);
    fprintf(f, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    fclose(f);

    f = fopen("test.txt", "r");
    char buffer[BUFFER_SIZE];
    shift_buffer(buffer, 0, f);
    assert(strcmp(buffer, "Lorem ipsum dol\0") == 0);

    shift_buffer(buffer, 5, f);
    assert(strcmp(buffer, "ipsum dolor sit amet, consectetur adipiscing elit.") == 0);

    shift_buffer(buffer, 10, f);
    assert(strcmp(buffer, "consectetur adipiscing elit.amet, consectetur adipiscing elit.") == 0);

    fclose(f);
    remove("test.txt");
}

void test_boyer_moore()
{
    FILE *fichier = fopen("test.txt", "r");
    assert(fichier != NULL);

    struct liste_occ *liste = boyer_moore(fichier, "branche");
    assert(liste != NULL);
    assert(liste->occ == 123); // la première occurence se trouve à l'indice 123 du texte

    fclose(fichier);

    // libération de la mémoire allouée pour la liste des occurrences
    while (liste != NULL)
    {
        struct liste_occ *tmp = liste->suite;
        free(liste);
        liste = tmp;
    }
}

int main()
{
    test_occ_droite();
    test_shift_buffer();
    printf("Tests passed\n");
    return 0;
}