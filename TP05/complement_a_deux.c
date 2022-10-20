/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g complement_a_deux.c -o complement_a_deux
Date	: 21.10.2022
____________________
*/
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool *complement_a_deux(int n)
{
  bool *result = malloc(8 * sizeof(bool));
  ///#args: ^-Werror ^-O
  // bool result[8] = {0};
  int current = 0;
  result[0] = n < 0; // Set the first bit to 1 if n < 0
  if (n < 0)
  {
    current = -(1 << 7);
  }
  else
  {
    current = 0;
  }
  for (int i = 6; i > -1; i--)
  {
    if (current + (1 << i) <= n)
    {
      result[7 - i] = true;
      current += (1 << i);
    }
    else
    {
      result[7 - i] = false;
    }
    // printf("n : %d, current : %d, i : %d, 2^i : %d\n",n, current, i, (1 <<
    // i));
  }

  return result;
}

bool assert_identiques(bool *r, bool *rep)
{
  for (int i = 0; i < 8; i = i + 1)
  {
    if (r[i] != rep[i])
    {
      return false;
    }
  }
  return true;
}

int main(void)
{
  bool *r0 = complement_a_deux(0);
  bool rep0[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  bool *r1 = complement_a_deux(1);
  bool rep1[8] = {0, 0, 0, 0, 0, 0, 0, 1};
  bool *rm1 = complement_a_deux(-1);
  bool repm1[8] = {1, 1, 1, 1, 1, 1, 1, 1};
  bool *r4 = complement_a_deux(4);
  bool rep4[8] = {0, 0, 0, 0, 0, 1, 0, 0};
  bool *rm4 = complement_a_deux(-4);
  bool repm4[8] = {1, 1, 1, 1, 1, 1, 0, 0};
  bool *r10 = complement_a_deux(10);
  bool rep10[8] = {0, 0, 0, 0, 1, 0, 1, 0};
  bool *rm10 = complement_a_deux(-10);
  bool repm10[8] = {1, 1, 1, 1, 0, 1, 1, 0};

  assert(assert_identiques(r0, rep0));
  assert(assert_identiques(r1, rep1));
  assert(assert_identiques(rm1, repm1));
  assert(assert_identiques(r4, rep4));
  assert(assert_identiques(rm4, repm4));
  assert(assert_identiques(r10, rep10));
  assert(assert_identiques(rm10, repm10));

  free(r0);
  free(r1);
  free(rm1);
  free(r4);
  free(rm4);
  free(r10);
  free(rm10);

  return 0;
}
