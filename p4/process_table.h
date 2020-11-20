#ifndef PROCESS_TABLE_H
#define PROCESS_TABLE_H

typedef struct ProcTE {
	char *pid;
	int first_trace;
	int last_trace;
	int runnable;
	struct ProcTE *next;
} ProcTE;

typedef struct ProcT {
	ProcTE *head;
	ProcTE *tail;
} ProcT;

ProcTE *create_proc_te(char *pid, int trace);

ProcT *create_proc_t();

ProcTE *find_proc_te(ProcT *proc_t, char *pid);

void update_proc_te_trace(ProcT *proc_t, char *pid, int trace);

void print_proc_t(ProcT *proc_t);

#endif
