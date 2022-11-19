/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g vecteurs.c -o vecteurs
Date	: 19.11.2022
____________________
*/
#include "vecteurs.h"
#include <math.h>
#include <stdio.h>

/**
 * a : un vecteur
 * b : un vecteur
 * sortie : la somme des deux vecteurs
 */
struct vec vec_add(struct vec a, struct vec b)
{
    struct vec c = {.x = a.x + b.x, .y = a.y + b.y};
    return c;
}

/**
 * a : un double
 * b : un vecteur
 * sortie : le produit du vecteur par le double
 */
struct vec vec_mult(double a, struct vec b)
{
    struct vec c = {.x = a * b.x, .y = a * b.y};
    return c;
}

/**
 * a : un vecteur
 * sortie : la norme du vecteur
 */
double vec_norm(struct vec a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

/**
 * a : un vecteur
 * sortie : le vecteur normalisé (vecteur unitaire ou de norme 1)
 */
struct vec vec_unit(struct vec a)
{
    double norm = vec_norm(a);
    struct vec b = {.x = a.x / norm, .y = a.y / norm};
    return b;
}

/**
 * a : un vecteur
 * b : un vecteur
 * d : une norme (peut être négative)
 * sortie : un vecteur de norme d, allant de a vers b
 */
struct vec vec_towards(struct vec a, struct vec b, double d)
{
    struct vec c = vec_add(b, vec_mult(-1, a));
    return vec_mult(d, vec_unit(c));
}

/**
 * a : un vecteur
 * sortie : affiche le vecteur
 */
void print_vec(char prefix[], struct vec a)
{
    printf("%s : (%f, %f)\n", prefix, a.x, a.y);
}