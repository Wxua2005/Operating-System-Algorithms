#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_READERS 5

sem_t resource_access;    // Semaphore to control access to the resource
sem_t read_count_access;  // Semaphore to control access to read_count
int read_count = 0;       // Number of active readers

void *reader(void *arg) {
    int reader_id = *((int *)arg);
    while (1) {
        
        sem_wait(&read_count_access);
        read_count++;
        if (read_count == 1) {
            
            sem_wait(&resource_access);
        }
        sem_post(&read_count_access);

        
        printf("Reader %d is reading the resource\n", reader_id);
        sleep(2); 

        
        sem_wait(&read_count_access);
        read_count--;
        if (read_count == 0) {
            
            sem_post(&resource_access);
        }
        sem_post(&read_count_access);

        sleep(2); 
    }
}

void *writer(void *arg) {
    int writer_id = *((int *)arg);
    while (1) {
        
        sem_wait(&resource_access);

        printf("Writer %d is writing to the resource\n", writer_id);
        sleep(2);   

        
        sem_post(&resource_access);

        sleep(2); 
    }
}

int main() {
    pthread_t readers[MAX_READERS], writer_thread;
    int reader_ids[MAX_READERS], writer_id = 1;

    sem_init(&resource_access, 0, 1);
    sem_init(&read_count_access, 0, 1);

    pthread_create(&writer_thread, NULL, writer, &writer_id);

    for (int i = 0; i < MAX_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    pthread_join(writer_thread, NULL);
    for (int i = 0; i < MAX_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    sem_destroy(&resource_access);
    sem_destroy(&read_count_access);

    return 0;
}

