/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g valeur_absolue.c -o valeur_absolue
Date	: 21.10.2022
____________________
*/
/*
__________
Author:         Alexis Rossfelder, MP2I
Compilation:    /usr/bin/gcc -Wall -Wextra -Werror -Wpedantic -O -g -o build/valeur_absolue valeur_absolue.c
Built on:       2022.09.25 11:55:15
__________
*/
/*
    Alexis Rossfelder
    gcc -Wall -Wextra -Werror -lm valeur_absolue.c
*/ 


#include <math.h>
#include <assert.h>

double valeur_absolue(double a);

int main(void){
  double epsilon = 0.0000001;

  assert(fabs(valeur_absolue(0)) < epsilon);
  assert(fabs(valeur_absolue(1.) - 1) < epsilon);
  assert(fabs(valeur_absolue(-1.7) - 1.7) < epsilon);

  return 0;
}

double valeur_absolue(double a) {
    if (a < 0) {
        return -a;
    }
    return a;
}