#ifndef PROCESS_TABLE_H
#define PROCESS_TABLE_H

typedef struct PTEntry {
	char *pid;
	int first_trace;
	int last_trace;
	int runnable;
	struct PTEntry *next;
} PTEntry;

typedef struct PTable {
	PTEntry *head;
	PTEntry *tail;
} PTable;

PTEntry *create_ptentry(char *pid, int trace);

PTable *create_ptable();

PTEntry *find_pte(PTable *pt, char *pid);

void update_pte_trace(PTable *pt, char *pid, int trace);

void print_pt(PTable *pt);

#endif
