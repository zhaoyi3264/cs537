#ifndef PROCESS_TABLE_H
#define PROCESS_TABLE_H

typedef struct PPN {
	long ppn;
	struct PPN *next;
} PPN;

typedef struct ProcTE {
	long pid;
	long first_trace;
	long last_trace;
	
	int runnable;
	FILE *fp;
	long line_num;
	PPN *ppn_head;
	PPN *ppn_tail;
	
	struct ProcTE *next;
} ProcTE;

typedef struct ProcT {
	ProcTE *head;
	ProcTE *tail;
} ProcT;

void add_ppn(ProcT *proc_t, long pid, long ppn);

void remove_ppn(ProcT *proc_t, long pid, long ppn);

ProcTE *create_proc_te(long pid, long trace);

ProcT *create_proc_t();

ProcTE *find_proc_te(ProcT *proc_t, long pid);

ProcTE *delete_proc_te(ProcT *proc_t, long pid);

void update_proc_te_trace(ProcT *proc_t, long pid, long trace);

ProcTE *find_runnable_least_fp(ProcT *proc_t);

void print_proc_t(ProcT *proc_t);

#endif
