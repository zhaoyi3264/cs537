#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "process_table.h"
#include "page_table.h"
#include "disk_queue.h"
#include "page_frame.h"
#include "schedule_algo.h"

int rand10() {
	return rand() % 10;
}

void process_table() {
	ProcT *proc_t = create_proc_t();
	for (int i = 0; i < 100; i++) {
		update_proc_te_trace(proc_t, i % 10, i);
	}
	for (int i = 0; i < 20; i += 2) {
		delete_proc_te(proc_t, i);
	}
	print_proc_t(proc_t);
}

void page_table() {
	PT* pt = create_pt();
	int count = 0;
	for (int i = 0; i < 25; i++) {
		add_pte(pt, i, i, count++);
	}
	for (int i = 0; i < 10; i++) {
		add_pte(pt, 100, i, count++);
	}
	print_pt(pt);
	for (int i = 0; i < 30; i+=2) {
		delete_pte(pt, i, i);
	}
	print_pt(pt);
	delete_ptes(pt, 100);
	print_pt(pt);
	clear_pt(pt);
	free(pt);
}

void disk_queue() {
	DiskQueue *dq = create_dq(5);
	for (int i = 0; i < 10; i++) {
		enqueue(dq, rand10() % 10, rand10());
	}
	print_dq(dq);
	for (int i = 0; i < 60; i++) {
		Node *node = advance(dq);
		if (node) {
			printf("%d: vpn: %ld vpn: %ld\n", i, node->pid, node->pid);
			free(node);
		}
	}
	free(dq);
}

void page_frame_1() {
	PF *pf = create_pf(5);
	IPT *ipt = create_ipt();
	for(int i = 0; i < 10; i++) {
		printf("%ld\n", add_pfn(pf, ipt, i, i));
	}
	delete_pfn(pf, ipt, 0);
	delete_pfn(pf, ipt, 2);
	delete_pfn(pf, ipt, 4);
	printf("%ld\n", add_pfn(pf, ipt, 100, 100));
	printf("%ld\n", add_pfn(pf, ipt, 200, 200));
	printf("%ld\n", add_pfn(pf, ipt, 400, 400));
	printf("%ld\n", add_pfn(pf, ipt, 500, 500));
	print_pf(pf);
	print_ipt(ipt);
	free(ipt);
	free(pf);
}

void page_frame_2() {
	PF *pf = create_pf(5);
	IPT *ipt = create_ipt();
	for(int i = 0; i < 4; i++) {
		printf("%ld\n", add_pfn(pf, ipt, 0, 0));
	}
	printf("%ld\n", add_pfn(pf, ipt, 1, 1));
	delete_pfns(pf, ipt, 0);
	print_pf(pf);
	print_ipt(ipt);
	for(int i = 1; i < 10; i++) {
		printf("%ld\n", add_pfn(pf, ipt, i * 100, i * 100));
	}
	print_pf(pf);
	print_ipt(ipt);
	free(ipt);
	free(pf);
}

void fifo() {
	PF *pf = create_pf(5);
	IPT *ipt = create_ipt();
	for(int i = 0; i < 9; i++) {
		if (add_pfn(pf, ipt, i, i) == -1) {
			replace_pfn(pf, ipt, i, i);
		}
	}
	for (int i = 0; i < 3; i++) {
		replace_pfn(pf, ipt, 20, 20);
	}
	delete_pfns(pf, ipt, 20);
	print_pf(pf);
	print_ipt(ipt);
	free(ipt);
	free(pf);
}

void fifo_page_table() {
	long size = 7;
	int length = 100;
	PT* pt = create_pt();
	PF *pf = create_pf(size);
	IPT *ipt = create_ipt();
	
	long pid = 0;
	long vpn = 0;
	long ppn = -1;
	PFN *replaced = NULL;
	int arr[length];
	int replace_count = 0;
	int replace[length];
	for (int i = 0; i < length; i++) {
		pid = vpn = arr[i] = rand() % 100;
		if ((ppn = find_pte(pt, pid, vpn)) != -1) {
			find_pfn(ipt, ppn);
			continue;
		}
		ppn = add_pfn(pf, ipt, pid, vpn);
		if (ppn == -1) {
			replaced = replace_pfn(pf, ipt, pid, vpn);
			ppn = replaced->ppn;
			delete_pte(pt, replaced->pid, replaced->vpn);
			replace[replace_count++] = replaced->pid;
		}
		add_pte(pt, pid, vpn, ppn);
	}
	for (int i = 0; i < length; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
	for (int i = 0; i < replace_count; i++) {
		printf("%d ", replace[i]);
	}
	printf("\n");
	print_pt(pt);
	print_pf(pf);
	print_ipt(ipt);
	free(ipt);
	free(pf);
	free(pt);
}

int main() {
	time_t t;
	srand((unsigned) time(&t));
	
	//~ process_table();
	//~ page_table();
	//~ disk_queue();
	//~ page_frame_1();
	//~ page_frame_2();
	//~ fifo();
	fifo_page_table();
	
}
