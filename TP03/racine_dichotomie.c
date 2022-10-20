/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g racine_dichotomie.c -o racine_dichotomie -lm
Date	: 21.10.2022
____________________
*/
/*
__________
Author:         Alexis Rossfelder, MP2I
Compilation:    /usr/bin/gcc -Wall -Wextra -Werror -Wpedantic -O -g -o build/racine_dichotomie racine_dichotomie.c -lm
Built on:       2022.09.25 11:55:15
__________
*/
#include <math.h>
#include <assert.h>

double racine_par_dichotomie(double, double);
double vabs(double a);

int main(void)
{
    double epsilon = 0.001;
    double precisiontest = 0.0000001;
    double xtest = 43643285.43;

    assert(fabs(racine_par_dichotomie(0, precisiontest)) < epsilon);
    assert(fabs(racine_par_dichotomie(xtest, precisiontest) - sqrt(xtest)) < epsilon);
    assert(fabs(racine_par_dichotomie(.5, precisiontest) - sqrt(.5)) < epsilon);

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

double racine_par_dichotomie(double number, double precision)
{
    // Left and right are the bounds of the interval
    // Middle is the middle of the interval, the value we are testing
    double left, right, middle;

    if (number < 1)
    {
        left = number;
        right = 1;
    }
    else
    {
        left = 1;
        right = number;
    }
    middle = (right + left) / 2;
    while (vabs(middle * middle - number) > precision)
    {

        if ((middle * middle) < number)
        {
            left = middle;
        }
        else
        {
            right = middle;
        }
        middle = (right + left) / 2;
    }
    return middle;
}

//# libs: -lm