/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g test_racine_heron.c -o test_racine_heron
Date	: 21.10.2022
____________________
*/
/*
__________
Author:         Alexis Rossfelder, MP2I
Compilation:    /usr/bin/gcc -Wall -Wextra -Werror -Wpedantic -O -g -o build/test_racine_heron test_racine_heron.c
Built on:       2022.09.25 11:55:15
__________
*/
#include <math.h>
#include <assert.h>

double racine_Heron(double, double);
double vabs(double a);

int main(void)
{
    double precisiontest = 0.0000001;

    for (int i=43561; i < 143560; i++){
        racine_Heron(i, precisiontest);
    }

    return 0;
}

/* Valeur absolue de a */
double vabs(double a)
{
    if (a < 0)
    {
        return -a;
    }
    return a;
}

double racine_Heron(double number, double precision)
{
    double cote_a = number, cote_b = 1;
    while (vabs(cote_a - cote_b) > precision)
    {
        cote_a = (cote_a + cote_b) / 2;
        cote_b = (2 * number) / (cote_a + cote_b);
    }
    return cote_a;
}
