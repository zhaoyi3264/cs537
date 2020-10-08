typedef struct Statistics {
	int enqueueCount;
	int dequeueCount;
	suseconds_t enqueueTime;
	suseconds_t dequeueTime;
} Statistics;

void update_enqueue(Statistics *stat, struct timeval *start, struct timeval *end);

void update_dequeue(Statistics *stat, struct timeval *start, struct timeval *end);
