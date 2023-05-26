#ifndef _CONTOURS_H
#define _CONTOURS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

extern const int haut;
extern const int larg;
#define FILTRE_TAILLE 3

typedef double filtre[FILTRE_TAILLE][FILTRE_TAILLE];

void appliquer_filtre(int src[haut][larg], int dest[haut][larg], filtre f);
double appliquer_filtre_pos(int src[haut][larg], int x, int y, filtre f);
void scale_filter(filtre f, double s);

void derive(int src[haut][larg], int dest[haut][larg]);
double derive_pos(int src[haut][larg], int i, int j);
void seuil(int img[haut][larg], int s);

void lire_image_pgm(char *fic, int img[haut][larg], int *max);
void ecrire_image_pgm(char *fic, int img[haut][larg], int max);

#endif
