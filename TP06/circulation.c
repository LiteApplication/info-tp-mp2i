/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g circulation.c -o circulation
Date	: 21.10.2022
____________________
*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <wchar.h>
#include <locale.h>

/** frequence : la route est libre avec frequence 1/frequence
 * sortie : un entier entre 0 et 4 qui représente soit une portion de route libre, soit un type de véhicule
 */
uint8_t vehicule(int frequence)
{
  if (random() % frequence == 0)
  {
    return 0;
  }
  else
  {
    return (random() % 4) + 1;
  }
}

/*
  Initialise la route
*/
uint8_t *init(int size, int frequence)
{
  uint8_t *result = (uint8_t *)malloc(size * sizeof(uint8_t));

  for (int i = 0; i < size; i++)
  {
    result[i] = vehicule(frequence);
  }
  return result;
}

uint8_t *next(uint8_t *tab, int size, int frequence)
{
  uint8_t *result = (uint8_t *)malloc(size * sizeof(uint8_t));
  // Fill the table with 0s
  for (int i = 0; i < size; i++)
  {
    result[i] = 0;
  }

  // le véhicule en position 0 sort du champ de notre vue systématiquement,
  if (tab[0] != 0)
  {
    result[0] = 0;
  }
  // le véhicule en position 0 sort du champ de notre vue systématiquement,
  if (tab[size - 1] == 0)
  {
    result[size - 1] = vehicule(frequence);
  }

  for (int i = 1; i < size; i++)
  {
    if (tab[i] != 0)
    {
      if (tab[i - 1] == 0)
      {
        // si la case i−1 est libre au temps t, le véhicule v se retrouvera en case i−1 au temps t+1,
        result[i - 1] = tab[i];
      }
      else
      {
        // si la case i−1 est occupée au temps t, le véhicule v reste en case i au temps t+1.
        result[i] = tab[i];
      }
    }
  }
  return result;
}

/** route : état de la route
 * n : longueur de la route
 * sortie : affichage de la route
 */
void affiche(uint8_t *route, int n)
{
  // le type wint_t est hors programme
  wint_t voiture[] = {' ', (wint_t)128663, (wint_t)128665, (wint_t)128661, (wint_t)127949};
  // wint_t voiture[] = {' ', (wint_t)85, (wint_t)86, (wint_t)87, (wint_t)88};
  //  cette instruction est hors programme et permet l'affichage des caractères bizarres au-dessus;
  setlocale(LC_ALL, "");
  for (int i = 0; i < n; i++)
  {
    printf("%lc", voiture[route[i]]);
  }
  printf("\n");
}

void affichage()
{
  int n = 60;
  int frequence = 2;
  // initialisation de la route et affichage du temps 0
  uint8_t *route = init(n, frequence);
  affiche(route, n);
  // afficher jusqu'au temps 30
  // a vous de jouer

  uint8_t *tmp;
  for (int t = 0; t < 30; t++)
  {
    tmp = next(route, n, frequence);
    affiche(tmp, n);
    free(route);
    route = tmp;
  }
}

void tests_question1()
{
  uint8_t *test = init(15, 4);
  for (int i = 0; i < 15; i = i + 1)
  {
    assert(test[i] >= 0 && test[i] <= 4);
  }
  free(test);
}

void tests_question2()
{
  uint8_t test[] = {0, 1, 2, 0, 0, 0, 3, 0, 4, 3, 2};
  uint8_t *test2 = next(test, 11, 3);
  assert(test2[0] == 1);
  assert(test2[1] == 0);
  assert(test2[2] == 2);
  assert(test2[3] == 0);
  assert(test2[4] == 0);
  assert(test2[5] == 3);
  assert(test2[6] == 0);
  assert(test2[7] == 4);
  assert(test2[8] == 0);
  assert(test2[9] == 3);
  assert(test2[10] == 2);
  free(test2);
}

void tests_question3()
{
  affichage();
}

int main(void)
{
  tests_question1();
  tests_question2(); // a decommenter
  tests_question3(); // a decommenter
  //#run: \> /dev/null
  return 0;
}
