#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N 5 // Number of philosophers

pthread_mutex_t forks[N];

void *philosopher(void *arg) {
    int id = *((int *)arg);

    while (1) {
        printf("Philosopher %d is thinking.\n", id);
        sleep(1);

        // Pick up forks (left first, then right to avoid deadlock)
        pthread_mutex_lock(&forks[id]);
        pthread_mutex_lock(&forks[(id + 1) % N]);

        // Eating
        printf("Philosopher %d is eating.\n", id);
        sleep(2);

        // Put down forks
        pthread_mutex_unlock(&forks[(id + 1) % N]);
        pthread_mutex_unlock(&forks[id]);
    }
}

int main() {
    pthread_t philosophers[N];
    int ids[N];

    // Initialize mutexes for forks
    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for threads (runs indefinitely)
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutexes
    for (int i = 0; i < N; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}

