#ifndef _OISEAUX_H
#define _OISEAUX_H

#include <stdbool.h>

// On a enfin vu les define
#define IMG_SIZE 300
#define SPEED (double)0.5
#define IMG_STEPS 200

struct oiseau
{
    double x, y; // entre 0 et 1 pour être visible
    int ami;     // indice de l'ami dans la nuee (voir ci-dessous)
    int ennemi;  // indice de l'ennemi dans la nuee
};

struct nuee
{
    int taille; // nombre d'oiseaux
    struct oiseau *cui;
};

void ecrire_image(int n, bool **img, int largeur, int hauteur);
int hasard(int max, int avoid);

void deplacement_oiseau(struct nuee *nuee, struct oiseau *oiseau, struct oiseau *nouveau);
struct nuee *alloue_nuee(int n);
struct nuee *init_nuee(int n);
void deplacement(struct nuee *in, struct nuee *out);
void change_amities(struct nuee *in);
bool **image(struct nuee *nuee);

#endif // _OISEAUX_H
