#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "queue.h"

Queue *q;

void *enqueue_test(void *arg) {
	sleep(1);
	EnqueueString(q, arg);
	return NULL;
}

void *dequeue_test(void *arg) {
	sleep(2);
	DequeueString(q);
	return arg;
}

void stress_test(int queue_size, int thread_num) {
	q = CreateStringQueue(queue_size);
	pthread_t threads[thread_num];
	for (int i = 0; i < thread_num; i++) {
		if (i % 2) {
			char *buffer  = malloc(8);
			sprintf(buffer, "%d", i);
			pthread_create(&(threads[i]), NULL, enqueue_test, buffer);
		} else {
			pthread_create(&(threads[i]), NULL, dequeue_test, NULL);
		}
	}
	for (int i = 0; i < thread_num; i++) {
		pthread_join(threads[i], NULL);
	}
	PrintQueueStats(q);
}

int main() {
	stress_test(5, 50);
}
