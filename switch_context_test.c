#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <sched.h>

#define NUM_THREADS 16

void *worker(void *arg) {
    long id = (long)arg;
    volatile int counter = 0;

    for (int i = 0; i < 100000000; i++) {
        counter += i % 100;
    }

    printf("Thread %ld finished\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    sched_setaffinity(0, sizeof(cpuset), &cpuset);

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, (void*)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}