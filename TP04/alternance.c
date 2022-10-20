/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g alternance.c -o alternance
Date	: 21.10.2022
____________________
*/
/*
__________
Author:         Alexis Rossfelder, MP2I
Compilation:    /usr/bin/gcc -Wall -Wextra -Werror -Wpedantic -O -g -o alternance alternance.c
Built on:       2022.09.30 10:24:08
__________
*/
#include <stdio.h>

void alternance(int a, int b, int count)
{
    if (count <= 0)
        return;
    for (int i = 0; i < a; i++)
        printf("*");
    printf("\n");
    alternance(b, a, count - 1);
}

int main(void)
{
    alternance(5, 3, 8);
    return 0;
}
