//Define a global common period of the tasks (each task has same
//period), using timer function, implement release of each task at the period

#include <stdio.h>
#include <time.h>
#include <unistd.h>

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
    int period = 3;
    int i;
    for(i=0;i<3;i++)
    {
        task1();
        task2();
        task3();
        sleep(period);
    }
    return 0;
}

