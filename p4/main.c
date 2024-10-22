/*
 * Main driver module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */
 
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "trace_parser.h"
#include "page_table.h"
#include "disk_queue.h"
#include "statistics.h"
#include "schedule_algo.h"

/* 
 * Calculate the exponent of a number, assuming it is raised to the power of 2
 * 
 * x: the number
 * 
 * return: the exponent
 */
int exponent(long x) {
	int exp = 0;
	while (1) {
		x = x >> 1;
		if (x > 0) {
			exp++;
		} else {
			break;
		}
	}
	return exp;
}

/* 
 * Parse the command line arguments and calculate number of page frames
 * 
 * argc: argument count
 * argv: argument values
 * page_frame_num: pointer to number of page frames
 * 
 * return: trace file name
 */
char *parse_cmd(int argc, char *argv[], long *page_frame_num) {
	int opt = 0;
	int mem_size = 1;
	long page_size = 4096;
	char *trace_file = NULL;
	while ((opt = getopt(argc, argv, "-p:m:")) != -1) {
		switch (opt) {
			case 'p':
				if (is_number(optarg)) {
					page_size = strtol(optarg, NULL, 10);
				} else {
					fprintf(stderr, "error: page size is not a number\n");
					exit(1);					
				}
				break;
			case 'm':
				if (is_number(optarg)) {
					mem_size = atoi(optarg);
				} else {
					fprintf(stderr, "error: memory size is not a number\n");
					exit(1);
				}
				break;
			case 1:
				if (trace_file) {
					fprintf(stderr, "error: multilple trace file specified\n");
					exit(1);
				}
				trace_file = malloc(strlen(optarg) + 1);
				if (trace_file == NULL || sprintf(trace_file, "%s", optarg) < 0) {
					exit(1);
				}
				break;
			default:
				exit(1);
		}
	}
	
	int exp = exponent(page_size);
	if ((int)pow(2, exp) != page_size) {
		fprintf(stderr, "error: page size must be a power of two\n");
		exit(1);
	}
	*page_frame_num = pow(2, 20 - exp) * mem_size;
	if (*page_frame_num <= 0) {
		fprintf(stderr, "error: page size must be greater than real memory size\n");
		exit(1);
	}
	return trace_file;
}

/*
 * Perform disk I/O. Add vitural pages into RAM, and replace existing page frame
 * if necessary
 * 
 * proc_t: the process table
 * pt: the page table
 * pf: the page frame
 * pid: the pid of the trace
 * vpn: the vpn of the trace
 */
void disk_io(ProcT *proc_t, PT *pt, PF *pf, unsigned long pid, unsigned long vpn) {
	long ppn = -1;
	// add page frame and page table
	ppn = add_pfn(pf, pid, vpn);
	if (ppn == -1) {
		PFN *replaced = replace_pfn(pf, pid, vpn);
		ppn = replaced->ppn;
		delete_pte(pt, replaced->pid, replaced->vpn);
		// update process table
		delete_ppn(proc_t, replaced->pid, ppn);
		free(replaced);
	}
	add_pte(pt, pid, vpn, ppn);
	add_ppn(proc_t, pid, ppn);
}

/*
 * Terminate a process
 * 
 * proc_t: the process table
 * pt: the page table
 * pf: the page frame
 * pid: the pid of the trace
 * vpn: the vpn of the trace
 */
void terminate_proc(ProcT *proc_t, PT *pt, PF *pf, unsigned long pid) {
	ProcTE *proc_te = delete_proc_te(proc_t, pid);
	// remove relavent page table entries
	delete_ptes(pt, pid);
	// remove relavent page frames
	PPN *previous_ppn = NULL;
	PPN *current_ppn = proc_te->ppn_head;
	while (current_ppn) {
		delete_pfn(pf, current_ppn->ppn);
		previous_ppn = current_ppn;
		current_ppn = current_ppn->next;
		free(previous_ppn);
	}
	if (fclose(proc_te->fp) != 0) {
		exit(1);
	}
	free(proc_te);
}

/*
 * Main method
 * 
 * argc: argument count
 * argv: argument values
 * 
 * return: exit status
 */
int main(int argc, char * argv[]) {
	// create necessary variables and structs
	long page_frame_num = 0;
	char *fname = parse_cmd(argc, argv, &page_frame_num);
	ProcTE *proc_te = NULL;
	ProcT *proc_t = parse_trace(fname);
	// for reading trace file
	char *line = NULL;
	char *token = NULL;
	size_t len = 0;
	ssize_t size;
	// for executing trace
	unsigned long pid = 0;
	unsigned long vpn = 0;
	long ppn = 0;
	long elapse = 0;
	// for simulating
	PT *pt = create_pt();
	Node *node = NULL;
	DiskQueue *dq = create_dq(2000000);
	PF *pf = create_pf(page_frame_num);
	Stat *stat = create_stat();
	// main loop
	while (1) {
		if (proc_t->runnable == 0) {
			// all blocked, fast forward and do the I/O
			node = fast_forward(dq, &elapse);
			tick(stat, elapse, (pf->size) * elapse, 0);
			disk_io(proc_t, pt, pf, node->pid, node->vpn);
			free(node);
			node = NULL;
		} else if ((proc_te = find_runnable_least_fp(proc_t))) {
			// some runnable processes
			advance(dq);
			// read and parse the trace from the file pointer of that PID
			size = getline(&line, &len, proc_te->fp);
			line[size - 1] = '\0';
			token = strtok(line, " ");
			pid = strtoul(token, NULL, 10);
			token = strtok(NULL, " ");
			vpn = strtoul(token, NULL, 10);
			if ((ppn = find_pte(pt, pid, vpn)) != -1) {
				// (pid, vpn) found in page table
				find_pfn(pf, ppn);
				mem_ref(stat);
				// advance the file pointer of that PID to the next line with
				// the same PID
				if (advance_to_next_available_line(proc_te)) {
					// terminate
					terminate_proc(proc_t, pt, pf, pid);
				}
			} else {
				// page fault
				enqueue(dq, pid, vpn);
				proc_te->runnable = 0;
				proc_t->runnable--;
				if (fseek(proc_te->fp, -size, SEEK_CUR) < 0) {
					exit(1);
				}
				page_in(stat);
			}
		}
		tick(stat, 1, pf->size, proc_t->runnable);
		// exit when no processes
		if (proc_t->head == NULL) {
			break;
		}
	}
	print_stat(stat, page_frame_num);
}
