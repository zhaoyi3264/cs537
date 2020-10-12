/*
 * Main driver module
 * 
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */
 
#ifndef STATISTICS_H
#define STATISTICS_H

#include <sys/time.h>
#include <semaphore.h>

typedef struct Statistics {
	int enqueueCount;
	int dequeueCount;
	suseconds_t enqueueTime;
	suseconds_t dequeueTime;
	sem_t *mutex;
} Statistics;

Statistics *create_stat();

void end_timer(Statistics *stat, struct timeval *start, int is_enqueue);

void print_stat(Statistics *stat);

#endif
