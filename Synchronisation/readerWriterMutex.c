#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_READERS 5

pthread_mutex_t resource_access = PTHREAD_MUTEX_INITIALIZER;  // Controls resource access
pthread_mutex_t read_count_access = PTHREAD_MUTEX_INITIALIZER; // Controls read_count access
int read_count = 0; // Number of active readers

void *reader(void *arg) {
    int reader_id = *((int *)arg);
    while (1) {
        // Reader requests access
        pthread_mutex_lock(&read_count_access);
        read_count++;
        if (read_count == 1) {
            // First reader locks the resource
            pthread_mutex_lock(&resource_access);
        }
        pthread_mutex_unlock(&read_count_access);

        // Reading resource
        printf("Reader %d is reading the resource\n", reader_id);
        sleep(1);

        // Reader releases access
        pthread_mutex_lock(&read_count_access);
        read_count--;
        if (read_count == 0) {
            // Last reader unlocks the resource
            pthread_mutex_unlock(&resource_access);
        }
        pthread_mutex_unlock(&read_count_access);

        sleep(1);
    }
}

void *writer(void *arg) {
    int writer_id = *((int *)arg);
    while (1) {
        // Writer requests access
        pthread_mutex_lock(&resource_access);

        // Writing to resource
        printf("Writer %d is writing to the resource\n", writer_id);
        sleep(2);

        // Writer releases access
        pthread_mutex_unlock(&resource_access);

        sleep(2);
    }
}

int main() {
    pthread_t readers[MAX_READERS], writer_thread;
    int reader_ids[MAX_READERS], writer_id = 1;

    // Create writer thread
    pthread_create(&writer_thread, NULL, writer, &writer_id);

    // Create reader threads
    for (int i = 0; i < MAX_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Wait for threads (runs indefinitely)
    pthread_join(writer_thread, NULL);
    for (int i = 0; i < MAX_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    return 0;
}

