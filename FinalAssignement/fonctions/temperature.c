#include <stdio.h>
void Temperature(void)
{
    int fahrenheit = 98;
    float celsius = (fahrenheit - 32) * 5 / 9;
    printf( "Temperature: %f\n", celsius );
}

int main(void)
{
    Temperature();
    return 0;
}
