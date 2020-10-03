#include <semaphore.h>

typedef struct Queue {
	int size;
	char **data;
	sem_t *mutex;
	sem_t *empty;
	sem_t *full;
	
	int enqueueCount;
	int dequeueCount;
	int enqueueTime;
	int dequeueTime;
} Queue;

Queue *CreateStringQueue(int size);

void EnqueueString(Queue *q, char *string);

char * DequeueString(Queue *q);

void PrintQueueStats(Queue *q);
