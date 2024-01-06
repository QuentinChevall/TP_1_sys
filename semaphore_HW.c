#include <stdio.h>
#include <pthread.h>

void *printHello(void *arg) {
    printf("Hello World\n");
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, printHello, NULL);
    pthread_join(thread, NULL);
    return 0;
}
