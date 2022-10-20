/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g plus_petite_distance.c -o plus_petite_distance
Date	: 21.10.2022
____________________
*/
#include <assert.h>
#include <math.h>

double plus_petite_distance(double *tab, int size)
{
  double ppd = INFINITY;
  double prec = tab[0];

  for (int i = 1; i < size; i++)
  {
    if (tab[i] - prec < ppd)
      ppd = tab[i] - prec;
    prec = tab[i];
  }
  return ppd;
}

int main(void)
{
  double tab[] = {-4.3, -3.1, 0, 1.2, 5.6, 6.3};
  assert(fabs(plus_petite_distance(tab, 6) - .7) < 0.00001);

  return 0;
}
