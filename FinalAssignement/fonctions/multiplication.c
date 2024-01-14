#include <stdio.h>
void Multiplication(void)
{
    long int a = 123456789, b = 987654321, c;
    c = a * b;
    printf( "Multiplication of %ld and %ld is %ld\n", a, b, c );
}

int main(void)
{
    Multiplication();
    return 0;
}
