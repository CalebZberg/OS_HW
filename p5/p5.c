#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER 10

// Buffer from main to producer
int input_buffer[MAX_BUFFER];
int input_count = 0;
int input_in = 0, input_out = 0;

// Buffer from producer to consumer
int* output_buffer[MAX_BUFFER];
int output_count = 0;
int output_in = 0, output_out = 0;

// Mutexes and condition variables
pthread_mutex_t input_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t input_not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t input_not_full = PTHREAD_COND_INITIALIZER;

pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t output_not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t output_not_full = PTHREAD_COND_INITIALIZER;

int done = 0;

int* factor_number(int num) {
    int* factors = malloc(64 * sizeof(int));
    int original = num;
    int count = 1;
    factors[0] = original;

    while (num % 2 == 0) {
        factors[count++] = 2;
        num /= 2;
    }

    for (int i = 3; i * i <= num; i += 2) {
        while (num % i == 0) {
            factors[count++] = i;
            num /= i;
        }
    }

    if (num > 1) {
        factors[count++] = num;
    }

    if (count == 1 && original > 1) {
        factors[count++] = original;
    }

    factors[count] = -1;
    return factors;
}

void* producer_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&input_mutex);
        while (input_count == 0 && !done) {
            pthread_cond_wait(&input_not_empty, &input_mutex);
        }
        if (input_count == 0 && done) {
            pthread_mutex_unlock(&input_mutex);
            break;
        }
        int number = input_buffer[input_out];
        input_out = (input_out + 1) % MAX_BUFFER;
        input_count--;
        pthread_cond_signal(&input_not_full);
        pthread_mutex_unlock(&input_mutex);

        int* factors = factor_number(number);

        pthread_mutex_lock(&output_mutex);
        while (output_count == MAX_BUFFER) {
            pthread_cond_wait(&output_not_full, &output_mutex);
        }
        output_buffer[output_in] = factors;
        output_in = (output_in + 1) % MAX_BUFFER;
        output_count++;
        pthread_cond_signal(&output_not_empty);
        pthread_mutex_unlock(&output_mutex);
    }
    return NULL;
}

void* consumer_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&output_mutex);
        while (output_count == 0 && !done) {
            pthread_cond_wait(&output_not_empty, &output_mutex);
        }
        if (output_count == 0 && done) {
            pthread_mutex_unlock(&output_mutex);
            break;
        }
        int* factors = output_buffer[output_out];
        output_out = (output_out + 1) % MAX_BUFFER;
        output_count--;
        pthread_cond_signal(&output_not_full);
        pthread_mutex_unlock(&output_mutex);

        int original = factors[0];
        printf("%d:", original);
        for (int i = 1; factors[i] != -1; i++) {
            printf(" %d", factors[i]);
        }
        printf("\n");

        free(factors);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t producer, consumer;

    pthread_create(&producer, NULL, producer_thread, NULL);
    pthread_create(&consumer, NULL, consumer_thread, NULL);

    for (int i = 1; i < argc; ++i) {
        int num = atoi(argv[i]);
        pthread_mutex_lock(&input_mutex);
        while (input_count == MAX_BUFFER) {
            pthread_cond_wait(&input_not_full, &input_mutex);
        }
        input_buffer[input_in] = num;
        input_in = (input_in + 1) % MAX_BUFFER;
        input_count++;
        pthread_cond_signal(&input_not_empty);
        pthread_mutex_unlock(&input_mutex);
    }

    pthread_mutex_lock(&input_mutex);
    done = 1;
    pthread_cond_broadcast(&input_not_empty);
    pthread_mutex_unlock(&input_mutex);

    pthread_mutex_lock(&output_mutex);
    pthread_cond_broadcast(&output_not_empty);
    pthread_mutex_unlock(&output_mutex);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    return 0;
}
