#include <stdio.h>
#include <stdlib.h>

#include "disk_queue.h"

Node *create_node(long pid, long vpn) {
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

void enqueue(DiskQueue *dq, long pid, long vpn) {
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

Node *advance(DiskQueue*dq) {
	if (dq->head) {
		dq->count_down--;
		if (dq->count_down == 0) {
			dq->count_down = dq->cool_down;
			return dequeue(dq);
		}
	}
	return NULL;
}

void print_dq(DiskQueue *dq) {
	printf("==========disk queue==========\n");
	Node *current = dq->head;
	while (current) {
		printf("(%ld, %ld)", current->pid, current->vpn);
		current = current->next;
		if (current) {
			printf("->");
		}
	}
	printf("\n==========disk queue end=======\n");
}
