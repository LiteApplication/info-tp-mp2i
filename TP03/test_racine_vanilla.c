/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g test_racine_vanilla.c -o test_racine_vanilla -lm
Date	: 21.10.2022
____________________
*/
/*
__________
Author:         Alexis Rossfelder, MP2I
Compilation:    /usr/bin/gcc -Wall -Wextra -Werror -Wpedantic -O -g -o build/test_racine_vanilla test_racine_vanilla.c -lm
Built on:       2022.09.25 11:55:15
__________
*/
#include <math.h>

int main(void)
{
    for (int i = 43561; i < 143560; i++)
    {
        sqrt((double)i);
    }

    return 0;
}

//# libs: -lm