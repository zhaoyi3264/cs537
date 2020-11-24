#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "process_table.h"

PPN *create_ppn(long ppn) {
	PPN *p = malloc(sizeof(PPN));
	p->ppn = ppn;
	p->next = NULL;
	return p;
}

void add_ppn(ProcT *proc_t, long pid, long ppn) {
	ProcTE *proc_te = find_proc_te(proc_t, pid);
	if (proc_te == NULL) {
		return;
	}
	PPN *p = create_ppn(ppn);
	if (proc_te->ppn_head) {
		proc_te->ppn_tail->next = p;
	} else {
		proc_te->ppn_head = p;
	}
	proc_te->ppn_tail = p;
	proc_te->runnable = 1;
}

void remove_ppn(ProcT *proc_t, long pid, long ppn) {
	ProcTE *proc_te = find_proc_te(proc_t, pid);
	if (proc_te == NULL) {
		return;
	}
	PPN *current = proc_te->ppn_head;
	PPN *previous = NULL;
	if (current && current->ppn == ppn) {
		proc_te->ppn_head = proc_te->ppn_head->next;
		free(current);
		return;
	}
	while(current) {
		if (current && current->ppn == ppn) {
			previous->next = current->next;
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
}

ProcTE *create_proc_te(long pid, long trace) {
	ProcTE *proc_te = malloc(sizeof(ProcTE));
	proc_te->pid = pid;
	proc_te->first_trace = trace;
	proc_te->last_trace = trace;
	proc_te->runnable = 0;
	proc_te->fp = NULL;
	proc_te->line_num = 0;
	proc_te->ppn_head = NULL;
	proc_te->ppn_tail = NULL;
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

ProcTE *delete_proc_te(ProcT *proc_t, long pid) {
	if (proc_t->head->pid == pid) {
		ProcTE *prev_head = proc_t->head;
		proc_t->head = proc_t->head->next;
		return prev_head;
	}
	ProcTE *current = proc_t->head;
	ProcTE *previous = NULL;
	while (current) {
		if (current->pid == pid) {
			previous->next = current->next;
			return current;
		}
		previous = current;
		current = current->next;
	}
	return NULL;
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

ProcTE *find_runnable_least_fp(ProcT *proc_t) {
	ProcTE *current = proc_t->head;
	ProcTE *least = NULL;
	while (current) {
		if (current->runnable &&
			(least || ftell(current->fp) < ftell(least->fp))) {
			least = current;
		}
		current = current->next;
	}
	return least;
}

void print_proc_t(ProcT *proc_t) {
	ProcTE *current = proc_t->head;
	printf("==========process table==========\n");
	while (current) {
		printf("pid: %ld\tfirst: %ld\tlast: %ld\t",
			current->pid, current->first_trace, current->last_trace);
		if (current->fp) {
			printf("runnable: %d\tfp: %ld\tline: %ld\t",
				current->runnable, ftell(current->fp), current->line_num);
		}
		PPN *ppn = current->ppn_head;
		printf("ppn: ");
		while (ppn) {
			printf("%ld ", ppn->ppn);
			ppn = ppn->next;
		}
		printf("\n");
		current = current->next;
	}
	printf("==========process table end=======\n");
}
