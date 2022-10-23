/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g Koch.c -o Koch
Date	: 23.10.2022
Run 	: Koch > tmp.txt && gnuplot -p tmp.txt && rm tmp.txt
____________________
*/
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#define ALPHA 3.14159265359 / 3

double *init_x()
{
  double *x = malloc(2 * sizeof(double));
  x[0] = 0.0;
  x[1] = 1.0;
  return x;
}

double *init_y()
{
  double *y = malloc(2 * sizeof(double));
  y[0] = 0.0;
  y[1] = 0.0;
  return y;
}

/*
Écrire une fonction suivante_x qui prend en argument deux tableaux représentants respectivement
les abscisses et les ordonnées des sommets à une étape donnée dans la construction de la courbe
de Koch, et le nombre de sommets en question (sans avoir à le vérifier) et renvoie un tableau
avec les abscisses des sommets à l’étape suivante.
*/
// x3=x1+(x2−x1)cosα−(y2−y1)sinα
// y3=y1+(x2−x1)sinα+(y2−y1)cosα

double *suivante_x(double *xs, double *ys, int n)
{
  int next_size = (n - 1) * 4 + 1;
  double *result = malloc(next_size * sizeof(double));
  for (int i = 0; i < next_size; i++)
  {
    result[i] = 0;
  }
  for (int i = 0; i < n - 1; i++)
  {
    double dy = (ys[i + 1] - ys[i]) / 3; // Distance entre les abscisses de la base du triangle

    double x1 = xs[i];
    double x5 = xs[i + 1];
    double x2 = x1 + (x5 - x1) / 3;
    double x4 = x5 - (x5 - x1) / 3;
    double x3 = x2 + (x4 - x2) * cos(ALPHA) - dy * sin(ALPHA);

    // Enregistre les résultats dans la liste
    result[i * 4 + 0] = x1;
    result[i * 4 + 1] = x2;
    result[i * 4 + 2] = x3;
    result[i * 4 + 3] = x4;
    result[i * 4 + 4] = x5;
  }
  return result;
}
double *suivante_y(double *xs, double *ys, int n)
{
  int next_size = (n - 1) * 4 + 1;
  double *result = malloc(next_size * sizeof(double));
  for (int i = 0; i < next_size; i++)
  {
    result[i] = 0;
  }
  for (int i = 0; i < n - 1; i++)
  {
    double dx = (xs[i + 1] - xs[i]) / 3;

    double y1 = ys[i];
    double y5 = ys[i + 1];
    double y2 = y1 + (y5 - y1) / 3;
    double y4 = y5 - (y5 - y1) / 3;
    double y3 = y2 + (y4 - y2) * cos(ALPHA) + dx * sin(ALPHA);

    result[i * 4 + 0] = y1;
    result[i * 4 + 1] = y2;
    result[i * 4 + 2] = y3;
    result[i * 4 + 3] = y4;
    result[i * 4 + 4] = y5;
  }
  return result;
}

void affichage(int n)
{
  double *xs, *ys, *txs, *tys;
  int np;
  xs = init_x();
  ys = init_y();

  np = 2;

  for (int i = 0; i < n; i++)
  {
    txs = suivante_x(xs, ys, np);
    tys = suivante_y(xs, ys, np);
    free(xs);
    free(ys);
    xs = txs;
    ys = tys;
    np = (np - 1) * 4 + 1;
  }

  for (int i = 0; i < np; i++)
  {
    printf("%f %f\n", xs[i], ys[i]);
  }

  return;
}

// Met en forme la sortie pour utiliser directement gnuplot (voir "Run :" dans l'en-tête)
void setup_gnuplot()
{
  ///#run: \> tmp.txt \&& gnuplot -p tmp.txt \&& rm tmp.txt
  printf("set style data lines\n"
         "set xrange [-0.2:1.2]\n"
         "set yrange [-0.1:0.8]\n"
         "set size ratio -1\n"
         "plot '-' with lines\n");
}

void tests_question1()
{
  double epsilon = 0.00001;
  double *test = init_x();

  assert(fabs(test[0]) < epsilon);
  assert(fabs(test[1] - 1) < epsilon);
  free(test);
}

void tests_question2()
{
  double epsilon = 0.00001;
  double *test = init_y();

  assert(fabs(test[0]) < epsilon);
  assert(fabs(test[1]) < epsilon);
  free(test);
}

int main(void)
{
  tests_question1();
  tests_question2(); // a decommenter
  setup_gnuplot();
  //#run: \> /dev/null
  affichage(3); // a decommenter (en commentant les tests precedents)
  return 0;
}
