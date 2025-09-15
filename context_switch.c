#define _GNU_SOURCE  // Enable GNU extensions for thread affinity functions

#include <stdio.h>
#include <pthread.h>
#include <sched.h>

#define NUM_THREADS 16 

void *worker(void *arg) {
    long id = (long)arg;  
    volatile int counter = 0;

    // A loop to simulate some work
    for (int i = 0; i < 100000000; i++) {
        counter += i % 100;
    }

    printf("Thread %ld finished\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];  // Array to hold the thread IDs
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    
    sched_setaffinity(0, sizeof(cpuset), &cpuset);

    // Create NUM_THREADS threads and assign them the worker function
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, (void*)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);  // Wait for each thread to complete
    }

    return 0;
}

/*
# Compile with debug symbols:
gcc -g -o context_switch context_switch.c -lpthread

# Show context switches:
sudo perf stat -e context-switches,cpu-migrations ./context_switch

# Scheduler profile:
sudo perf record -e sched:sched_switch,sched:sched_stat_wait ./context_switch
sudo perf script | head -20
*/