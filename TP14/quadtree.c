/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Wpedantic -O -g -Werror quadtree.c image.c ppm.c -o quadtree
Date	: 13.01.2023
____________________
*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "quadtree.h"
#include "image.h"
#include "ppm.h"

/*************************/
/* Les fonctions à écrire */
/*************************/

/* Image
+----+----+
| SO | SE |
+----+----+
| NO | NE |
+----+----+
*/

// #args: ^-Werror

// #files: image.c ppm.c
quadtree *image2quadtree(image *img, int debi, int debj, int taille)
{

  quadtree *q = malloc(sizeof(quadtree));
  q->taille = taille;
  assert(taille != 0);
  if (taille == 1)
  {
    q->so = NULL;
    q->se = NULL;
    q->ne = NULL;
    q->no = NULL;

    q->c = img->c[debj][debi];
    return q;
  }
  q->so = image2quadtree(img, debi, debj, taille / 2);
  q->se = image2quadtree(img, debi + taille / 2, debj, taille / 2);
  q->ne = image2quadtree(img, debi + taille / 2, debj + taille / 2, taille / 2);
  q->no = image2quadtree(img, debi, debj + taille / 2, taille / 2);

  q->c = melange4(q->so->c, q->se->c, q->ne->c, q->no->c);

  return q;
}

void simplifier(quadtree *q)
{
  if (est_feuille(q))
  {
    return;
  }
  if (meme_couleur(q->so->c, q->se->c) && meme_couleur(q->so->c, q->ne->c) && meme_couleur(q->so->c, q->no->c))
  {
    liberer_quadtree(q->so);
    liberer_quadtree(q->se);
    liberer_quadtree(q->ne);
    liberer_quadtree(q->no);
    q->so = NULL;
    q->se = NULL;
    q->ne = NULL;
    q->no = NULL;
    return;
  }
  simplifier(q->so);
  simplifier(q->se);
  simplifier(q->ne);
  simplifier(q->no);
}

void quadtree2image(quadtree *q, image *img, int debi, int debj, int taille)
{
  if (est_feuille(q))
  {
    for (int i = 0; i < q->taille; i++)
    {
      for (int j = 0; j < q->taille; j++)
      {
        img->c[debj + j][debi + i] = q->c;
      }
    }
    return;
  }

  quadtree2image(q->so, img, debi, debj, q->taille / 2);
  quadtree2image(q->se, img, debi + q->taille / 2, debj, q->taille / 2);
  quadtree2image(q->ne, img, debi + q->taille / 2, debj + q->taille / 2, q->taille / 2);
  quadtree2image(q->no, img, debi, debj + q->taille / 2, q->taille / 2);
}

void liberer_quadtree(quadtree *q)
{
  if (q == NULL)
  {
    return;
  }
  liberer_quadtree(q->so);
  liberer_quadtree(q->se);
  liberer_quadtree(q->ne);
  liberer_quadtree(q->no);
  free(q);
}

quadtree *reduire(quadtree *q, int taille)
{
  if (q == NULL)
  {
    return NULL;
  }
  if (taille == 0)
  {
    liberer_quadtree(q);
    return NULL;
  }
  q->taille = taille;
  if (est_feuille(q))
  {
    return q;
  }
  q->so = reduire(q->so, taille / 2);
  q->se = reduire(q->se, taille / 2);
  q->ne = reduire(q->ne, taille / 2);
  q->no = reduire(q->no, taille / 2);
  return q;
}

void tourner(quadtree *q)
{
  if (est_feuille(q))
  {
    return;
  }

  quadtree *tmp = q->so;
  q->so = q->no;
  q->no = q->ne;
  q->ne = q->se;
  q->se = tmp;

  tourner(q->so);
  tourner(q->se);
  tourner(q->ne);
  tourner(q->no);
}

/*************************/
/* Les fonctions fournies */
/*************************/

bool est_feuille(quadtree *q)
{
  return q == NULL || (q->se == NULL && q->so == NULL && q->ne == NULL && q->no == NULL);
}

int taillei(quadtree *q)
{
  if (q == NULL || est_feuille(q))
  {
    return 0;
  }
  return 1 + taillei(q->se) + taillei(q->so) + taillei(q->ne) + taillei(q->no);
}

quadtree *img2qt(image *img)
{
  if (img == NULL || img->haut != img->larg)
  {
    return NULL;
  }
  return image2quadtree(img, 0, 0, img->haut);
}

image *qt2img(quadtree *q)
{
  image *img;
  if (q == NULL)
  {
    return NULL;
  }

  img = (image *)malloc(sizeof(image));
  img->larg = q->taille;
  img->haut = q->taille;
  img->c = (couleur **)malloc(img->haut * sizeof(couleur *));
  for (int j = 0; j < img->haut; j = j + 1)
  {
    img->c[j] = (couleur *)malloc(img->larg * sizeof(couleur));
  }
  quadtree2image(q, img, 0, 0, q->taille);

  return img;
}

int main(void)
{
  // TODO tests

  // tests
  couleur c1 = {.r = 255, .g = 0, .b = 0};
  assert(meme_couleur(c1, c1));
  couleur c2 = {.r = 0, .g = 255, .b = 0};
  assert(!meme_couleur(c1, c2));

  assert(melange2(c1, c2).r == 127);
  assert(melange2(c1, c2).g == 127);
  assert(melange2(c1, c2).b == 0);

  couleur c3 = {.r = 0, .g = 0, .b = 255};
  couleur c4 = {.r = 255, .g = 255, .b = 0};
  assert(melange4(c1, c2, c3, c4).r == 127);
  assert(melange4(c1, c2, c3, c4).g == 127);
  assert(melange4(c1, c2, c3, c4).b == 63);

  // On affiche la taille de l'image et du quadtree
  image *img = lecture("ordi.ppm");
  quadtree *q = img2qt(img);
  fprintf(stderr, "Taille de l'image: %d x %d\n", img->larg, img->haut);
  fprintf(stderr, "Taille du quadtree: %d x %d\n", q->taille, q->taille);
  fprintf(stderr, "Nb de pixels: %d\n", img->larg * img->haut);
  fprintf(stderr, "Nb de pixels (arbre): %d\n", taillei(q) * 3 + 1);
  fprintf(stderr, "Nb de noeuds: %d\n", taillei(q));

  // simplifier(q);
  fprintf(stderr, "Nb de noeuds apres simplification: %d\n", taillei(q));
  tourner(q);
  tourner(q);
  reduire(q, 256);
  affiche(qt2img(q));

  // affiche(qt2img(img2qt(lecture("arc.ppm"))));
  // affiche(qt2img(reduire(img2qt(lecture("img.ppm")), 64)));
}
