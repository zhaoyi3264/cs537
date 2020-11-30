#ifndef DISK_QUEUE_H
#define DISK_QUEUE_H

typedef struct Node {
	long pid;
	long vpn;
	struct Node *next;
} Node;

typedef struct DiskQueue {
	long cool_down;
	long count_down;
	Node *head;
	Node *tail;
} DiskQueue;

DiskQueue *create_dq(long cool_down);

void enqueue(DiskQueue *dq, unsigned long pid, unsigned long vpn);

Node *advance(DiskQueue*dq);

Node *fast_forward(DiskQueue *dq, long *elapse);

void print_dq(DiskQueue *dq);

#endif
