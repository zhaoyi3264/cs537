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

void disk_io(ProcT *proc_t, PT *pt, PF *pf, unsigned long pid, unsigned long vpn) {
	long ppn = 0;
	// add page frame and page table
	ppn = add_pfn(pf, pid, vpn);
	if (ppn == -1) {
		PFN *replaced = replace_pfn(pf, pid, vpn);
		ppn = replaced->ppn;
		delete_pte(pt, replaced->pid, replaced->vpn);
		// update process table
		delete_ppn(proc_t, replaced->pid, ppn);
		//~ printf("replace (%lu, %lu) -> %ld\n", replaced->pid, replaced->vpn, ppn);
	}
	//~ printf("assign (%lu, %lu) -> %ld\n", pid, vpn, ppn);
	add_pte(pt, pid, vpn, ppn);
	add_ppn(proc_t, pid, ppn);
	//~ print_proc_t(proc_t);
	//~ print_pt(pt);
	//~ print_pf(pf);
}

int main(int argc, char * argv[]) {
	long page_frame_num = 0;
	char *fname = parse_cmd(argc, argv, &page_frame_num);
	ProcTE *proc_te = NULL;
	ProcT *proc_t = parse_trace(fname);
	print_proc_t(proc_t);

	long real = 0;
	long cpu = 0;
	long mem_util = 0;
	long run_proc = 0;
	long tot_mem_ref = 0;
	long tot_page_in = 0;

	char *line = NULL;
	char *token = NULL;
	size_t len = 0;
	ssize_t size;
	
	unsigned long pid = 0;
	unsigned long vpn = 0;
	long ppn = 0;
	
	PT *pt = create_pt();
	Node *node = NULL;
	long cool_down = 2000000;
	//~ long cool_down = 5;
	DiskQueue *dq = create_dq(cool_down);
	//~ page_frame_num = 5;
	PF *pf = create_pf(page_frame_num);
	
	while (1) {
		real++;
		//~ printf("***********\ntick %.6ld\n", real);
		if ((node = advance(dq))) {
			//~ printf("dequeue: (%lu, %lu)\n", node->pid, node->vpn);
			pid = node->pid;
			vpn = node->vpn;
			disk_io(proc_t, pt, pf, pid, vpn);
			node = NULL;
		} else if ((proc_te = find_runnable_least_fp(proc_t))) {
			size = getline(&line, &len, proc_te->fp);
			line[size - 1] = '\0';
			//~ printf("execute trace %s\n", line);
			token = strtok(line, " ");
			pid = strtol(token, NULL, 10);
			token = strtok(NULL, " ");
			vpn = strtol(token, NULL, 10);
			// find
			if ((ppn = find_pte(pt, pid, vpn)) != -1) {
				find_pfn(pf, ppn);
				tot_mem_ref++;
				if (advance_to_next_available_line(proc_te)) {
					// terminate
					//~ printf("terminate %lu\n", pid);
					proc_te = delete_proc_te(proc_t, pid);
					fclose(proc_te->fp);
					delete_ptes(pt, pid);
					PPN *previous_ppn = NULL;
					PPN *current_ppn = proc_te->ppn_head;
					while (current_ppn) {
						delete_pfn(pf, current_ppn->ppn);
						previous_ppn = current_ppn;
						current_ppn = current_ppn->next;
						free(previous_ppn);
					}
					free(proc_te);
					proc_t->runnable--;
					//~ printf("deleted\n");
					//~ print_proc_t(proc_t);
					//~ print_pt(pt);
					//~ print_pf(pf);
					//~ exit(1);
					// free mem
				}
				cpu++;
			// page fault
			} else {
				//~ printf("enqueue (%lu, %lu)\n", pid, vpn);
				enqueue(dq, pid, vpn);
				proc_te->runnable = 0;
				proc_t->runnable--;
				fseek(proc_te->fp, -size, SEEK_CUR);
				tot_page_in++;
			}
		} else {
			// all blocked
			//~ printf("all blocked\n");
		}
		if (proc_t->head == NULL) {
			break;
		}
		run_proc += proc_t->runnable;
		mem_util += pf->size;
		//~ printf("\n");
		//~ sleep(1);
	}
	// statistics
	printf("AMU: %f\n", mem_util / (float)real / (float)page_frame_num);
	printf("ARP: %f\n", run_proc / (float)real);
	printf("TMR: %ld\n", tot_mem_ref);
	printf("TPI: %ld\n", tot_page_in);
	printf("Running Time: %ld\ncpu: %ld\n", real, cpu);
}
