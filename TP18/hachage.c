/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g hachage.c liste.c -o hachage
Date	: 10.02.2023
____________________
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "hachage.h"

// #files: liste.c
int zero(int n, int p)
{ // le paramètre est là pour le type de la fonction
    (void)n, (void)p;
    return 0;
} // très peu intéressant comme fonction de hachage

int identite(int n, int p)
{
    return n % p;
}

int hash_srandom(int n, int p)
{
    srand(n);
    return rand() % p;
}

int hash_prime(int n, int p)
{
    return (n * p * p + 1) % p;
}

hachage
init(int taille, int (*function)(int, int))
{
    hachage h;
    h.lg = taille;
    h.alveoles = malloc(taille * sizeof(liste));
    for (int i = 0; i < taille; i++)
        h.alveoles[i] = NULL;
    h.hacher = function;
    return h;
}

void ajout(hachage h, int val)
{
    int i = h.hacher(val, h.lg) % h.lg;
    h.alveoles[i] = ajout_liste(val, h.alveoles[i]);
}

bool element(hachage h, int val)
{
    int i = h.hacher(val, h.lg) % h.lg;
    return element_liste(val, h.alveoles[i]);
}

void suppression(hachage h, int val)
{
    int i = h.hacher(val, h.lg) % h.lg;
    h.alveoles[i] = suppression_liste(val, h.alveoles[i]);
}

void contenu(hachage h)
{
    for (int i = 0; i < h.lg; i++)
    {
        printf("%d : ", i);
        contenu_liste(h.alveoles[i]);
        printf("\n");
    }
}

void vider(hachage h)
{
    for (int i = 0; i < h.lg; i++)
        vider_liste(h.alveoles[i]);

    free(h.alveoles);
}

void tests_hachage()
{

    // Test de la fonction zero
    assert(zero(1, 2) == 0);
    assert(zero(3, 4) == 0);
    assert(zero(5, 6) == 0);

    // Test de la fonction init
    hachage h = init(10, zero);
    assert(h.lg == 10);
    for (int i = 0; i < 10; i++)
        assert(h.alveoles[i] == NULL);
    assert(h.hacher == zero);

    // Test de la fonction ajout
    ajout(h, 1);
    int i = zero(1, h.lg) % h.lg;
    assert(element_liste(1, h.alveoles[i]));

    // Test de la fonction element
    assert(element(h, 1));
    assert(!element(h, 2));

    // Test de la fonction suppression
    suppression(h, 1);
    i = zero(1, h.lg) % h.lg;
    assert(!element_liste(1, h.alveoles[i]));

    // Test de la fonction vider
    ajout(h, 2);
    ajout(h, 3);
    vider(h);
}

int main()
{
    tests_liste();
    tests_hachage();
    return 0;
}