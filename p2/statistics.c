/*
 * Statistics module used to keep enqueue and dequeue operation count and time
 * 
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */
 
#include <stdlib.h>
#include <stdio.h>

#include "statistics.h"

/*
 * Create a statistics struct
 * 
 * return: pointer to the new statistics struct
 */
Statistics *create_stat() {
	Statistics *stat = malloc(sizeof(Statistics));
	if (stat == NULL) {
		exit(1);
	}
	stat->mutex = malloc(sizeof(sem_t));
	if (stat->mutex == NULL || sem_init(stat->mutex, 0, 1)) {
		fprintf(stderr, "error in semaphore initialization\n");
		exit(1);
	}
	return stat;
}

/*
 * End the timer and update enqueue/dequeue time
 * 
 * stat: statistics struct
 * start: start time of the operation
 * is_enqueue: whether to update enqueue time or dequeue time
 */
void end_timer(Statistics *stat, struct timeval *start, int is_enqueue) {
	struct timeval *end = malloc(sizeof(struct timeval));
	struct timeval *res = malloc(sizeof(struct timeval));
	if (end == NULL || res == NULL || gettimeofday(end, NULL)) {
		exit(1);
	}
	timersub(end, start, res);
	// acquire the lock
	if (sem_wait(stat->mutex)) {
		fprintf(stderr, "error in semaphore wait operation\n");
		exit(1);
	}
	if (is_enqueue) {
		stat->enqueueTime += res->tv_sec * 10e6 + res->tv_usec;
	} else {
		stat->dequeueTime += res->tv_sec * 10e6 + res->tv_usec;
	}
	free(end);
	free(res);
	// release the lock
	if (sem_post(stat->mutex)) {
		fprintf(stderr, "error in semaphore post operation\n");
		exit(1);
	}
}

/*
 * Print the statistics to stderr
 * 
 * stat: stat struct
 */
void print_stat(Statistics *stat) {
	if (sem_post(stat->mutex)) {
		fprintf(stderr, "error in semaphore wait operation");
		exit(1);
	}
	fprintf(stderr, "enqueueCount:\t%.2d\n", stat->enqueueCount);
	fprintf(stderr, "dequeueCount:\t%.2d\n", stat->dequeueCount);
	fprintf(stderr, "enqueueTime:\t%.3f\n", stat->enqueueTime / 10e6);
	fprintf(stderr, "dequeueTime:\t%.3f\n", stat->dequeueTime / 10e6);
	if (sem_post(stat->mutex)) {
		fprintf(stderr, "error in semaphore post operation\n");
		exit(1);
	}
}
