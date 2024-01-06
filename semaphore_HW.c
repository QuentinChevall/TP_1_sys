#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;

void* thread_func(void* arg) {
    sem_wait(&sem); // entering critical section
    printf("Hello World\n");
    sem_post(&sem); // leaving critical section
    return NULL;
}

int main() {
    pthread_t t1;

    // initialize semaphore
    sem_init(&sem, 0, 1);

    // create threads
    pthread_create(&t1, NULL, thread_func, NULL);

    // wait for threads to finish
    pthread_join(t1, NULL);

    // destroy semaphore
    sem_destroy(&sem);

    return 0;
}