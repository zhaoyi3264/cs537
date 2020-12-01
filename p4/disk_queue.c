#include <stdio.h>
#include <stdlib.h>

#include "disk_queue.h"

Node *create_node(unsigned long pid, unsigned long vpn) {
	Node *node = malloc(sizeof(Node));
	node->pid = pid;
	node->vpn = vpn;
	node->next = NULL;
	return node;
}

DiskQueue *create_dq(long cool_down) {
	DiskQueue *dq = malloc(sizeof(DiskQueue));
	dq->head = NULL;
	dq->tail = NULL;
	dq->cool_down = cool_down;
	dq->count_down = cool_down;
	return dq;
}

void enqueue(DiskQueue *dq, unsigned long pid, unsigned long vpn) {
	Node *node = create_node(pid, vpn);
	if (dq->head) {
		dq->tail->next = node;
	} else {
		dq->head = node;
	}
	dq->tail = node;
}

Node *dequeue(DiskQueue *dq) {
	Node *result = NULL;
	if (dq->head) {
		result = dq->head;
		dq->head = dq->head->next;
	}
	return result;
}

void advance(DiskQueue*dq) {
	if (dq->head) {
		dq->count_down--;
		if (dq->count_down == 0) {
			dq->count_down = dq->cool_down;
		}
	}
}

Node *fast_forward(DiskQueue *dq, long *elapse) {
	if (dq->head) {
		*elapse = dq->count_down - 1;
		dq->count_down = dq->cool_down;
	}
	return dequeue(dq);
}

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
