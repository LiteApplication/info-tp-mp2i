/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g crible_d_eratosthene.c -o crible_d_eratosthene
Date	: 21.10.2022
____________________
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

int *crible(int n)
{
    bool *tab = malloc(n * sizeof(bool));

    // Initialize the table
    for (int i = 0; i < n; i++)
    {
        tab[i] = false;
    }

    tab[0] = true;
    tab[1] = true; // 0 and 1 are not prime

    for (int i = 1; i < n; i++)
    {
        if (!tab[i])
        {
            for (int j = i * 2; j < n; j += i)
            { // Mark all multiples of i
                tab[j] = true;
            }
        }
    }
    int count = 0;
    for (int i = 0; i < n; i++)
        if (!tab[i])
            count++;
    // We will use that count to allocate an array of the perfect size

    count++; // Total amount of cells to allocate
    // We use one more because the first cell is the total amount of primes found

    int *result = malloc((count) * sizeof(int));

    result[0] = count - 1; // One less than the total amount of cells
    for (int i = 0; i < n; i++)
    {
        if (!tab[i])
        {
            count--; // We decrement the count to know where to put the next prime
            // Everything is stored in reverse order
            result[count] = i;
        }
    }

    free(tab); // We don't need the table anymore

    return result;
}

int assert_identiques(int *r, int *rep, int size)
{
    for (int i = 0; i < size; i = i + 1)
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
    int *primes = crible(100);
    int count = primes[0];

    int check[] = {25, 97, 89, 83, 79, 73, 71, 67, 61, 59, 53, 47, 43, 41, 37, 31, 29, 23, 19, 17, 13, 11, 7, 5, 3, 2};

    for (int i = 0; i < count + 1; i++)
    {
        //printf("%d\n", primes[i]);
    }

    assert(assert_identiques(primes, check, count + 1));
    free(primes);
    return 0;
}
