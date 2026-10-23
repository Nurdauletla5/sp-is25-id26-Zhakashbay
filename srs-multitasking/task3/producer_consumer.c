#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define PRODUCERS 3
#define CONSUMERS 2
#define ITEMS_PER_PRODUCER 20

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

pthread_mutex_t mutex;
pthread_cond_t not_full;
pthread_cond_t not_empty;

void *producer(void *arg)
{
    int id = *(int *)arg;

    for (int i = 1; i <= ITEMS_PER_PRODUCER; i++)
    {
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE)
            pthread_cond_wait(&not_full, &mutex);

        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        printf("Producer %d produced %d\n", id, i);

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);

        usleep(100000);
    }

    return NULL;
}

void *consumer(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < (PRODUCERS * ITEMS_PER_PRODUCER) / CONSUMERS; i++)
    {
        pthread_mutex_lock(&mutex);

        while (count == 0)
            pthread_cond_wait(&not_empty, &mutex);

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        printf("Consumer %d consumed %d\n", id, item);

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);

        usleep(150000);
    }

    return NULL;
}

int main()
{
    pthread_t producers[PRODUCERS];
    pthread_t consumers[CONSUMERS];

    int prod_ids[PRODUCERS];
    int cons_ids[CONSUMERS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    for (int i = 0; i < PRODUCERS; i++)
    {
        prod_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &prod_ids[i]);
    }

    for (int i = 0; i < CONSUMERS; i++)
    {
        cons_ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &cons_ids[i]);
    }

    for (int i = 0; i < PRODUCERS; i++)
        pthread_join(producers[i], NULL);

    for (int i = 0; i < CONSUMERS; i++)
        pthread_join(consumers[i], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}
