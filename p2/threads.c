/*
 * Main driver module
 * 
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include <pthread.h>

#include "queue.h"

/*
 * Runnable for reader thread. Read inputs from stdin
 * 
 * args: queue struct
 * 
 * return: NULL
 */
void *reader(void *args) {
	Queue *rd_to_m1 = (Queue *)args;
	int buf_limit = 4096;
	size_t buf_size = buf_limit;
	char *buf = NULL;
	int size = 0;
	while (1) {
		buf = malloc(buf_size);
		if (buf == NULL) {
			exit(1);
		}
		size = getline(&buf, &buf_size, stdin);
		// exit on sentinel value
		if (size < 1) {
			EnqueueString(rd_to_m1, NULL);
			pthread_exit(NULL);
		} else if (size > buf_limit) { // skip long lines
			fprintf(stderr, "error: the line is too long\n");
		} else {
			// replace '\n' with '\0'
			buf[size - 1] = '\0';
			EnqueueString(rd_to_m1, buf);
		}
	}
	return NULL;
}

/*
 * Runnable for munch1 thread. Convert the space in the string to '*'
 * 
 * args: queue structs
 * 
 * return: NULL
 */
void *munch1(void *args) {
	Queue **munch1_args = (Queue **)args;
	Queue *rd_to_m1 = munch1_args[0];
	Queue *m1_to_m2 = munch1_args[1];
	char *buf;
	char *replace;
	while (1) {
		buf = DequeueString(rd_to_m1);
		// exit on sentinel value
		if (buf == NULL) {
			EnqueueString(m1_to_m2, NULL);
			pthread_exit(NULL);
		}
		// replace ' ' with '*'
		while ((replace = index(buf, ' '))) {
			*replace = '*';
		}
		EnqueueString(m1_to_m2, buf);
	}

	return NULL;
}

/*
 * Runnable for munch2 thread. Convert lower case letters in the string to upper
 * case ones
 * 
 * args: queue structs
 * 
 * return: NULL
 */
void *munch2(void *args) {
	Queue **munch2_args = (Queue **)args;
	Queue *m1_to_m2 = munch2_args[0];
	Queue *m2_to_wt = munch2_args[1];
	char *buf;
	while (1) {
		buf = DequeueString(m1_to_m2);
		// exit on sentinel value
		if (buf == NULL) {
			EnqueueString(m2_to_wt, NULL);
			pthread_exit(NULL);
		}
		// replace lower case characters with upper case ones
		for (int i = 0; buf[i]; i++) {
			if (islower(buf[i])) {
				buf[i] = toupper(buf[i]);
			}
		}
		EnqueueString(m2_to_wt, buf);
	}
	return NULL;
}

/*
 * Runnable for writer thread. Write the string to stdout
 * 
 * args: queue struct
 * 
 * return: NULL
 */
void *writer(void *args) {
	Queue *m2_to_wt = (Queue *)args;
	char *buf;
	while (1) {
		buf = DequeueString(m2_to_wt);
		// exit on sentinel value
		if (buf == NULL) {
			printf("\nWriter processed %d strings\n",
				m2_to_wt->stat->dequeueCount - 1);
			pthread_exit(NULL);
		}
		// print to stdout and free the buffer
		printf("%s\n", buf);
		free(buf);
	}
	return NULL;
}
