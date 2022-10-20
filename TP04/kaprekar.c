/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g kaprekar.c -o kaprekar
Date	: 21.10.2022
____________________
*/
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

int power_10(int a);
int n_digits(int number);
int cut_after(int number, int place);
int cut_before(int number, int place);
bool Kaprekar(int number);
int main(void);

int power_10(int a)
{
    int result = 1;
    for (int i = 0; i < a; i++)
    {
        result *= 10;
    }
    return result;
}

int n_digits(int number)
{
    int count = 0;
    while (number != 0)
    {
        number = number / 10;
        count++;
    }
    return count;
}

int cut_after(int number, int place)
{
    return number / power_10(place);
}

int cut_before(int number, int place)
{
    return number % power_10(place);
}

bool Kaprekar(int number)
{
    int squared = number * number;
    int len_b10 = n_digits(squared);
    for (int i = 0; i < len_b10; i++)
    {
        if (cut_after(squared, i) + cut_before(squared, i) == number)
        {
            return true;
        }
    }
    return false;
}

int main(void)
{
    assert(Kaprekar(703));
    assert(Kaprekar(4879));
    assert(!Kaprekar(4329));

    return 0;
}
