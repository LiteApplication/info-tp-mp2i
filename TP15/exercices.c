/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g exercices.c files.c exemple.c -o exercices
Date	: 20.01.2023
____________________
*/
#include <stdio.h>
#include <assert.h>
#include "exemple.h"
#include "files.h"
#include "arbres.h"
// #files: files.c exemple.c

// Exercice 1
void parcours_largeur(struct arbre *a)
{
    file f = file_vide();
    enfile(f, a);
    while (!est_file_vide(f))
    {
        struct arbre *b = defile(f);
        printf("%c ", b->nom);
        if (b->g != NULL)
        {
            enfile(f, b->g);
        }
        if (b->d != NULL)
        {
            enfile(f, b->d);
        }
    }
    printf("\n");
}

// Exercice 2
void parcours_niveaux(struct arbre *a)
{
    // On insère null entre chaque niveau
    file f = file_vide();
    enfile(f, a);
    enfile(f, NULL);
    while (!est_file_vide(f))
    {
        struct arbre *b = defile(f);
        if (b == NULL)
        {
            printf("\n");
            if (!est_file_vide(f))
            {
                enfile(f, NULL);
            }
        }
        else
        {
            printf("%c ", b->nom);
            if (b->g != NULL)
            {
                enfile(f, b->g);
            }
            if (b->d != NULL)
            {
                enfile(f, b->d);
            }
        }
    }
}

int remplir_espaces(struct arbre *a, int hauteur)
{
    if (hauteur == 0)
    {
        a->espace = 0;
        return 0;
    }
    int espace_g = remplir_espaces(a->g, hauteur - 1);
    int espace_d = remplir_espaces(a->d, hauteur - 1);
    assert(espace_g == espace_d); // arbre parfait seulement
    a->espace = 2 * espace_g + 1;
    return a->espace;
}

void joli_affichage_arbre_parfait(struct arbre *a)
{
    // L'arbre a été construit avec exemple_parfait et a subi remplir_espaces
    file f = file_vide();
    enfile(f, a);
    enfile(f, NULL);
    while (!est_file_vide(f))
    {
        struct arbre *b = defile(f);
        if (b == NULL)
        {
            printf("\n");
            if (!est_file_vide(f))
            {
                enfile(f, NULL);
            }
        }
        else
        {
            for (int i = 0; i < b->espace; i++)
            {
                printf(" ");
            }
            printf("%c", b->nom);
            for (int i = 0; i < b->espace + 1; i++)
            {
                printf(" ");
            }
            if (b->g != NULL)
            {
                enfile(f, b->g);
            }
            if (b->d != NULL)
            {
                enfile(f, b->d);
            }
        }
    }
}

void parcours_prefixe(struct arbre *a)
{
    if (a == NULL)
    {
        return;
    }
    printf("%c\n", a->nom);
    parcours_prefixe(a->g);
    parcours_prefixe(a->d);
}

void parcours_decale_aux(struct arbre *a, int decalage)
{
    if (a == NULL)
    {
        return;
    }
    for (int i = 0; i < decalage; i++)
    {
        printf("⎯⎯");
    }
    printf("%c\n", a->nom);
    parcours_decale_aux(a->g, decalage + 1);
    parcours_decale_aux(a->d, decalage + 1);
}
void parcours_decale(struct arbre *a)
{
    parcours_decale_aux(a, 0);
}

/*
a-b--d
   --e---g----j
 -c--f---h
      ---i
*/
void parcours_decale_joli_aux(struct arbre *a, int decalage, bool espaces)
{
    if (a == NULL)
    {
        return;
    }
    if (espaces)
    {
        for (int i = 0; i < decalage; i++)
            printf(" ");
    }
    for (int i = 0; i < decalage; i++)
        printf("―");
    printf("%c", a->nom);
    if (a->g == NULL)
    {
        if (a->d == NULL)
        {
            printf("\n");
            return;
        }
        parcours_decale_joli_aux(a->d, decalage + 1, false);
        return;
    }
    parcours_decale_joli_aux(a->g, decalage + 1, false);
    parcours_decale_joli_aux(a->d, decalage + 1, true);
}

int main(void)
{
    struct arbre *a = exemple();
    // Exercice 1
    printf("========  Parcours en largeur: ========\n");
    parcours_largeur(a);
    // Exercice 2
    printf("======== Parcours par niveaux: ========\n");
    parcours_niveaux(a);
    // Exercice 3
    printf("========    Affichage joli:    ========\n");
    struct arbre *b = exemple_parfait(4);
    remplir_espaces(b, 4);
    joli_affichage_arbre_parfait(b);
    // Exercice 4
    printf("========  Parcours préfixe:    ========\n");
    parcours_prefixe(a);
    // Exercice 5
    printf("========  Parcours décalé:     ========\n");
    parcours_decale(a);
    // Exercice 6
    printf("======== Parcours décalé joli: ========\n");
    parcours_decale_joli_aux(a, 0, false);
    printf("\n");

    return 0;
}