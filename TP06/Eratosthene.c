/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g Eratosthene.c -o Eratosthene
Date	: 23.10.2022
____________________
*/
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

bool *eratosthene(int n)
{
    if (n < 2)
        return NULL;

    bool *result = (bool *)malloc(n * sizeof(bool));

    // Initialize the table
    for (int i = 0; i < n; i++)
    {
        result[i] = true;
    }

    result[0] = false;
    result[1] = false; // 0 and 1 are not prime

    for (int i = 2; i < n; i++)
    {
        if (result[i])
        {
            for (int j = i * 2; j < n; j += i)
            { // Mark all multiples of i
                result[j] = false;
            }
        }
    }

    printf("The prime numbers are :");
    for (int i = 0; i < n; i++)
    {
        if (result[i])
        {
            printf(" %d", i);
        }
    }

    return result;
}

int main(void)
{
    bool *premier = eratosthene(200);

    assert(eratosthene(-1) == NULL);
    assert(!premier[0]);
    assert(!premier[1]);
    assert(premier[2]);
    assert(premier[3]);
    assert(premier[137]);
    assert(!premier[169]);

    free(premier);

    return 0;
}
