#include <semaphore.h>

struct Queue {
	int size;
	int capacity;
	char **data;
	sem_t *mutex;
	sem_t *wait;
	
	int enqueueCount;
	int dequeueCount;
	int enqueueBlockCount;
	int dequeueBlockCount;
};

struct Queue *CreateStringQueue(int capacity);

void EnqueueString(struct Queue *q, char *string);

char * DequeueString(struct Queue *q);

void PrintQueueStats(struct Queue *q);
