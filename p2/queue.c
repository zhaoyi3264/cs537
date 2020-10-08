#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
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
Queue *CreateStringQueue(int size) {
	Queue *q = malloc(sizeof(Queue));
	q->data = malloc(size * sizeof(char *));
	q->size = size;
	q->stat = malloc(sizeof(Statistics *));
	
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
	(q->data)[(q->stat->enqueueCount++) % (q->size)] = string;
	// end timing
	struct timeval *end = malloc(sizeof(struct timeval));
	gettimeofday(end, NULL);
	update_enqueue(q->stat, start, end);
	// release the lock
	sem_post(q->mutex);
	// signal for full
	sem_post(q->full);
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
	char *string = (q->data)[(q->stat->dequeueCount) % (q->size)];
	(q->data)[(q->stat->dequeueCount++) % (q->size)] = NULL;
	// end timing
	struct timeval *end = malloc(sizeof(struct timeval));
	gettimeofday(end, NULL);
	//update stat
	update_dequeue(q->stat, start, end);
	// release the lock
	sem_post(q->mutex);
	// signal for empty
	sem_post(q->empty);
	free(end);
	free(start);
	return string;
}

/*
 * This function prints the statistics for this queue (see the next section for
 * details).
 */
void PrintQueueStats(Queue *q) {
	sem_wait(q->mutex);
	Statistics *stat = q->stat;
	fprintf(stderr, "enqueueCount:\t%.2d\n", stat->enqueueCount);
	fprintf(stderr, "dequeueCount:\t%.2d\n", stat->dequeueCount);
	fprintf(stderr, "enqueueTime:\t%.3f\n", stat->enqueueTime / 10e6);
	fprintf(stderr, "dequeueTime:\t%.3f\n", stat->dequeueTime / 10e6);
	sem_post(q->mutex);
}
