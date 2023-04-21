/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g alg.c -o alg
Date	: 09.02.2023
____________________
*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "alg.h"

// #package: alg.h alg.ml

alg *exemples()
{ // “inf”, “info”, “matelas”, “math”, “matrice”
    alg *a = malloc(sizeof(alg) * 16);
    a[0] = (alg){.lettre = 'i', .fils = a + 1, .frere = a + 4, .end = false, .parent = NULL};
    a[1] = (alg){.lettre = 'n', .fils = a + 2, .frere = NULL, .end = false, .parent = a + 0};
    a[2] = (alg){.lettre = 'f', .fils = a + 3, .frere = NULL, .end = true, .parent = a + 1};
    a[3] = (alg){.lettre = 'o', .fils = NULL, .frere = NULL, .end = true, .parent = a + 2};
    a[4] = (alg){.lettre = 'm', .fils = a + 5, .frere = NULL, .end = false, .parent = NULL};
    a[5] = (alg){.lettre = 'a', .fils = a + 6, .frere = NULL, .end = false, .parent = a + 4};
    a[6] = (alg){.lettre = 't', .fils = a + 7, .frere = NULL, .end = false, .parent = a + 5};
    a[7] = (alg){.lettre = 'e', .fils = a + 8, .frere = a + 11, .end = false, .parent = a + 6};
    a[8] = (alg){.lettre = 'l', .fils = a + 9, .frere = NULL, .end = false, .parent = a + 7};
    a[9] = (alg){.lettre = 'a', .fils = a + 10, .frere = NULL, .end = false, .parent = a + 8};
    a[10] = (alg){.lettre = 's', .fils = NULL, .frere = NULL, .end = true, .parent = a + 9};
    a[11] = (alg){.lettre = 'h', .fils = NULL, .frere = a + 12, .end = true, .parent = a + 6};
    a[12] = (alg){.lettre = 'r', .fils = a + 13, .frere = NULL, .end = false, .parent = a + 6};
    a[13] = (alg){.lettre = 'i', .fils = a + 14, .frere = NULL, .end = false, .parent = a + 12};
    a[14] = (alg){.lettre = 'c', .fils = a + 15, .frere = NULL, .end = false, .parent = a + 13};
    a[15] = (alg){.lettre = 'e', .fils = NULL, .frere = NULL, .end = true, .parent = a + 14};
    return a;
}

stack init_stack()
{
    stack r = malloc(sizeof(struct stack *));
    *r = NULL;
    return r;
}

void push(stack s, alg *a)
{
    struct stack *n = malloc(sizeof(struct stack));
    n->a = a;
    n->next = *s;
    *s = n;
}

alg *pop(stack s)
{
    if (*s == NULL)
        assert(false);
    alg *a = (*s)->a;
    struct stack *n = *s;
    *s = (*s)->next;
    free(n);
    return a;
}
alg *top(stack s)
{
    if (*s == NULL)
        assert(false);
    return (*s)->a;
}

void print_up(alg *a)
{
    if (a == NULL)
        return;
    print_up(a->parent);
    printf("%c", a->lettre);
}

void affiche(alg *a)
{
    stack s = init_stack();
    push(s, a);
    while (*s != NULL)
    {
        alg *a = pop(s);
        if (a->end)
        {
            print_up(a);
            printf("\n");
        }
        if (a->fils != NULL)
            push(s, a->fils);
        if (a->frere != NULL)
            push(s, a->frere);
    }
}

void insert_word(alg *a, char *word)
{
    int i = 0;
    while (word[i] != '\0')
    {
        while (a->lettre != word[i])
        {
            if (a->frere == NULL)
            {
                a->frere = malloc(sizeof(alg));
                a->frere->lettre = word[i];
                a->frere->fils = NULL;
                a->frere->frere = NULL;
                a->frere->end = false;
                a->frere->parent = a->parent;
                a = a->frere;
                break;
            }
            a = a->frere;
        }
        if (word[i + 1] == '\0')
        {
            a->end = true;
            break;
        }
        if (a->fils == NULL)
        {
            a->fils = malloc(sizeof(alg));
            a->fils->lettre = word[i + 1];
            a->fils->fils = NULL;
            a->fils->frere = NULL;
            a->fils->end = false;
            a->fils->parent = a;
        }

        a = a->fils;
        i++;
    }
}

void delete_word(alg *a, char *word)
{
    int i = 0;
    while (word[i] != '\0')
    {
        while (a->lettre != word[i])
        {
            if (a->frere == NULL)
                return;
            a = a->frere;
        }
        if (word[i + 1] == '\0')
        {
            a->end = false;
            break;
        }
        if (a->fils == NULL)
            return;
        a = a->fils;
        i++;
    }
}

int main()
{
    alg *a = exemples();
    printf("##########\n");
    affiche(a);
    insert_word(a, "mat");
    printf("##########\n");
    affiche(a);
    insert_word(a, "matelasse");
    insert_word(a, "matrices");
    insert_word(a, "bibelots");
    insert_word(a, "matelots");
    printf("##########\n");
    affiche(a);
    delete_word(a, "matelasse");
    delete_word(a, "matelots");
    delete_word(a, "mat");
    printf("##########\n");
    affiche(a);
    return 0;
}