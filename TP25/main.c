/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -g -lm main.c -o main
Date	: 12.05.2023
Run 	: ./main filtre && display edit_paul_valery.pgm
____________________
*/
#include "contours.h"

const int haut = 400;
const int larg = 600;

// #args: -lm
// #run: filtre \&& display edit_paul_valery.pgm

double appliquer_filtre_pos(int src[haut][larg], int x, int y, filtre f)
{
  double res = 0.0;
  int start = -((FILTRE_TAILLE - 1) / 2);
  int end = (FILTRE_TAILLE - 1) / 2;
  for (int i = start; i <= end; i++)
  {
    for (int j = start; j <= end; j++)
    {
      if (x + j < 0 || x + j >= larg || y + i < 0 || y + i >= haut)
        continue;
      res += src[y + i][x + j] * f[i - start][j - start];
    }
  }
  return res;
}

void appliquer_filtre(int src[haut][larg], int dest[haut][larg], filtre f)
{
  for (int i = 0; i < haut; i++)
  {
    for (int j = 0; j < larg; j++)
    {
      dest[i][j] = appliquer_filtre_pos(src, j, i, f);
    }
  }
}

void appliquer_2_filtres(int src[haut][larg], int dest[haut][larg], filtre f1, filtre f2, void (*combine)(int src1[haut][larg], int src2[haut][larg], int dest[haut][larg]))
{
  int tmp1[haut][larg];
  int tmp2[haut][larg];
  appliquer_filtre(src, tmp1, f1);
  appliquer_filtre(src, tmp2, f2);
  combine(tmp1, tmp2, dest);
}

void scale_filter(filtre f, double s)
{
  for (int i = 0; i < FILTRE_TAILLE; i++)
  {
    for (int j = 0; j < FILTRE_TAILLE; j++)
    {
      f[i][j] *= s;
    }
  }
}

void dist_combine2(int src1[haut][larg], int src2[haut][larg], int dest[haut][larg])
{
  for (int i = 0; i < haut; i++)
  {
    for (int j = 0; j < larg; j++)
    {
      dest[i][j] = sqrt((pow(src1[i][j], 2) + pow(src2[i][j], 2)));
    }
  }
}
void dist_combine3(int src1[haut][larg], int src2[haut][larg], int src3[haut][larg], int dest[haut][larg])
{
  for (int i = 0; i < haut; i++)
  {
    for (int j = 0; j < larg; j++)
    {
      dest[i][j] = sqrt((pow(src1[i][j], 2) + pow(src2[i][j], 2) + pow(src3[i][j], 2)));
    }
  }
}

void autoscale_filter(filtre f, double mult)
{
  double s = 0.0;
  for (int i = 0; i < FILTRE_TAILLE; i++)
  {
    for (int j = 0; j < FILTRE_TAILLE; j++)
    {
      s += f[i][j];
    }
  }
  if (s == 0.0)
  {
    printf("S=0 \n");
    return;
  }
  s = mult / s;
  for (int i = 0; i < FILTRE_TAILLE; i++)
  {
    for (int j = 0; j < FILTRE_TAILLE; j++)
    {
      f[i][j] *= s;
    }
  }
}

double derive_pos(int src[haut][larg], int x, int y)
{
  if (x == 0 || y == 0)
    return 0.0;
  return sqrt(
      pow(src[y][x] - src[y][x - 1], 2) +
      pow(src[y][x] - src[y - 1][x], 2));
}

void derive(int src[haut][larg], int dest[haut][larg])
{
  for (int i = 0; i < haut; i++)
  {
    for (int j = 0; j < larg; j++)
      dest[i][j] = derive_pos(src, j, i);
  }
}
void lire_image_pgm(char *fic, int img[haut][larg], int *max)
{
  FILE *f = fopen(fic, "r");
  if (f == NULL)
  {
    printf("Erreur lors de l'ouverture du fichier\n");
  }

  int version, largeur, hauteur;
  fscanf(f, "P%d\n%d %d\n%d", &version, &largeur, &hauteur, max);
  if (version != 2)
  {
    printf("Le fichier n'est pas au format PPM\n");
  }

  assert(larg == largeur && haut == hauteur);

  for (int i = 0; i < haut; i++)
  {
    for (int j = 0; j < larg; j++)
      fscanf(f, " %d", &img[i][j]);
  }

  fclose(f);
}

void ecrire_image_pgm(char *fic, int img[haut][larg], int max)
{
  FILE *f = fopen(fic, "w");
  if (f == NULL)
  {
    printf("Erreur lors de l'ouverture du fichier\n");
  }

  fprintf(f, "P2\n%d %d\n%d\n", larg, haut, max);

  for (int i = 0; i < haut; i++)
  {
    for (int j = 0; j < larg; j++)
      fprintf(f, "%d ", img[i][j]);
    fprintf(f, "\n");
  }

  fclose(f);
}

void seuil(int img[haut][larg], int s)
{
  for (int i = 0; i < haut; i++)
  {
    for (int j = 0; j < larg; j++)
      img[i][j] = img[i][j] > s ? 255 : 0;
  }
}

int main(int argc, char *argv[])
{
  int img[haut][larg], transforme1[haut][larg], transforme2[haut][larg], transforme[haut][larg];
  int max;

  // check if the first argument is "derive"
  if (argc > 1 && strcmp(argv[1], "derive") == 0)
  {
    lire_image_pgm("paul_valery.pgm", img, &max);
    derive(img, transforme);
    seuil(transforme, 4000);
    ecrire_image_pgm("edit_paul_valery.pgm", transforme, max);
  }
  else if (argc > 1 && strcmp(argv[1], "filtre") == 0)
  {
    filtre Prewittv = {
        {-1, 0, 1},
        {-1, 0, 1},
        {-1, 0, 1}};

    filtre Prewitth = {
        {-1, -1, -1},
        {0, 0, 0},
        {1, 1, 1}};

    lire_image_pgm("paul_valery.pgm", img, &max);
    // autoscale_filter(Prewitt, 1.0);
    // scale_filter(Prewitt, 1.0 / 3.0);
    appliquer_filtre(img, transforme1, Prewittv);
    appliquer_filtre(img, transforme2, Prewitth);
    dist_combine2(transforme1, transforme2, transforme);
    ecrire_image_pgm("edit_paul_valery.pgm", transforme, max);
  }

  return 0;
}
