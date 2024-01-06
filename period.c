//execute tasks at the period (same time) using timer function
//mplement release of each task at the period



#include <stdio.h>
#include <time.h>

void printTime()
{
    clock_t t;
    t = clock();
    printf("Time since execution: %f\n", (float)t/CLOCKS_PER_SEC);
}

void task1()
{
    printf("Task 1\n");
    printTime();
}

void task2()
{
    printf("Task 2\n");
    printTime();
}

void task3()
{
    printf("Task 3\n");
    printTime();
}

int main()
{
    clock_t t;
    t = clock();
    clock_t t1;
    int end = 0;
    while (end == 0)
    {
        t1 = clock();
        if ((float)(t1-t)/CLOCKS_PER_SEC > 3)
        {
            task1();
            task2();
            task3();
            end = 1;
        }  
    }    
    return 0;
}
