#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <search.h>

#include "page_table.h"

PTE *create_pte(long pid, long vpn, long ppn) {
	PTE *pte = malloc(sizeof(PTE));
	pte->pid = pid;
	pte->vpn = vpn;
	pte->ppn = ppn;
	return pte;
}

PT *create_pt() {
	PT *pt = malloc(sizeof(PT));
	return pt;
}

int compare_pte(const void *a, const void *b) {
	PTE *pte_1 = (PTE *)a;
	PTE *pte_2 = (PTE *)b;
	long result = pte_1->pid - pte_2->pid;
	if (result == 0) {
		result = pte_1->vpn - pte_2->vpn;
	}
	return result;
}

int compare_pte_pid(const void *a, const void *b) {
	PTE *pte_1 = (PTE *)a;
	PTE *pte_2 = (PTE *)b;
	return pte_1->pid - pte_2->pid;
}

void add_pte(PT *pt, long pid, long vpn, long ppn) {
	PTE *pte = create_pte(pid, vpn, ppn);
	tsearch((void *)pte, &(pt->root), &compare_pte);
}

void delete_pte(PT *pt, long pid, long vpn) {
	PTE *key = create_pte(pid, vpn, 0);
	tdelete((void *)key, &(pt->root), &compare_pte);
	free(key);
}

void delete_ptes(PT *pt, long pid) {
	PTE *pte = create_pte(pid, 0, 0);
	while (tdelete((void *)pte, &(pt->root), &compare_pte_pid));
	free(pte);
}

long find_pte(PT *pt, long pid, long vpn) {
	PTE *key = create_pte(pid, vpn, 0);
	long ppn = -1;
	void * result = tfind((void *)key, &(pt->root), &compare_pte);
	if(result) {
		ppn = (*(PTE **)result)->ppn;
	}
	free(key);
	return ppn;
}

void free_pte(void *nodep) {
	PTE *datap = (PTE *) nodep;
	free(datap);
}

void clear_pt(PT *pt) {
	tdestroy(pt->root, &free_pte);
	pt->root = NULL;
}

void print_pte(const void *nodep, const VISIT which, const int depth) {
    PTE *datap = *(PTE **) nodep;
    switch (which) {
		case preorder:
			break;
		case postorder:
			printf("(%ld, %ld) -> %ld\n", datap->pid, datap->vpn, datap->ppn);
			break;
		case endorder:
			break;
		case leaf:
			printf("(%ld, %ld) -> %ld\n", datap->pid, datap->vpn, datap->ppn);
			break;
		default:
			printf("%d\n", depth);
			break;
    }
}

void print_pt(PT *pt) {
	printf("==========page table==========\n");
	if (pt->root) {
		twalk(pt->root, &print_pte);
	}
	printf("==========page table end=======\n");
}
