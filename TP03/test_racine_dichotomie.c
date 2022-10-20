/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g test_racine_dichotomie.c -o test_racine_dichotomie
Date	: 21.10.2022
____________________
*/
/*
__________
Author:         Alexis Rossfelder, MP2I
Compilation:    /usr/bin/gcc -Wall -Wextra -Werror -Wpedantic -O -g -o build/test_racine_dichotomie test_racine_dichotomie.c
Built on:       2022.09.25 11:55:15
__________
*/
#include <math.h>
#include <assert.h>

double racine_par_dichotomie(double, double);
double vabs(double a);

int main(void)
{
    double precisiontest = 0.0000001;

    for (int i=43561; i < 143560; i++){
        racine_par_dichotomie(i, precisiontest);
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

double racine_par_dichotomie(double number, double precision)
{
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