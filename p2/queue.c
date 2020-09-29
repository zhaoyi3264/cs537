#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/*
 * Dynamically allocate a new Queue structure and initialize it with an array of
 * character points of length size. That means you'll malloc the queue structure
 * and then malloc the char ** array pointed to from that structure. Also
 * remember to any state and synchronization variables used in this structure.
 * 
 * The function returns a pointer to the new queue structure.
 * 
 * For testing purposes, create your Queue's with a size of 10.
 */
struct Queue *CreateStringQueue(int capacity) {
	struct Queue *q = malloc(sizeof(struct Queue));
	q->capacity = capacity;
	q->data = malloc(capacity * sizeof(char *));
	q->mutex = malloc(sizeof(sem_t));
	q->wait = malloc(sizeof(sem_t));
	sem_init(q->mutex, 0, 1);
	return q;
}

/*
 * This function places the pointer to the string at the end of queue q. If the
 * queue is full, then this function blocks until there is space available.
 */
void EnqueueString(struct Queue *q, char *string) {
	sem_wait(q->mutex);
	if(q->size >= q->capacity) {
		q->enqueueBlockCount += 1;
		sem_post(q->mutex);
		sem_wait(q->wait);
	} else {
		(q->data)[(q->enqueueCount) % (q->capacity)] = string;
		q->enqueueCount += 1;
		q->size += 1;
		sem_post(q->mutex);
	}
}

/*
 * This function removes a pointer to a string from the beginning of queue q.
 * If the queue is empty, then this function blocks until there is a string
 * placed into the queue. This function returns the pointer that was removed
 * from the queue.
 */
char * DequeueString(struct Queue *q) {
	sem_wait(q->mutex);
	if (q->size <= 0) {
		q->dequeueBlockCount += 1;
		sem_post(q->mutex);
		sem_wait(
	}
	sem_post(q->mutex);
	return NULL;
}

/*
 * This function prints the statistics for this queue (see the next section for details).
 */
void PrintQueueStats(struct Queue *q) {
	for (int i = 0; i < q->capacity; i++) {
		printf("%s ", (q->data)[i]);
	}
	printf("\n");
	printf("enqueueCount:\t\t%.2d\n", q->enqueueCount);
	printf("dequeueCount:\t\t%.2d\n", q->dequeueCount);
	printf("enqueueBlockCount:\t%.2d\n", q->enqueueBlockCount);
	printf("dequeueBlockCount:\t%.2d\n", q->dequeueBlockCount);
}
