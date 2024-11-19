#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];  // Shared buffer
int in = 0;               // Index for producer
int out = 0;              // Index for consumer

sem_t empty;              // Semaphore to track empty slots
sem_t full;               // Semaphore to track filled slots
pthread_mutex_t mutex;    // Mutex for critical section

void *producer(void *arg) {
    int item;
    while (1) {
        // Produce an item (e.g., a random number)
        item = rand() % 100;

        // Wait for an empty slot
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        // Critical section: Add the item to the buffer
        buffer[in] = item;
        printf("Producer produced: %d at %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);  // Signal a filled slot

        sleep(1);  // Simulate production time
    }
    return NULL;
}

void *consumer(void *arg) {
    int item;
    while (1) {
        // Wait for a filled slot
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        // Critical section: Remove the item from the buffer
        item = buffer[out];
        printf("Consumer consumed: %d from %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);  // Signal an empty slot

        sleep(2);  // Simulate consumption time
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    // Initialize the semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);  // Initial empty slots = BUFFER_SIZE
    sem_init(&full, 0, 0);             // Initial filled slots = 0
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for threads to finish (this will never happen in this example)
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Clean up
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}

