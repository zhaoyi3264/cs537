#include <stdlib.h>
#include <sys/time.h>
#include "statistics.h"

void update_enqueue(Statistics *stat, struct timeval *start,
	struct timeval *end) {
	struct timeval *res = malloc(sizeof(struct timeval));
	timersub(end, start, res);
	stat->enqueueTime += res->tv_sec * 10e6 + res->tv_usec;
}

void update_dequeue(Statistics *stat, struct timeval *start,
	struct timeval *end) {
	struct timeval *res = malloc(sizeof(struct timeval));
	timersub(end, start, res);
	stat->dequeueTime += res->tv_sec * 10e6 + res->tv_usec;
}
