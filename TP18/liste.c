/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g liste.c -o liste
Date	: 10.02.2023
____________________
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "liste.h"

bool element_liste(int val, liste lst)
{
    if (lst == NULL)
        return false;
    return lst->val == val || element_liste(val, lst->suivant);
}

liste ajout_liste(int val, liste lst)
{
    if (element_liste(val, lst))
        return lst;
    liste new = malloc(sizeof(*new));
    new->val = val;
    new->suivant = lst;
    return new;
}

liste suppression_liste(int val, liste lst)
{
    if (lst == NULL)
        return NULL;
    if (lst->val == val)
    {
        liste tmp = lst->suivant;
        free(lst);
        return tmp;
    }
    lst->suivant = suppression_liste(val, lst->suivant);
    return lst;
}

void contenu_liste(liste lst)
{
    if (lst == NULL)
        return;
    printf("%d ", lst->val);
    contenu_liste(lst->suivant);
}

void vider_liste(liste lst)
{
    if (lst == NULL)
        return;
    vider_liste(lst->suivant);
    free(lst);
}

void tests_liste()
{
    liste l1 = NULL;
    assert(!element_liste(3, l1));
    l1 = ajout_liste(3, l1);
    assert(element_liste(3, l1));
    l1 = ajout_liste(4, l1);
    l1 = ajout_liste(5, l1);
    assert(element_liste(4, l1));
    l1 = suppression_liste(4, l1);
    assert(!element_liste(4, l1));
    vider_liste(l1);
}
