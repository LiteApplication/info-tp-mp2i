/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g serpent.c -o serpent
Date	: 16.11.2022
Run 	: ./serpent && convert -delay 1 -loop 0 serpent-*.ppm serpent.gif && rm -f serpent-*.ppm
____________________
*/

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

struct serpent
{
  int taille;    // nombre d'écailles, y compris la tête
  int tete;      // abscisse de la tête
  int *ecailles; // ordonnées des écailles
                 // l'écaille 0 est la tête
};

/** n : un entier pour le nom de l'image
 * img : un tableau de booleen tel que sorti par la fonction image, de dimensions largeurxhauteur
 * sortie : entete écrite dans le fichier serpent-N.ppm, où N est l'entier n sur 3 chiffres
 */
void ecrire_image(int n, bool **img, int largeur, int hauteur)
{
  char fichier[20];
  FILE *file;
  char couleur[2][7] = {"1 1 1 ", "0 0 0 "};

  sprintf(fichier, "serpent-%03d.ppm", n);
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

//////////////// code a ecrire
// new_serpent
/*
  lenght: la longueur du serpent à initialiser
*/
struct serpent *new_serpent(int lenght)
{
  ///#args: ^-Werror
  if (lenght <= 0)
    return NULL;
  struct serpent *result = (struct serpent *)malloc(sizeof(struct serpent));
  result->taille = lenght;
  result->tete = 0;
  // result->ecailles = (int *)calloc(lenght, sizeof(int));

  result->ecailles = (int *)malloc(lenght * sizeof(int));
  for (int i = 0; i < lenght; i++)
    result->ecailles[i] = 0;

  return result;
}

// free_serpent
/*
  s: un pointeur sur un serpent
*/
void free_serpent(struct serpent *s)
{
  free(s->ecailles);
  free(s);
}

// Print serpent
/*
  s: un pointeur sur un serpent
*/
void print_serpent(struct serpent *s)
{
  printf("Taille : %d, Tete(x) : %d, Ecailles : ", s->taille, s->tete);
  for (int i = 0; i < s->taille - 1; i++)
  {
    printf("%d  ", s->ecailles[i]);
  }
  printf("\n");
}
// avance
/*
  s: un pointeur sur un serpent à faire avancer
*/
void avance(struct serpent *s)
{
  // Faire avancer le serpent
  s->tete += 1;
  for (int i = s->taille - 2; i > -1; i--)
  {
    // printf("i = %d : ", i);
    // print_serpent(s);
    s->ecailles[i + 1] = s->ecailles[i];
  }
  // Est-ce que le serpent va monter ou descendre
  char dir = (rand() % 3) - 1;
  s->ecailles[0] += dir;

  // print_serpent(s);
}
// image
/*
  s: un pointeur sur un serpent
  width: la largeur de l'image à générer
  height: la hauteur de l'image à générer
*/
bool **image(struct serpent *s, int width, int height)
{
  // Initialise la hauteur
  bool **result = (bool **)malloc(height * sizeof(bool *));
  // Initialise les largeurs
  for (int i = 0; i < height; i++)
  {
    result[i] = (bool *)malloc(width * sizeof(bool));
    for (int x = 0; x < width; x++)
    {
      result[i][x] = 0;
    }
  }

  int middle_y = height / 2;
  for (int i = 0; i < s->taille; i++)
  {
    if (s->tete - i > 0 && s->tete - i - 1 < width)
      result[s->ecailles[i] + middle_y][s->tete - i - 1] = true;
  }
  return result;
}
// trajet
/*
  size: Taille du serpent
  width: Largeur de l'image
  height: Hauteur de l'image
*/
void trajet(int size, int width, int height)
{
  struct serpent *s = new_serpent(size);
  int steps = width + size;

  for (int t = 0; t < steps; t++)
  {
    bool **img = image(s, width, height);
    ecrire_image(t, img, width, height);
    avance(s);
    for (int i = 0; i < height; i++)
    {
      free(img[i]);
    }
    free(img);
  }

  free_serpent(s);
  //#run: \&& convert -delay 1 -loop 0 \serpent-*.ppm serpent.gif \&& rm -f \serpent-*.ppm
  //#package: serpent.gif
}

//////////////// debut des tests

void test_new_serpent()
{
  struct serpent *s = new_serpent(-1);
  assert(s == NULL);

  s = new_serpent(0);
  assert(s == NULL);

  s = new_serpent(15);
  assert(s != NULL);
  assert(s->taille == 15);
  assert(s->tete == 0);
  assert(s->ecailles[0] == 0);
  assert(s->ecailles[14] == 0);

  free_serpent(s); // a decommenter quand ce sera implemente
}

void test_avance()
{
  struct serpent *s = new_serpent(10);
  s->ecailles[0] = -1;
  s->ecailles[5] = 1;
  s->tete = 32;

  avance(s);
  assert(s->ecailles[1] == -1);
  assert(s->ecailles[6] == 1);
  assert(s->ecailles[9] == 0);
  assert(s->ecailles[0] >= -2 && s->ecailles[0] <= 0);
  assert(s->tete == 33);

  free_serpent(s);
}

//////////////// fin des tests

int main(void)
{
  test_new_serpent();
  test_avance();

  trajet(15, 100, 80); // pour générer les images ppm
}
