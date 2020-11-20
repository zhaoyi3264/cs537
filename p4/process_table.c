#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "process_table.h"

ProcTE *create_proc_te(char *pid, int trace) {
	ProcTE *proc_te = malloc(sizeof(ProcTE));
	proc_te->pid = malloc(strlen(pid) + 1);
	sprintf(proc_te->pid, "%s", pid);
	proc_te->first_trace = trace;
	proc_te->last_trace = trace;
	proc_te->runnable = 1;
	proc_te->next = NULL;
	return proc_te;
}

ProcT *create_proc_t() {
	ProcT *proc_t = malloc(sizeof(ProcT));
	proc_t->head = NULL;
	proc_t->tail = NULL;
	return proc_t;
}

ProcTE *find_proc_te(ProcT *proc_t, char *pid) {
	ProcTE *current = proc_t->head;
	while (current) {
		if (strcmp(current->pid, pid) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void update_proc_te_trace(ProcT *proc_t, char *pid, int trace) {
	ProcTE *proc_te = find_proc_te(proc_t, pid);
	if (proc_te) {
		proc_te->last_trace = trace;
	} else {
		proc_te = create_proc_te(pid, trace);
		if (proc_t->head) {
			proc_t->tail->next = proc_te;
			proc_t->tail = proc_t->tail->next;
		} else {
			proc_t->head = proc_te;
			proc_t->tail = proc_t->head;
		}
	}
}

void print_proc_t(ProcT *proc_t) {
	ProcTE *current = proc_t->head;
	printf("==========process table==========\n");
	while (current) {
		printf("pid: %s\tfirst: %d\tlast: %d\trunnable: %d\n", current->pid,
			current->first_trace, current->last_trace, current->runnable);
		current = current->next;
	}
}
