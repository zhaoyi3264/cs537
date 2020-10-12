/*
 * Main driver module
 * 
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <semaphore.h>

#include "statistics.h"

typedef struct Queue {
	int size;
	char **data;
	sem_t *mutex;
	sem_t *empty;
	sem_t *full;
	Statistics *stat;
} Queue;

Queue *CreateStringQueue(int size);

void EnqueueString(Queue *q, char *string);

char * DequeueString(Queue *q);

void PrintQueueStats(Queue *q);

#endif
