/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g premiers_termes.c -o premiers_termes
Date	: 23.10.2022
____________________
*/
#include <assert.h>
#include <math.h>
#include <stdlib.h>
/*
On considère une suite numérique $(u_n)_{n>= 0}$ donnée par son premier terme $(u_0)$ et la relation de récurrence $u_{n+1}=au_n+b$, où $u_0$, $a$ et $b$ sont des réels. Le but de cet exercice est de calculer les premiers termes de cette suite.

Écrire et documenter une fonction `premiers_termes` dont les paramètres sont, dans l'ordre, le terme initial de la suite, les coefficients $a$ et $b$, et le nombre de termes à calculer $n$ et renvoie un tableau qui contient les $n$ premiers termes de la suite.
*/

double *premiers_termes(double u0, double a, double b, int n)
{
  double *result = malloc(n * sizeof(double));
  result[0] = u0;
  for (int i = 1; i < n; i++)
  {
    result[i] = a * result[i - 1] + b;
  }
  return result;
}

int main(void)
{
  double epsilon = 0.0001;
  double *u = premiers_termes(2, 3, 0, 35);
  double *v = premiers_termes(-1, 1, 1.7, 28);
  double *w = premiers_termes(3.14, 0, 1.4142, 53);

  assert(fabs(u[0] - 2) < epsilon);
  assert(fabs(u[10] - 2 * pow(3, 10)) < epsilon);
  assert(fabs(u[34] - 2 * pow(3, 34)) < epsilon);

  assert(fabs(v[0] + 1) < epsilon);
  assert(fabs(v[21] - (-1 + 1.7 * 21)) < epsilon);
  assert(fabs(v[27] - (-1 + 1.7 * 27)) < epsilon);

  assert(fabs(w[0] - 3.14) < epsilon);
  assert(fabs(w[52] - 1.4142) < epsilon);

  return 0;
}
