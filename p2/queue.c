#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "queue.h"

void printQueue(Queue *q) {
	printf("queue: ");
	for(int i = 0; i < q->size; i++) {
		if ((q->data)[i]) {
			printf("%.5s - ", (q->data)[i]);
		} else {
			printf("%s - ", "empty");
		}
	}
	printf("\n");
}

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
Queue *CreateStringQueue(int size) {
	Queue *q = malloc(sizeof(Queue));
	q->data = malloc(size * sizeof(char *));
	q->size = size;
	q->enqueueCount = 0;
	q->dequeueCount = 0;
	q->enqueueTime = 0;
	q->dequeueTime = 0;
	
	q->mutex = malloc(sizeof(sem_t));
	q->empty = malloc(sizeof(sem_t));
	q->full = malloc(sizeof(sem_t));
	sem_init(q->mutex, 0, 1);
	sem_init(q->empty, 0, size);
	sem_init(q->full, 0, 0);
	return q;
}

/*
 * This function places the pointer to the string at the end of queue q. If the
 * queue is full, then this function blocks until there is space available.
 */
void EnqueueString(Queue *q, char *string) {
	// start timing
	struct timeval *start = malloc(sizeof(struct timeval));
	gettimeofday(start, NULL);
	// wait for any empty
	sem_wait(q->empty);
	// acquire the lock
	sem_wait(q->mutex);
	(q->data)[(q->enqueueCount) % (q->size)] = string;
	if (string) {
		q->enqueueCount += 1;
	}
	// debug
	//~ printf("enqueued: %s", string);
	//~ printQueue(q);
	sem_post(q->mutex);
	// signal for full
	sem_post(q->full);
	// end timing
	struct timeval *end = malloc(sizeof(struct timeval));
	gettimeofday(end, NULL);
	if (string) {
		struct timeval *res = malloc(sizeof(struct timeval));
		timersub(end, start, res);
		q->enqueueTime += res->tv_sec * 10e6 + res->tv_usec;
	}
	free(end);
	free(start);
}

/*
 * This function removes a pointer to a string from the beginning of queue q.
 * If the queue is empty, then this function blocks until there is a string
 * placed into the queue. This function returns the pointer that was removed
 * from the queue.
 */
char * DequeueString(Queue *q) {
	//start timing
	struct timeval *start = malloc(sizeof(struct timeval));
	gettimeofday(start, NULL);
	// wait for any full
	sem_wait(q->full);
	// acquire the lock
	sem_wait(q->mutex);
	char *string = (q->data)[(q->dequeueCount) % (q->size)];
	(q->data)[(q->dequeueCount) % (q->size)] = NULL;
	if (string) {
		q->dequeueCount += 1;
	}
	// debug
	//~ printf("dequeued: %s", string);
	//~ printQueue(q);
	sem_post(q->mutex);
	// signal for empty
	sem_post(q->empty);
	// end timing
	struct timeval *end = malloc(sizeof(struct timeval));
	gettimeofday(end, NULL);
	if (string) {
		struct timeval *res = malloc(sizeof(struct timeval));
		timersub(end, start, res);
		q->dequeueTime += res->tv_sec * 10e6 + res->tv_usec;
	}
	free(end);
	free(start);
	return string;
}

/*
 * This function prints the statistics for this queue (see the next section for
 * details).
 */
void PrintQueueStats(Queue *q) {
	fprintf(stderr, "enqueueCount:\t%.2d\n", q->enqueueCount);
	fprintf(stderr, "dequeueCount:\t%.2d\n", q->dequeueCount);
	fprintf(stderr, "enqueueTime:\t%f\n", q->enqueueTime / 10e6);
	fprintf(stderr, "dequeueTime:\t%f\n", q->dequeueTime / 10e6);
}
