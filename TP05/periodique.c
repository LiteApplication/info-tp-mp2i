/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g periodique.c -o periodique
Date	: 21.10.2022
____________________
*/
#include <assert.h>
#include <stdbool.h>

bool periodique(int *tab, int size, int p)
{
  for (int i = 0; i < size - p; i++)
  {
    if (tab[i] != tab[i + p])
      return false;
  }
  return true;
}

int main(void)
{
  int t[] = {1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2};
  int t2[] = {1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 1};

  assert(periodique(t, 11, 3));
  assert(!periodique(t, 11, 2));
  assert(!periodique(t2, 11, 3));

  return 0;
}
