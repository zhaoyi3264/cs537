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

void enqueue(DiskQueue *dq, long pid, long vpn);

Node *advance(DiskQueue*dq);

void print_dq(DiskQueue *dq);

#endif
