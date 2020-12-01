/*
 * Process table module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "process_table.h"

/*
 * Create a ppn that is used by a process
 * 
 * ppn: the physical page number
 * 
 * return the ppn
 */
PPN *create_ppn(long ppn) {
	PPN *p = malloc(sizeof(PPN));
	if (p == NULL) {
		exit(1);
	}
	p->ppn = ppn;
	p->next = NULL;
	return p;
}

/*
 * Add a ppn to a process and mark it runnable
 * 
 * proc_t: the process table
 * pid: the pid of the process
 * ppn: the ppn used by the process
 */
void add_ppn(ProcT *proc_t, unsigned long pid, long ppn) {
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
	proc_t->runnable++;
}
/*
 * Remove a ppn to a process and mark it runnable
 * 
 * proc_t: the process table
 * pid: the pid of the process
 * ppn: the ppn used by the process
 */
void delete_ppn(ProcT *proc_t, unsigned long pid, long ppn) {
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

/*
 * Create a process table entry
 * 
 * pid: the pid of the process
 * byte: the byte in the trace file where the pid occur
 * 
 * return: the process table entry created
 */
ProcTE *create_proc_te(unsigned long pid, long byte) {
	ProcTE *proc_te = malloc(sizeof(ProcTE));
	if (proc_te == NULL) {
		exit(1);
	}
	proc_te->pid = pid;
	proc_te->first_byte = byte;
	proc_te->last_byte = byte;
	proc_te->runnable = 1;
	proc_te->fp = NULL;
	proc_te->ppn_head = NULL;
	proc_te->ppn_tail = NULL;
	proc_te->next = NULL;
	return proc_te;
}

/*
 * Create process table
 * 
 * return: the process table
 */
ProcT *create_proc_t() {
	ProcT *proc_t = malloc(sizeof(ProcT));
	if (proc_t == NULL) {
		exit(1);
	}
	proc_t->head = NULL;
	proc_t->tail = NULL;
	proc_t->runnable = 0;
	return proc_t;
}

/*
 * Find a process table entry
 * 
 * proc_t: the process table
 * pid: the pid of the process
 * 
 * return: the process table entry found
 */
ProcTE *find_proc_te(ProcT *proc_t, unsigned long pid) {
	ProcTE *current = proc_t->head;
	while (current) {
		if (current->pid == pid) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

/*
 * Delete a process table entry
 * 
 * proc_t: the process table
 * pid: the pid of the process
 * 
 * return: the process table entry deleted
 */
ProcTE *delete_proc_te(ProcT *proc_t, unsigned long pid) {
	if (proc_t->head->pid == pid) {
		ProcTE *prev_head = proc_t->head;
		proc_t->head = proc_t->head->next;
		proc_t->runnable--;
		return prev_head;
	}
	ProcTE *current = proc_t->head;
	ProcTE *previous = NULL;
	while (current) {
		if (current->pid == pid) {
			previous->next = current->next;
			proc_t->runnable--;
			return current;
		}
		previous = current;
		current = current->next;
	}
	return NULL;
}

/*
 * Update a process table entry
 * 
 * proc_t: the process table
 * pid: the pid of the process
 * byte: the byte in the trace file where the pid occur
 */
void update_proc_te_trace(ProcT *proc_t, unsigned long pid, long byte) {
	ProcTE *proc_te = find_proc_te(proc_t, pid);
	if (proc_te) {
		proc_te->last_byte = byte;
	} else {
		proc_te = create_proc_te(pid, byte);
		if (proc_t->head) {
			proc_t->tail->next = proc_te;
			proc_t->tail = proc_t->tail->next;
		} else {
			proc_t->head = proc_te;
			proc_t->tail = proc_t->head;
		}
		proc_t->runnable++;
	}
}

/*
 * Find a runnable process that has the least file pointer
 * 
 * proc_t: the process table
 * 
 * return: the process table entry found
 */
ProcTE *find_runnable_least_fp(ProcT *proc_t) {
	ProcTE *current = proc_t->head;
	ProcTE *least = NULL;
	while (current) {
		if (current->runnable &&
			(least == NULL || ftell(current->fp) < ftell(least->fp))) {
			least = current;
		}
		current = current->next;
	}
	return least;
}

/*
 * Advance the file pointer of the process to the next line that has the same PID
 * 
 * proc_te: the process table entry
 * 
 * return: 1 if the end of the file is reached or the process is finished, 0 
 * otherwise
 */
int advance_to_next_available_line(ProcTE *proc_te) {
	FILE *fp = proc_te->fp;
	char *line = NULL;
	char *token;
	size_t len = 0;
	ssize_t size;
	unsigned long pid = 0;
	
	// while before the last occurrence or before the eof
	while (ftell(fp) <= proc_te->last_byte &&
		(size = getline(&line, &len, fp)) != -1) {
		line[size - 1] = '\0';
		token = strtok(line, " ");
		pid = strtol(token, NULL, 10);
		if (pid == proc_te->pid) {
			if (fseek(fp, -size, SEEK_CUR) < 0) {
				exit(1);
			}
			return 0;
		}
	}
	return 1;
}

/*
 * Print the process table
 * 
 * proc_t: process table
 */
void print_proc_t(ProcT *proc_t) {
	ProcTE *current = proc_t->head;
	fprintf(stderr, "\t==========process table==========\n");
	while (current) {
		fprintf(stderr, "\tpid: %ld\tfirst: %ld\tlast: %ld\t",
			current->pid, current->first_byte, current->last_byte);
		if (current->fp) {
			fprintf(stderr, "runnable: %d\tfp: %.4ld\t",
				current->runnable, ftell(current->fp));
		}
		PPN *ppn = current->ppn_head;
		fprintf(stderr, "ppn: ");
		while (ppn) {
			fprintf(stderr, "%ld ", ppn->ppn);
			ppn = ppn->next;
		}
		fprintf(stderr, "\n");
		current = current->next;
	}
	fprintf(stderr, "\trunning process: %d\n", proc_t->runnable);
	fprintf(stderr, "\t==========process table end=======\n");
}
