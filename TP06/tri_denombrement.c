/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g tri_denombrement.c -o tri_denombrement
Date	: 23.10.2022
____________________
*/
#include <assert.h>
#include <stdlib.h>

/*
Le tri par dénombrement peut être utilisé dans le cas où on doit trier des entiers positifs pas nécessairement distincts dont on connaît un majorant. Il est efficace si ce majorant n'est pas trop grand par rapport au nombre d'entiers à trier.
Son principe est le suivant : on parcourt les données et pour chaque valeur qui apparaît, on maintient à jour un compteur qui nous permet de savoir combien de fois chaque valeur a été vue. Avec ces nombres d'occurences, on peut à la fin reconstruire les données dans l'ordre croissant.

1. Écrire et documenter une fonction `tri_denombrement` qui prend dans l'ordre un tableau d'entiers supposés positifs ou nuls, sa taille et un majorant (sans faire de vérification), et renvoie un nouveau tableau avec les données dans l'ordre croissant.
2. Quelle est la complexité du tri par dénombrement ?

La complexité du tri par dénombrement est en $\theta(n+m)$ où $n$ est la taille du tableau et $m$ le majorant.
*/

int *tri_denombrement(int *t, int n, int majorant)
{
  int *result = malloc(n * sizeof(int));
  int *compteur = malloc((majorant + 1) * sizeof(int));
  for (int i = 0; i < majorant + 1; i++)
  {
    compteur[i] = 0;
  }
  for (int i = 0; i < n; i++)
  {
    compteur[t[i]]++;
  }
  int j = 0;
  for (int i = 0; i < majorant + 1; i++)
  {
    for (int k = 0; k < compteur[i]; k++)
    {
      result[j] = i;
      j++;
    }
  }
  free(compteur);
  return result;
}

int main(void)
{
  int a1[] = {1};
  int *a2 = tri_denombrement(a1, 1, 5);
  int b1[] = {1, 1, 1};
  int *b2 = tri_denombrement(b1, 3, 5);
  int c1[] = {5, 4, 3, 2, 1, 0};
  int *c2 = tri_denombrement(c1, 6, 5);
  int d1[] = {2, 3, 1, 2, 0, 1, 5, 5, 4, 3, 0, 4};
  int *d2 = tri_denombrement(d1, 12, 5);

  assert(a2[0] == 1);
  free(a2);

  for (int i = 0; i < 3; i = i + 1)
  {
    assert(b2[i] == 1);
  }
  free(b2);

  for (int i = 0; i < 6; i = i + 1)
  {
    assert(c2[i] == i);
  }
  free(c2);

  for (int i = 0; i < 12; i = i + 1)
  {
    assert(d2[i] == i / 2);
  }
  free(d2);

  return 0;
}
