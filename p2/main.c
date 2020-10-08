#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include "queue.h"
#include "threads.h"

int main() {
	// create queues
	int queue_size = 10;
	Queue *rd_to_m1 = CreateStringQueue(queue_size);
	Queue *m1_to_m2 = CreateStringQueue(queue_size);
	Queue *m2_to_wt = CreateStringQueue(queue_size);
	
	// create threads
	pthread_t reader_thread;
	pthread_create(&reader_thread, NULL, reader, rd_to_m1);
	
	pthread_t munch1_thread;
	Queue **munch1_args = malloc(2 * sizeof(Queue *));
	munch1_args[0] = rd_to_m1;
	munch1_args[1] = m1_to_m2;
	pthread_create(&munch1_thread, NULL, munch1, (void *)munch1_args);
	
	pthread_t munch2_thread;
	Queue **munch2_args = malloc(2 * sizeof(Queue *));
	munch2_args[0] = m1_to_m2;
	munch2_args[1] = m2_to_wt;
	pthread_create(&munch2_thread, NULL, munch2, (void *)munch2_args);
	
	pthread_t writer_thread;
	pthread_create(&writer_thread, NULL, writer, m2_to_wt);
	
	// join threads
	pthread_join(reader_thread, NULL);
	pthread_join(munch1_thread, NULL);
	pthread_join(munch2_thread, NULL);
	pthread_join(writer_thread, NULL);
	
	// free args
	free(munch1_args);
	free(munch2_args);
	
	// print stats
	printf("Writer processed %d strings\n", m2_to_wt->stat->dequeueCount);
	
	printf("\nReader to Munch1 statistics\n");
	PrintQueueStats(rd_to_m1);
	printf("\nMunch1 to Munch2 statistics\n");
	PrintQueueStats(m1_to_m2);
	printf("\nMunch2 to Writer statistics\n");
	PrintQueueStats(m2_to_wt);
	
	// free queues and stats
}
