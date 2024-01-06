//Use sleep(duration) from unistd.h to print two lines, one 10 seconds after another

#include <stdio.h>
#include <unistd.h>
#include <time.h>

void printTime()
{
    clock_t t;
    t = clock();
    printf("Time since execution: %f\n", (float)t/CLOCKS_PER_SEC);
}
int main()
{


    printf("Debut\n");
    sleep(10);
    printf("Fin\n");
    printTime();

    return 0;
}

