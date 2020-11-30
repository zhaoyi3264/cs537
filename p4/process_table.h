#ifndef PROCESS_TABLE_H
#define PROCESS_TABLE_H

typedef struct PPN {
	long ppn;
	struct PPN *next;
} PPN;

typedef struct ProcTE {
	unsigned long pid;
	long first_byte;
	long last_byte;
	
	int runnable;
	FILE *fp;
	PPN *ppn_head;
	PPN *ppn_tail;
	
	struct ProcTE *next;
} ProcTE;

typedef struct ProcT {
	ProcTE *head;
	ProcTE *tail;
	int runnable;
} ProcT;

void add_ppn(ProcT *proc_t, unsigned long pid, long ppn);

void delete_ppn(ProcT *proc_t, unsigned long pid, long ppn);

ProcTE *create_proc_te(unsigned long pid, long trace);

ProcT *create_proc_t();

ProcTE *find_proc_te(ProcT *proc_t, unsigned long pid);

ProcTE *delete_proc_te(ProcT *proc_t, unsigned long pid);

void update_proc_te_trace(ProcT *proc_t, unsigned long pid, long trace);

ProcTE *find_runnable_least_fp(ProcT *proc_t);

int advance_to_next_available_line(ProcTE *proc_te);

void print_proc_t(ProcT *proc_t);

#endif
