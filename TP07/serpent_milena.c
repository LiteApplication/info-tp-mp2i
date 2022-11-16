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
struct serpent *new_serpent(int taille)
{
  // 1. On alloue la mémoire pour le serpent
  struct serpent *s = (struct serpent *)malloc(sizeof(struct serpent));
  // 2. On vérifie l'argument
  if (taille < 1)
  {
    return NULL;
  }
  // 3. On initialise le serpent avec la bonne taille
  s->tete = 0;
  s->taille = taille;
  // 4. On alloue la mémoire pour la queue
  s->ecailles = (int *)malloc(taille * sizeof(int));
  // 5. On retourne le serpent
  return s;
}
// free_serpent
void free_serpent(struct serpent *s)
{
  //#args: ^-Werror
  // 1. On libère la queue
  free(s->ecailles);
  // 2. On libère le serpent
  free(s);
}

// avance
void avance(struct serpent *s) // s is a pointer to a struct serpent
{
  int mv = rand() % 3 - 1;                // mv is an integer between -1 and 1
  s->tete += 1;                           // Increment the head position
  for (int i = 0; i < s->taille - 1; i++) // Loop on the body
  {
    s->ecailles[s->taille - 1 - i] = s->ecailles[s->taille - 2 - i]; // Move the body forward
  }
  s->ecailles[0] += mv; // Move the head forward
}
// image
// trajet

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
  //  trajet(15, 100, 80);  // pour générer les images ppm
}
