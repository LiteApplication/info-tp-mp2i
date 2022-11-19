/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g oiseaux.c vecteurs.c -o oiseaux -lm
Date	: 19.11.2022
Run 	: ./oiseaux && convert -delay .1 -loop 0 'oiseaux-*.ppm' oiseaux.gif && rm oiseaux-*.ppm
____________________
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "oiseaux.h"
#include "vecteurs.h"
#include "tests_oiseaux.h"

//#package: oiseaux.h vecteurs.h
//#files: vecteurs.c
//#libs: -lm

/** n : un entier pour le nom de l'image
 * img : un tableau de booleen tel que sorti par la fonction image, de dimensions largeurxhauteur
 * sortie : entete écrite dans le fichier serpent-N.ppm, où N est l'entier n sur 3 chiffres
 */
void ecrire_image(int n, bool **img, int largeur, int hauteur)
{
    char fichier[20];
    FILE *file;
    char couleur[2][7] = {"1 1 1 ", "0 0 0 "};

    sprintf(fichier, "oiseaux-%03d.ppm", n);
    file = fopen(fichier, "w");
    fprintf(file, "P3\n%d %d\n1\n", largeur, hauteur);
    for (int j = 0; j < hauteur; j = j + 1)
    {
        for (int i = 0; i < largeur; i = i + 1)
        {
            fprintf(file, "%s", couleur[img[j][i]]);
        }
    }
    fclose(file);
}

/** max: nombre maximal atteignable par la fonction hasard
 * avoid: nombre à éviter
 * sortie: un entier entre 0 et max-1 (inclus), différent de avoid
 */
int hasard(int max, int avoid)
{
    int n = random() % (max - 1);
    if (n >= avoid)
        n += 1;
    return n;
}

/**
 * nuee: pointeur vers une nuee d'oiseaux
 * o_in: pointeur vers l'oiseau à traiter
 * o_out: pointeur vers l'oiseau à modifier
 */
void deplacement_oiseau(struct nuee *nuee, struct oiseau *o_in, struct oiseau *o_out)
{
    struct vec center = {.x = 0.5, .y = 0.5};
    struct vec o = {.x = o_in->x, .y = o_in->y};
    struct vec ami = {.x = nuee->cui[o_in->ami].x, .y = nuee->cui[o_in->ami].y};
    struct vec ennemi = {.x = nuee->cui[o_in->ennemi].x, .y = nuee->cui[o_in->ennemi].y};

    struct vec v1 = vec_towards(o, center, 0.01);
    struct vec v2 = vec_towards(o, ami, 0.05);
    struct vec v3 = vec_towards(o, ennemi, -0.03);

    struct vec v = vec_add(v1, vec_add(v2, v3));

    o_out->x = o_in->x + v.x;
    o_out->y = o_in->y + v.y;
    o_out->ami = o_in->ami;
    o_out->ennemi = o_in->ennemi;
}

/**
 * n: nombre d'oiseaux dans la nuee
 * sortie: pointeur vers une nuee d'oiseaux
 */
struct nuee *alloue_nuee(int n)
{
    struct nuee *nuee = malloc(sizeof(struct nuee));
    nuee->taille = n;
    nuee->cui = malloc(n * sizeof(struct oiseau));
    return nuee;
}

/**
 * n: nombre d'oiseaux dans la nuee
 * sortie: pointeur vers une nuee d'oiseaux initialisée
 * Cette fonction alloue une nuee d'oiseaux, et initialise les positions et
 * les amitiés/inimitiés des oiseaux.
 */

struct nuee *init_nuee(int n)
{
    struct nuee *nuee = alloue_nuee(n);
    for (int i = 0; i < n; i++)
    {
        nuee->cui[i].x = random() / (double)RAND_MAX; // entre 0 et 1
        nuee->cui[i].y = random() / (double)RAND_MAX; // (double) est nécessaire pour la division
        nuee->cui[i].ami = hasard(n, i);              // On évite de s'aimer soi-même
        nuee->cui[i].ennemi = hasard(n, i);
    }
    return nuee;
}

/**
 * in: pointeur vers une nuee d'oiseaux
 * out: pointeur vers une nuee d'oiseaux
 * Cette fonction calcule le déplacement de chaque oiseau de la nuee in et le stocke dans la nuee out
 */
void deplacement(struct nuee *in, struct nuee *out)
{
    for (int i = 0; i < in->taille; i++)
    {
        deplacement_oiseau(in, &in->cui[i], &out->cui[i]);
    }
}

/**
 * nuee: pointeur vers une nuee d'oiseaux dont il faut changer les amis et ennemis
 */
void change_amities(struct nuee *nuee)
{
    for (int i = 0; i < nuee->taille; i++)
    {
        nuee->cui[i].ami = hasard(nuee->taille, i);
        nuee->cui[i].ennemi = hasard(nuee->taille, i);
    }
}

bool **image(struct nuee *nuee)
{
    // Malloc tout ce qu'il faut
    bool **img = malloc(IMG_SIZE * sizeof(bool *));
    for (int i = 0; i < IMG_SIZE; i++)
    {
        img[i] = malloc(IMG_SIZE * sizeof(bool));
    }

    // Remplir l'image de true
    for (int i = 0; i < IMG_SIZE; i++)
    {
        for (int j = 0; j < IMG_SIZE; j++)
        {
            img[i][j] = true;
        }
    }

    // Placer chaque oiseau dans l'image
    for (int i = 0; i < nuee->taille; i++)
    {
        int x = (int)(nuee->cui[i].x * IMG_SIZE);
        int y = (int)(nuee->cui[i].y * IMG_SIZE);
        if (x >= 0 && x < IMG_SIZE && y >= 0 && y < IMG_SIZE)
        {
            img[x][y] = false;
        }
    }

    return img;
}

void free_image(bool **img)
{
    for (int i = 0; i < IMG_SIZE; i++)
    {
        free(img[i]);
    }
    free(img);
}

int main()
{
    //#run: \&& convert -delay .1 -loop 0 oiseaux-*.ppm oiseaux.gif \&& rm \oiseaux-*.ppm

    // Initialisation
    struct nuee *n1 = init_nuee(300);
    struct nuee *n2 = alloue_nuee(300);

    // Création des images
    for (int i = 0; i < IMG_STEPS; i++)
    {
        bool **img = image(n1);
        if (i % 30 == 27) // Pas de signification particulière
            change_amities(n2);

        ecrire_image(i, img, IMG_SIZE, IMG_SIZE);
        free_image(img);

        deplacement(n1, n2);
        struct nuee *tmp = n1;
        n1 = n2;
        n2 = tmp;
    }

    // Libération de la mémoire
    free(n1->cui);
    free(n1);
    free(n2->cui);
    free(n2);
}