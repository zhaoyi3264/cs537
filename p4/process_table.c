#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "process_table.h"

ProcTE *create_proc_te(long pid, long trace) {
	ProcTE *proc_te = malloc(sizeof(ProcTE));
	proc_te->pid = pid;
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

ProcTE *find_proc_te(ProcT *proc_t, long pid) {
	ProcTE *current = proc_t->head;
	while (current) {
		if (current->pid == pid) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void delete_proc_te(ProcT *proc_t, long pid) {
	if (proc_t->head->pid == pid) {
		ProcTE *prev_head = proc_t->head;
		proc_t->head = proc_t->head->next;
		free(prev_head);
		return;
	}
	ProcTE *current = proc_t->head;
	ProcTE *previous = NULL;
	while (current) {
		if (current->pid == pid) {
			previous->next = current->next;
			free(current);
		}
		previous = current;
		current = current->next;
	}
}

void update_proc_te_trace(ProcT *proc_t, long pid, long trace) {
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
		printf("pid: %ld\tfirst: %ld\tlast: %ld\trunnable: %d\n", current->pid,
			current->first_trace, current->last_trace, current->runnable);
		current = current->next;
	}
	printf("==========process table end=======\n");
}
