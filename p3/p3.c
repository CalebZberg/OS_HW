#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
typedef struct {int number; int count; int factors[10];} thread_data_t;
void* factorize(void* arg) {
	thread_data_t* data = (thread_data_t*)arg;
	int n = data->number;
	int factor = 2;
	data->count = 0;
	while (factor * factor <= n) {
		while (n % factor == 0) {
			if (data->count <10) data->factors[data->count++] = factor;
			n /= factor;
		}
		factor++;
	}
	if (n > 1 && data->count < 10) data->factors[data->count++] = n;
	return data;
}
int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Usage:./p3 <number to factor>...\n");
		return 1;
	}
	int num_threads = argc -1;
	pthread_t threads[num_threads];
	thread_data_t* thread_data[num_threads];
	for (int i = 0; i < num_threads; i++) {
		thread_data[i] = malloc(sizeof(thread_data_t));
		thread_data[i]->number = atoi(argv[i+1]);
	}
	for (int i = 0; i < num_threads; i++) {
		pthread_create(&threads[i], NULL, factorize, thread_data[i]);
	}
	for (int i = 0; i < num_threads; i++) {
		void* ret;
		pthread_join(threads[i], &ret);
	}
	for (int i = 0; i < num_threads; i++) {
		printf("%d:", thread_data[i]->number);
		for (int j = 0; j < thread_data[i]->count; j++) {
			printf(" %d", thread_data[i]->factors[j]);
		}
		printf("\n");
		free(thread_data[i]);
	}
	return 0;
}
