/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g plagiat.c -o plagiat
Date	: 23.10.2022
____________________
*/
#include <assert.h>
#include <stdlib.h>

int *copie(int *t, int n)
{
    int *t2 = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        t2[i] = t[i];
    }
    return t2;
}

int main(void)
{
    int a[] = {9};
    int *a2 = copie(a, 1);
    int b[] = {6, 10, -3, 4, 5};
    int *b2 = copie(b, 5);

    assert(a != a2);
    assert(a[0] == a2[0]);

    assert(b != b2);
    assert(b[0] == b2[0]);
    assert(b[4] == b2[4]);

    return 0;
}
