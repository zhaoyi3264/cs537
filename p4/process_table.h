#ifndef PROCESS_TABLE_H
#define PROCESS_TABLE_H

typedef struct ProcTE {
	long pid;
	long first_trace;
	long last_trace;
	int runnable;
	struct ProcTE *next;
} ProcTE;

typedef struct ProcT {
	ProcTE *head;
	ProcTE *tail;
} ProcT;

ProcTE *create_proc_te(long pid, long trace);

ProcT *create_proc_t();

ProcTE *find_proc_te(ProcT *proc_t, long pid);

void delete_proc_te(ProcT *proc_t, long pid);

void update_proc_te_trace(ProcT *proc_t, long pid, long trace);

void print_proc_t(ProcT *proc_t);

#endif
