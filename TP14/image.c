/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Wpedantic -O -g image.c -o image
Date	: 13.01.2023
____________________
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "image.h"
#include "ppm.h"

couleur blanc = {.r = 255, .g = 255, .b = 255};

/*************************/
/* Les fonctions à écrire */
/*************************/

// #args: ^-Werror

couleur melange2(couleur c1, couleur c2)
{
  couleur res;
  res.r = (c1.r + c2.r) / 2;
  res.g = (c1.g + c2.g) / 2;
  res.b = (c1.b + c2.b) / 2;

  return res;
}

couleur melange4(couleur c1, couleur c2, couleur c3, couleur c4)
{
  couleur res;
  res.r = (c1.r + c2.r + c3.r + c4.r) / 4;
  res.g = (c1.g + c2.g + c3.g + c4.g) / 4;
  res.b = (c1.b + c2.b + c3.b + c4.b) / 4;

  return res;
}

bool meme_couleur(couleur c1, couleur c2)
{
  return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

/*************************/
/* Les fonctions fournies */
/*************************/

image *canevas(int largeur, int hauteur, couleur fond)
{
  image *res = (image *)malloc(sizeof(image));
  res->larg = largeur;
  res->haut = hauteur;
  res->c = (couleur **)malloc(hauteur * sizeof(couleur *));
  for (int j = 0; j < res->haut; j = j + 1)
  {
    res->c[j] = (couleur *)malloc(largeur * sizeof(couleur));
    for (int i = 0; i < res->larg; i = i + 1)
    {
      res->c[j][i] = fond;
    }
  }

  return res;
}
