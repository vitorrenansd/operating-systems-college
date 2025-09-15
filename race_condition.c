#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 10  // Number of threads to create
#define ITERATIONS 1000000  // Number of iterations each thread will run

int shared_counter = 0;
atomic_int atomic_counter = 0;

void *incrementer(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        shared_counter++;  // Increment the shared counter without any synchronization
    }
    return NULL;
}

void *atomic_incrementer(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        atomic_fetch_add(&atomic_counter, 1);  // Atomically increment the counter
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    printf("Testing race condition...\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, incrementer, NULL); 
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Race condition result: %d (needed: %d)\n", shared_counter, NUM_THREADS * ITERATIONS);

    shared_counter = 0;

    printf("Testing atomic...\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, atomic_incrementer, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); 
    }

    printf("Atomic result: %d (needed: %d)\n", atomic_counter, NUM_THREADS * ITERATIONS);

    return 0;
}

/*
# Compile with thread sanitizer
gcc -fsanitize=thread -g -o 
race_condition race_condition.c -lpthread

# Use perf to analyze locking overhead
sudo perf stat -e cache-misses,cycles,instructions
*/