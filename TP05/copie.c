/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g copie.c -o copie
Date	: 21.10.2022
____________________
*/
//#
#include <assert.h>
#include <stdlib.h>

int *copie(int *tab, int size)
{
  int *new = malloc(size * sizeof(int));
  for (int i = 0; i < size; i++)
  {
    new[i] = tab[i];
  }
  return new;
}

int main(void)
{
  int a[] = {9};
  int *a2 = copie(a, 1);
  int b[] = {6, 10, -3, 4, 5};
  int *b2 = copie(b, 5);

  assert(a[0] == a2[0]);

  assert(b[0] == b2[0]);
  assert(b[4] == b2[4]);

  return 0;
}
