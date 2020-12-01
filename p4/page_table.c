/*
 * Page table module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */
 
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <search.h>

#include "page_table.h"

/*
 * Create page table entry
 * 
 * pid: the pid
 * vpn: the vpn
 * ppn: the ppn used by the process
 * 
 * return: the page table entry created
 */
PTE *create_pte(unsigned long pid, unsigned long vpn, long ppn) {
	PTE *pte = malloc(sizeof(PTE));
	pte->pid = pid;
	pte->vpn = vpn;
	pte->ppn = ppn;
	return pte;
}

/*
 * Create page table
 */
PT *create_pt() {
	PT *pt = malloc(sizeof(PT));
	return pt;
}

/*
 * Compare two page table entries
 * 
 * a: the page table entry
 * b: the page table entry
 * 
 * return: positive if the first pte is larger, negative if the first pte is
 * smaller, 0 if equal
 */
int compare_pte(const void *a, const void *b) {
	PTE *pte_1 = (PTE *)a;
	PTE *pte_2 = (PTE *)b;
	long result = pte_1->pid - pte_2->pid;
	if (result == 0) {
		result = pte_1->vpn - pte_2->vpn;
	}
	return result;
}

/*
 * Compare two page table entries only by pid
 * 
 * a: the page table entry
 * b: the page table entry
 * 
 * return: positive if the first pte is larger, negative if the first pte is
 * smaller, 0 if equal
 */
int compare_pte_pid(const void *a, const void *b) {
	PTE *pte_1 = (PTE *)a;
	PTE *pte_2 = (PTE *)b;
	return pte_1->pid - pte_2->pid;
}

/*
 * Add a page table entry
 * 
 * pt: the page table
 * pid: the pid
 * vpn: the vpn
 * ppn: the ppn used by the process
 */
void add_pte(PT *pt, unsigned long pid, unsigned long vpn, long ppn) {
	PTE *pte = create_pte(pid, vpn, ppn);
	tsearch((void *)pte, &(pt->root), &compare_pte);
}

/*
 * Delete a page table entry
 * 
 * pt: the page table
 * pid: the pid
 * vpn: the vpn
 */
void delete_pte(PT *pt, unsigned long pid, unsigned long vpn) {
	PTE *key = create_pte(pid, vpn, 0);
	tdelete((void *)key, &(pt->root), &compare_pte);
	free(key);
}

/*
 * Delete page table entries of a process
 * 
 * pt: the page table
 * pid: the pid
 */
void delete_ptes(PT *pt, unsigned long pid) {
	PTE *key = create_pte(pid, 0, 0);
	while (tdelete((void *)key, &(pt->root), &compare_pte_pid));
	free(key);
}

/*
 * Find a page table entry
 * 
 * pt: the page table
 * pid: the pid
 * vpn: the vpn
 * 
 * return: the ppn used by the process
 */
long find_pte(PT *pt, unsigned long pid, unsigned long vpn) {
	PTE *key = create_pte(pid, vpn, 0);
	long ppn = -1;
	void * result = tfind((void *)key, &(pt->root), &compare_pte);
	if(result) {
		ppn = (*(PTE **)result)->ppn;
	}
	free(key);
	return ppn;
}

/*
 * Print page table entry
 * 
 * nodep: the pointer to the node in binary tree
 * which: the order which the node is visited
 * depth: the depth of the node
 */
void print_pte(const void *nodep, const VISIT which, const int depth) {
    PTE *datap = *(PTE **) nodep;
    switch (which) {
		case preorder:
			break;
		case postorder:
			fprintf(stderr, "\t(%lu, %lu) -> %ld\n", datap->pid, datap->vpn,
				datap->ppn);
			break;
		case endorder:
			break;
		case leaf:
			fprintf(stderr, "\t(%lu, %lu) -> %ld\n", datap->pid, datap->vpn,
				datap->ppn);
			break;
		default:
			fprintf(stderr, "%d\n", depth);
			break;
    }
}

/*
 * Print page table
 * 
 * pt: the page table
 */
void print_pt(PT *pt) {
	fprintf(stderr, "\t==========page table==========\n");
	if (pt->root) {
		twalk(pt->root, &print_pte);
	}
	fprintf(stderr, "\t==========page table end=======\n");
}
