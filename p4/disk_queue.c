/*
 * Disk queue module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */

#include <stdio.h>
#include <stdlib.h>

#include "disk_queue.h"

/*
 * Create a node in queue
 * 
 * pid: the pid
 * vpn: the vpn
 * 
 * return: the node created
 */
Node *create_node(unsigned long pid, unsigned long vpn) {
	Node *node = malloc(sizeof(Node));
	if (node == NULL) {
		exit(1);
	}
	node->pid = pid;
	node->vpn = vpn;
	node->next = NULL;
	return node;
}

/*
 * Create disk queue
 * 
 * cool_down: the I/O waiting time of the disk queue
 * 
 * return: the disk queue created
 */
DiskQueue *create_dq(long cool_down) {
	DiskQueue *dq = malloc(sizeof(DiskQueue));
	if (dq == NULL) {
		exit(1);
	}
	dq->head = NULL;
	dq->tail = NULL;
	dq->cool_down = cool_down;
	dq->count_down = cool_down;
	return dq;
}

/*
 * Enqueue a (pid, vpn) into the queue
 * 
 * dq: the disk queue
 * pid: the pid
 * vpn: the vpn
 */
void enqueue(DiskQueue *dq, unsigned long pid, unsigned long vpn) {
	Node *node = create_node(pid, vpn);
	if (dq->head) {
		dq->tail->next = node;
	} else {
		dq->head = node;
	}
	dq->tail = node;
}

/*
 * Dequeue from the queue
 * 
 * dq: the disk queue
 * 
 * return: the node dequeued
 */
Node *dequeue(DiskQueue *dq) {
	Node *result = NULL;
	if (dq->head) {
		result = dq->head;
		dq->head = dq->head->next;
	}
	return result;
}

/*
 * Decrement the count down, reset if it reached 0
 * 
 * dq: the disk queue
 */
void advance(DiskQueue *dq) {
	if (dq->head) {
		dq->count_down--;
		if (dq->count_down == 0) {
			dq->count_down = dq->cool_down;
		}
	}
}

/*
 * Fast forward the queue
 * 
 * dq: the disk queue
 * elapse: the time skipped in ticks
 * 
 * return: the node dequeued
 */
Node *fast_forward(DiskQueue *dq, long *elapse) {
	if (dq->head) {
		*elapse = dq->count_down - 1;
		dq->count_down = dq->cool_down;
	}
	return dequeue(dq);
}

/*
 * Print disk queue
 * 
 * dq: the disk queue
 */
void print_dq(DiskQueue *dq) {
	fprintf(stderr, "\t==========disk queue==========\n\t");
	Node *current = dq->head;
	while (current) {
		fprintf(stderr, "(%lu, %lu)", current->pid, current->vpn);
		current = current->next;
		if (current) {
			fprintf(stderr, "->");
		}
	}
	fprintf(stderr, "\n\t==========disk queue end=======\n");
}
