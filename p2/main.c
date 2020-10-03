#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include "queue.h"

// TODO: run Clang

struct munch_args {
	Queue *in;
	Queue *out;
};

void *reader(void *args) {
	Queue *rd_to_m1 = (Queue *)args;
	int buf_limit = 4096;
	size_t buf_size = buf_limit;
	char *buf = malloc(buf_size);
	int size = 0;
	while (1) {
		size = getline(&buf, &buf_size, stdin);
		//~ printf("input size: %d\n", size);
		// exit on sentinel value
		if (size < 1) {
			EnqueueString(rd_to_m1, NULL);
			//~ printf("reader exit\n");
			pthread_exit(NULL);
		} else if (size > buf_limit) { // skip long lines
			fprintf(stderr, "error: the line is too long\n");
		} else {
			EnqueueString(rd_to_m1, buf);
		}
		buf = malloc(buf_size);
	}
	return NULL;
}

void *munch1(void *args) {
	struct munch_args *munch1_args = (struct munch_args *)args;
	Queue *rd_to_m1 = munch1_args->in;
	Queue *m1_to_m2 = munch1_args->out;
	char *buf;
	char *replace;
	while (1) {
		buf = DequeueString(rd_to_m1);
		// exit on sentinel value
		if (buf == NULL) {
			EnqueueString(m1_to_m2, NULL);
			//~ printf("munch1 exit\n");
			pthread_exit(NULL);
		}
		// replace ' ' with '*'
		while ((replace = index(buf, ' '))) {
			*replace = '*';
		}
		//~ printf("munch1: %s\n", buf);
		EnqueueString(m1_to_m2, buf);
	}

	return NULL;
}

void *munch2(void *args) {
	struct munch_args *munch2_args = (struct munch_args *)args;
	Queue *m1_to_m2 = munch2_args->in;
	Queue *m2_to_wt = munch2_args->out;
	char *buf;
	while (1) {
		buf = DequeueString(m1_to_m2);
		// exit on sentinel value
		if (buf == NULL) {
			EnqueueString(m2_to_wt, NULL);
			//~ printf("munch2 exit\n");
			pthread_exit(NULL);
		}
		// replace lower case characters with upper case ones
		for (int i = 0; buf[i]; i++) {
			if (islower(buf[i])) {
				buf[i] = toupper(buf[i]);
			}
		}
		//~ printf("munch2: %s\n", buf);
		EnqueueString(m2_to_wt, buf);
	}
	return NULL;
}

void *writer(void *args) {
	Queue *m2_to_wt = (Queue *)args;
	char *buf;
	while (1) {
		buf = DequeueString(m2_to_wt);
		// exit on sentinel value
		if (buf == NULL) {
			//~ printf("writer exit\n");
			pthread_exit(NULL);
		}
		// print to stdout and free the buffer
		printf("%s", buf);
		free(buf);
	}
	return NULL;
}

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
	struct munch_args *munch1_args = malloc(sizeof(struct munch_args));
	munch1_args->in = rd_to_m1;
	munch1_args->out = m1_to_m2;
	pthread_create(&munch1_thread, NULL, munch1, (void *)munch1_args);
	
	pthread_t munch2_thread;
	struct munch_args *munch2_args = malloc(sizeof(struct munch_args));
	munch2_args->in = m1_to_m2;
	munch2_args->out = m2_to_wt;
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
	printf("Writer processed %d strings\n", m2_to_wt->dequeueCount);
	
	printf("\nReader to Munch1 statistics\n");
	PrintQueueStats(rd_to_m1);
	printf("\nMunch1 to Munch2 statistics\n");
	PrintQueueStats(m1_to_m2);
	printf("\nMunch2 to Writer statistics\n");
	PrintQueueStats(m2_to_wt);
}
