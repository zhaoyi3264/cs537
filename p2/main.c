/*
 * Main driver module
 * 
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "queue.h"
#include "threads.h"

/*
 * Main method
 * 
 * return: exit status
 */
int main() {
	// create queues
	int queue_size = 10;
	Queue *rd_to_m1 = CreateStringQueue(queue_size);
	Queue *m1_to_m2 = CreateStringQueue(queue_size);
	Queue *m2_to_wt = CreateStringQueue(queue_size);
	
	// create threads
	pthread_t reader_thread;
	if (pthread_create(&reader_thread, NULL, reader, rd_to_m1)) {
		fprintf(stderr, "error creating thread\n");
		exit(1);
	}
	
	pthread_t munch1_thread;
	Queue **munch1_args = malloc(2 * sizeof(Queue *));
	if (munch1_args == NULL) {
		exit(1);
	}
	munch1_args[0] = rd_to_m1;
	munch1_args[1] = m1_to_m2;
	if (pthread_create(&munch1_thread, NULL, munch1, (void *)munch1_args)) {
		fprintf(stderr, "error creating thread\n");
		exit(1);
	}
	
	pthread_t munch2_thread;
	Queue **munch2_args = malloc(2 * sizeof(Queue *));
	if (munch2_args == NULL) {
		exit(1);
	}
	munch2_args[0] = m1_to_m2;
	munch2_args[1] = m2_to_wt;
	if (pthread_create(&munch2_thread, NULL, munch2, (void *)munch2_args)) {
		fprintf(stderr, "error creating thread\n");
		exit(1);
	}
	
	pthread_t writer_thread;
	if (pthread_create(&writer_thread, NULL, writer, m2_to_wt)) {
		fprintf(stderr, "error creating thread\n");
		exit(1);
	}
	
	// join threads
	if (pthread_join(reader_thread, NULL) || pthread_join(munch1_thread, NULL) ||
		pthread_join(munch2_thread, NULL) || pthread_join(writer_thread, NULL)) {
		fprintf(stderr, "error joining thread\n");
		exit(1);
	}
	
	// free args
	free(munch1_args);
	free(munch2_args);
	
	// print stats
	fprintf(stderr, "\nReader to Munch1 Queue statistics\n");
	PrintQueueStats(rd_to_m1);
	fprintf(stderr, "\nMunch1 to Munch2 Queue statistics\n");
	PrintQueueStats(m1_to_m2);
	fprintf(stderr, "\nMunch2 to Writer Queue statistics\n");
	PrintQueueStats(m2_to_wt);
}
