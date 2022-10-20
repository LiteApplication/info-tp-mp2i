/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g -Wno-unused-variable maximum_local.c -o maximum_local
Date	: 21.10.2022
____________________
*/
#include <assert.h>
#include <stdbool.h>

bool maximum_local(int *tab, int size, int index)
{
  //#args: -Wno-unused-variable ^-0
  if (index == 0 || index == size)
  {
    return false;
  }

  if (tab[index - 1] < tab[index] && tab[index] > tab[index + 1])
  {
    return true;
  }
  return false;
}

bool vrai_maximum_local(int *tab, int size, int index)
{
  if (index == 0 || index == size)
  {
    return false;
  }
  int value = tab[index];
  int new_index = index;

  // Check the right side
  while (tab[new_index] == value)
  {

    new_index += 1;
    if (new_index == size)
      return false; // Reached the end
  }
  if (tab[new_index] > value)
    return false; // Found a larger value

  new_index = index;

  // Check the left side
  while (tab[new_index] == value)
  {
    new_index -= 1;
    if (new_index == 0)
      return false; // Reached the end
  }
  if (tab[new_index] > value)
    return false; // Found a bigger value

  return true;
}

int main(void)
{
  int t[] = {1, 2, 3, 2, 1, -1, -2, -1, -3, 5, 5, 2};
  int t2[] = {1, 2, 3, 2, 1, -1, -2, -1, -3, 5, 5, 2, 3, 3, 3, 3, 1, 4, 4, 4};

  assert(!maximum_local(t, 12, 0));
  assert(maximum_local(t, 12, 2));
  assert(maximum_local(t, 12, 7));
  assert(!maximum_local(t, 12, 9));
  assert(!maximum_local(t, 12, 12));

  //à décommenter pour la question 2
  assert(! vrai_maximum_local(t2, 20, 0));
  assert(vrai_maximum_local(t2, 20, 2));
  assert(vrai_maximum_local(t2, 20, 7));
  assert(vrai_maximum_local(t2, 20, 9));
  assert(vrai_maximum_local(t2, 20, 12));
  assert(vrai_maximum_local(t2, 20, 13));
  assert(vrai_maximum_local(t2, 20, 14));
  assert(vrai_maximum_local(t2, 20, 15));
  assert(! vrai_maximum_local(t2, 20, 16));
  assert(! vrai_maximum_local(t2, 20, 18));
  assert(! vrai_maximum_local(t2, 20, 19));
  

  return 0;
}
