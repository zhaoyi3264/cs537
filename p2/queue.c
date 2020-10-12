/*
 * String queue module
 * 
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "queue.h"

/*
 * Create a new string queue struct and return its pointer
 * 
 * size: size of the queue
 * 
 * return: the pointer to the queue struct
 */
Queue *CreateStringQueue(int size) {
	Queue *q = malloc(sizeof(Queue));
	if (q == NULL) {
		exit(1);
	}
	q->data = malloc(size * sizeof(char *));
	if (q->data == NULL) {
		exit(1);
	}
	q->size = size;
	q->stat = create_stat();
	
	q->mutex = malloc(sizeof(sem_t));
	q->empty = malloc(sizeof(sem_t));
	q->full = malloc(sizeof(sem_t));
	if (q->mutex == NULL || q->empty == NULL || q->full == NULL) {
		exit(1);
	}
	if (sem_init(q->mutex, 0, 1) || sem_init(q->empty, 0, size) ||
		sem_init(q->full, 0, 0)) {
		fprintf(stderr, "error in semaphore initialization\n");
		exit(1);
	}
	return q;
}

/*
 * Enqueue a string into the queue and update the queue statistics. Blocks until
 * there are free space in the queue
 * 
 * q: queue struct
 * string: string to enqueue
 */
void EnqueueString(Queue *q, char *string) {
	// start timing
	struct timeval *start = malloc(sizeof(struct timeval));
	if (start == NULL || gettimeofday(start, NULL)) {
		exit(1);
	}
	// wait for any empty
	if (sem_wait(q->empty)) {
		fprintf(stderr, "error in semaphore wait operation\n");
		exit(1);
	}
	// acquire the lock
	if (sem_wait(q->mutex)) {
		fprintf(stderr, "error in semaphore wait operation\n");
		exit(1);
	}
	(q->data)[(q->stat->enqueueCount++) % (q->size)] = string;
	// release the lock and signal for full
	if (sem_post(q->mutex)) {
		fprintf(stderr, "error in semaphore post operation\n");
		exit(1);
	}
	if (sem_post(q->full)) {
		fprintf(stderr, "error in semaphore post operation\n");
		exit(1);
	}
	// end timing
	end_timer(q->stat, start, 1);
	free(start);
}

/*
 * Remove a string from the queue and update queue statistics. Blocks until there
 * are strings in the queue
 * 
 * q: queue struct
 * 
 * return: string dequeued
 */
char * DequeueString(Queue *q) {
	//start timing
	struct timeval *start = malloc(sizeof(struct timeval));
	if (start == NULL || gettimeofday(start, NULL)) {
		exit(1);
	}
	// wait for any full
	if (sem_wait(q->full)) {
		fprintf(stderr, "error in semaphore wait operation\n");
		exit(1);
	}
	// acquire the lock
	if (sem_wait(q->mutex)) {
		fprintf(stderr, "error in semaphore wait operation\n");
		exit(1);
	}
	char *string = (q->data)[(q->stat->dequeueCount) % (q->size)];
	(q->data)[(q->stat->dequeueCount++) % (q->size)] = NULL;
	// release the lock and signal for empty
	if (sem_post(q->mutex)) {
		fprintf(stderr, "error in semaphore post operation\n");
		exit(1);
	}
	if (sem_post(q->empty)) {
		fprintf(stderr, "error in semaphore post operation\n");
		exit(1);
	}
	// end timing
	end_timer(q->stat, start, 0);
	free(start);
	return string;
}

/*
 * Print the statistics of the queue
 * 
 * q: queue to print statistics
 */
void PrintQueueStats(Queue *q) {
	if (sem_wait(q->mutex)) {
		fprintf(stderr, "error in semaphore wait operation\n");
		exit(1);
	}
	print_stat(q->stat);
	if (sem_post(q->mutex)) {
		fprintf(stderr, "error in semaphore post operation\n");
		exit(1);
	}
}
