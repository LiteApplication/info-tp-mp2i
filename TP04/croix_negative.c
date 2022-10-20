/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g croix_negative.c -o croix_negative
Date	: 21.10.2022
____________________
*/
/*
__________
Author:         Alexis Rossfelder, MP2I
Compilation:    /usr/bin/gcc -Wall -Wextra -Werror -Wpedantic -O -g -o croix_negative croix_negative.c
Built on:       2022.09.30 12:09:57
__________
*/
#include <stdio.h>

void croix_negative(int size)
{
    if (size % 2 == 0)
        size += 1;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            printf((i == j || size - i - 1 == j) ? " " : "*");
        printf("\n");
    }
}

int main(void)
{
    croix_negative(10);
    croix_negative(9);
    croix_negative(80);
    

    return 0;
}