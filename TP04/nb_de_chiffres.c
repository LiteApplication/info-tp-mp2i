/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g nb_de_chiffres.c -o nb_de_chiffres
Date	: 21.10.2022
____________________
*/
/*
__________
Author:         Alexis Rossfelder, MP2I
Compilation:    /usr/bin/gcc -Wall -Wextra -Werror -Wpedantic -O -g -o nb_de_chiffres nb_de_chiffres.c
Built on:       2022.09.30 11:18:51
__________
*/
#include <assert.h>

int nb_de_chiffres(int number) {
    int count =0; 
    while (number != 0) {
        number = number/10;
        count++;
    }
    return count;
}

int main(void){
  assert(nb_de_chiffres(0) == 0);
  assert(nb_de_chiffres(1) == 1);
  assert(nb_de_chiffres(9) == 1);
  assert(nb_de_chiffres(10) == 2);
  assert(nb_de_chiffres(99) == 2);
  assert(nb_de_chiffres(100) == 3);
  assert(nb_de_chiffres(865462) == 6);
  
  return 0;
}