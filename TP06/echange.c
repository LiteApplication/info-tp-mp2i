/*
____________________
Author	: Alexis Rossfelder, MP2I
Command	: gcc -Wall -Wextra -Werror -Wpedantic -O -g echange.c -o echange
Date	: 23.10.2022
____________________
*/
void echange(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int main()
{
    int a = 1;
    int b = 2;
    echange(&a, &b);
}
