/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g parfait.c -o parfait
Date	: 21.10.2022
____________________
*/
/*
__________
Author:         Alexis Rossfelder, MP2I
Compilation:    /usr/bin/gcc -Wall -Wextra -Werror -Wpedantic -O -g -o parfait parfait.c
Built on:       2022.09.30 11:16:57
__________
*/
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

bool parfait(unsigned int number) {
  unsigned int div_sum=0;

  for (unsigned int div=1; div < number; div++){
    if (number%div == 0) {
      div_sum += div;
    }
  }
  
  return number == div_sum;
}

int main(void){
  assert(parfait(6));
  assert(!parfait(15));
  assert(parfait(8128));
  assert(parfait(33550336));
  assert(!parfait(64726437));
  
  return 0;
}
