/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g ecriture_binaire_positif.c -o ecriture_binaire_positif
Date	: 21.10.2022
____________________
*/
/*
__________
Author:         Alexis Rossfelder, MP2I
Compilation:    /usr/bin/gcc -Wall -Wextra -Werror -Wpedantic -O -g -o ecriture_binaire_positif ecriture_binaire_positif.c
Built on:       2022.09.30 11:05:18
__________
*/
#include <stdio.h>
#include <inttypes.h>

void ecriture_binaire_positif_rec(int32_t number, int bit)
{
    if (bit == 0)
    {
        printf("\n");
        return;
    }
    ecriture_binaire_positif_rec(number / 2, bit - 1);
    printf("%d", number & 1);
}
void ecriture_binaire_positif(int32_t number)
{
    ecriture_binaire_positif_rec(number, 32);
}
int main()
{
    ecriture_binaire_positif(10);

    return 0;
}
