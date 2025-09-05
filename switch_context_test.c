#define _GNU_SOURCE  // Enable GNU extensions for thread affinity functions

#include <stdio.h>
#include <pthread.h>
#include <sched.h>  // For CPU affinity functions

#define NUM_THREADS 16 

// Worker function that each thread will run
void *worker(void *arg) {
    long id = (long)arg;  
    volatile int counter = 0;

    // A loop to simulate some work
    for (int i = 0; i < 100000000; i++) {
        counter += i % 100;
    }

    // Print the ID of the thread that finished its work
    printf("Thread %ld finished\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];  // Array to hold the thread IDs
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    
    // Set the CPU affinity to CPU 0 for all threads
    CPU_SET(0, &cpuset);
    
    // Set the CPU affinity of the main thread (process) to the CPU 0
    sched_setaffinity(0, sizeof(cpuset), &cpuset);

    // Create NUM_THREADS threads and assign them the worker function
    for (long i = 0; i < NUM_THREADS; i++) {
        // Create a new thread, passing the thread index as the argument
        pthread_create(&threads[i], NULL, worker, (void*)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);  // Wait for each thread to complete
    }

    return 0;
}