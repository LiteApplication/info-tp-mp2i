/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g racine_heron.c -o racine_heron -lm
Date	: 21.10.2022
____________________
*/
/*
__________
Author:         Alexis Rossfelder, MP2I
Compilation:    /usr/bin/gcc -Wall -Wextra -Werror -Wpedantic -O -g -o build/racine_heron racine_heron.c -lm
Built on:       2022.09.25 11:55:15
__________
*/
#include <math.h>
#include <assert.h>

double racine_Heron(double, double);
double vabs(double a);

int main(void)
{
    double epsilon = 0.001;

    /* Il manquait les variables donc j'ai pris les mêmes que pour la dichotomie */
    double precisiontest = 0.0000001;
    double xtest = 43643285.43;

    assert(fabs(racine_Heron(0.1, precisiontest) - sqrt(.1)) < epsilon);
    assert(fabs(racine_Heron(.5, precisiontest) - sqrt(.5)) < epsilon);
    assert(fabs(racine_Heron(xtest, precisiontest) - sqrt(xtest)) < epsilon);

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
//# libs: -lm