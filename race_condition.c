#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>  // For atomic operations

#define NUM_THREADS 10  // Number of threads to create
#define ITERATIONS 1000000  // Number of iterations each thread will run

// Shared counter without atomic operations, prone to race conditions
int shared_counter = 0;

// Atomic counter using atomic operations to prevent race conditions
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
    pthread_t threads[NUM_THREADS];  // Array to store thread IDs

    printf("Testing race condition...\n");

    // Create NUM_THREADS threads to increment shared_counter
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, incrementer, NULL); 
    }

    // Wait for all incrementer threads to finish execution
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Race condition result: %d (needed: %d)\n", shared_counter, NUM_THREADS * ITERATIONS);

    // Reset the shared counter to 0 before running the atomic version
    shared_counter = 0;

    // Create NUM_THREADS threads to increment atomic_counter (using atomic operations)
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, atomic_incrementer, NULL);
    }

    // Wait for all atomic_incrementer threads to finish execution
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); 
    }

    printf("Atomic result: %d (needed: %d)\n", atomic_counter, NUM_THREADS * ITERATIONS);

    return 0;
}
