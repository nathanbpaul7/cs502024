#include <cs50.h>
#include <stdio.h>
#include <string.h>
int pos;

int collatz(int n);


int main(void)
{
    while (pos < 1)
    {
        pos = get_int("Positive Integer: ");
    }
    printf("%i\n", collatz(pos));
}


int collatz(int n)
{
    if (n == 1)
        return 0;
    if (n % 2 == 0)
    {
        return 1 + collatz(n / 2);
    }
    else
    {
        return 1 + collatz(3*n + 1);
    }
}
