/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g pascal.c -o pascal
Date	: 21.10.2022
____________________
*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint8_t *ligne_suivante(uint8_t *ligne, int size)
{
  uint8_t *result = (uint8_t*)malloc((size + 1) * sizeof(uint8_t));
  result[0] = 1; //
  result[size] = 1;
  for (int i = 1; i < size; i++ ) {
    result[i] = ligne[i-1] + ligne[i];
  }

  return result;
}

void triangle_de_Pascal(int n) {
  // On crée un tableau pour contenir la ligne courante
  uint8_t* tab = (uint8_t*) malloc(n * sizeof(uint8_t));
  // Deuxième tableau pour contenir la ligne suivante
  uint8_t* tab2;

  for (int i = 0; i < n; i++)
  { 
    // On crée la ligne suivante (il y a 1 malloc)
    tab2 = ligne_suivante(tab, i);

    for (int j = 0; j < i + 1; j++){
      // Affichage de chaque case 
      printf("%d ", tab2[j]); 
    }
    free(tab); // On nettoie
    tab = tab2; // Et on swap!

    printf("\n"); // Fin de la ligne, on recommence
  }
  free(tab);
}

void tests_question1()
{
  uint8_t p0[] = {1};
  uint8_t *p1, *p2, *p3, *p4, *p5;

  p1 = ligne_suivante(p0, 1);
  assert(p1[0] == 1);
  assert(p1[1] == 1);

  p2 = ligne_suivante(p1, 2);
  free(p1);
  assert(p2[0] == 1);
  assert(p2[1] == 2);
  assert(p2[2] == 1);

  p3 = ligne_suivante(p2, 3);
  free(p2);
  assert(p3[0] == 1);
  assert(p3[1] == 3);
  assert(p3[2] == 3);
  assert(p3[3] == 1);

  p4 = ligne_suivante(p3, 4);
  free(p3);
  p5 = ligne_suivante(p4, 5);
  free(p4);
  assert(p5[1] == 5);
  assert(p5[3] == 10);
  free(p5);
}

void tests_question2()
{
  printf("triangle de Pascal de hauteur 1:\n");
  triangle_de_Pascal(1);
  printf("\n\ntriangle de Pascal de hauteur 10:\n");
  triangle_de_Pascal(10);
}

int main(void)
{
  tests_question1();
  tests_question2(); // a decommenter

  return 0;
}
