#ifndef _VECTEURS_H
#define _VECTEURS_H

/* Vecteurs */
struct vec
{
    double x, y;
};
struct vec vec_add(struct vec a, struct vec b);
struct vec vec_mult(double a, struct vec b);
double vec_norm(struct vec a);
struct vec vec_unit(struct vec a);
struct vec vec_towards(struct vec a, struct vec b, double d);

// TODO: Supprimer prefix avant d'envoyer
void print_vec(char prefix[], struct vec a);

#endif // _VECTEURS_H