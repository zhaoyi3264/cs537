#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "process_table.h"

PTEntry *create_ptentry(char *pid, int trace) {
	PTEntry *pte = malloc(sizeof(PTEntry));
	pte->pid = malloc(strlen(pid) + 1);
	sprintf(pte->pid, "%s", pid);
	pte->first_trace = trace;
	pte->last_trace = trace;
	pte->runnable = 1;
	pte->next = NULL;
	return pte;
}

PTable *create_ptable() {
	PTable *pt = malloc(sizeof(PTable));
	pt->head = NULL;
	pt->tail = NULL;
	return pt;
}

PTEntry *find_pte(PTable *pt, char *pid) {
	PTEntry *current = pt->head;
	while (current) {
		if (strcmp(current->pid, pid) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void update_pte_trace(PTable *pt, char *pid, int trace) {
	PTEntry *pte = find_pte(pt, pid);
	if (pte) {
		pte->last_trace = trace;
	} else {
		pte = create_ptentry(pid, trace);
		if (pt->head) {
			pt->tail->next = pte;
			pt->tail = pt->tail->next;
		} else {
			pt->head = pte;
			pt->tail = pt->head;
		}
	}
}

void print_pt(PTable *pt) {
	PTEntry *current = pt->head;
	printf("==========process table==========\n");
	while (current) {
		printf("pid: %s\tfirst: %d\tlast: %d\trunnable: %d\n", current->pid,
			current->first_trace, current->last_trace, current->runnable);
		current = current->next;
	}
}
