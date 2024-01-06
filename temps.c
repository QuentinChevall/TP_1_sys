#include<stdio.h>
#include<time.h>

void printTime()
{
    clock_t t;
    t = clock();
    printf("Time since execution: %f\n", (float)t/CLOCKS_PER_SEC);
}

int main()
{
    int a,b;
    printf("Enter two numbers: "); 
    scanf("%d %d", &a, &b);
    printf("The sum is: %d\n", a+b);
    printTime();
    return 0;
}