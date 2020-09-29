#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "queue.h"

void *run(void *q) {
	EnqueueString(q, "a");
	return NULL;
}

int main() {
	struct Queue *q = CreateStringQueue(10);
	pthread_t threads[10];
	for (int i = 0; i < 10; i++) {
		pthread_create(&(threads[i]), NULL, run, q);
	}
	for (int i = 0; i < 10; i++) {
		pthread_join(threads[i], NULL);
	}
	PrintQueueStats(q);
}
