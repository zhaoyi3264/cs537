#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "trace_parser.h"
#include "page_table.h"
#include "disk_queue.h"
#include "page_frame.h"
#include "schedule_algo.h"

int exponent(long int x) {
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

char *parse_cmd(int argc, char *argv[], long *page_frame_num) {
	int opt = 0;
	int mem_size = 1;
	long int page_size = 4096;
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
				trace_file = malloc(strlen(optarg) + 1);
				sprintf(trace_file, "%s", optarg);
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
	printf("page size: %ld\n mem size: %d\n  pf size: %ld\n    trace: %s\n",
		page_size, mem_size, *page_frame_num, trace_file);
	return trace_file;
}

void disk_io(ProcT *proc_t, PT *pt, PF *pf, long pid, long vpn) {
	long ppn = 0;
	// add page frame and page table
	ppn = add_pfn(pf, pid, vpn);
	if (ppn == -1) {
		PFN *replaced = replace_pfn(pf, pid, vpn);
		ppn = replaced->ppn;
		delete_pte(pt, replaced->pid, replaced->vpn);
		// update process table
		remove_ppn(proc_t, replaced->pid, ppn);
	}
	add_pte(pt, pid, vpn, ppn);
	// update process table
	add_ppn(proc_t, pid, ppn);
}

int main(int argc, char * argv[]) {
	long page_frame_num = 0;
	char *fname = parse_cmd(argc, argv, &page_frame_num);
	ProcTE *proc_te = NULL;
	ProcT *proc_t = parse_trace(fname);
	print_proc_t(proc_t);

	//~ long real = 0;
	//~ long cpu;

	FILE *fp = open_trace(fname);
	char * line = NULL;
	size_t len = 0;
	ssize_t size;
	long line_num = 0;
	
	long pid = 0;
	long vpn = 0;
	long ppn = 0;
	
	PT *pt = create_pt();
	Node *node = NULL;
	DiskQueue *dq = create_dq(5);
	PFN *pfn = NULL;
	PF *pf = create_pf(page_frame_num);
	//~ while ((size = getline(&line, &len, fp)) != -1) {
		
	//~ }
	while (1) {
		if ((node = advance(dq))) {
			pid = node->pid;
			vpn = node->vpn;
			disk_io(proc_t, pt, pf, pid, vpn);
			node = NULL;
		}
		if ((proc_te = find_runnable_least_fp(proc_t))) {
			// find
			if ((ppn = find_pte(pt, pid, vpn)) != -1) {
				find_pfn(pf, ppn);
				continue;
			}
		}
	}
}
