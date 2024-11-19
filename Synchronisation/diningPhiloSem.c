#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (num_of_philosopher + 4) % N
#define RIGHT (num_of_philosopher + 1) % N

int state[N];
int phil[N] = {0, 1, 2, 3, 4};

// Semaphores
sem_t mutex;
sem_t S[N];

void test(int num_of_philosopher) {
    if (state[num_of_philosopher] == HUNGRY &&
        state[LEFT] != EATING &&
        state[RIGHT] != EATING) {
        // State that philosopher is eating
        state[num_of_philosopher] = EATING;
        sleep(2);
        printf("Philosopher %d takes fork %d and %d\n", num_of_philosopher + 1, LEFT + 1, num_of_philosopher + 1);
        printf("Philosopher %d is Eating\n", num_of_philosopher + 1);
        sem_post(&S[num_of_philosopher]);
    }
}

void take_fork(int num_of_philosopher) {
    sem_wait(&mutex); // Enter critical section

    // State that philosopher is hungry
    state[num_of_philosopher] = HUNGRY;
    printf("Philosopher %d is Hungry\n", num_of_philosopher + 1);

    // Try to eat if neighbors are not eating
    test(num_of_philosopher);

    sem_post(&mutex); // Exit critical section
    sem_wait(&S[num_of_philosopher]); // Wait if unable to eat
    sleep(1);
}

void put_fork(int num_of_philosopher) {
    sem_wait(&mutex); // Enter critical section

    // State that philosopher is thinking
    state[num_of_philosopher] = THINKING;
    printf("Philosopher %d putting fork %d and %d down\n", num_of_philosopher + 1, LEFT + 1, num_of_philosopher + 1);
    printf("Philosopher %d is Thinking\n", num_of_philosopher + 1);

    // Test neighbors if they can eat
    test(LEFT);
    test(RIGHT);

    sem_post(&mutex); // Exit critical section
}

void* philosopher(void* num) {
    while (1) {
        int* i = num;
        sleep(1);         // Thinking
        take_fork(*i);    // Try to acquire forks
        sleep(1);         // Eating
        put_fork(*i);     // Release forks
    }
}	

int main() {
    int i;
    pthread_t thread_id[N];

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    for (i = 0; i < N; i++) {
        sem_init(&S[i], 0, 0);
    }

    // Create philosopher threads
    for (i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
        printf("Philosopher %d is Thinking\n", i + 1);
    }

    // Wait for threads to finish
    for (i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}

