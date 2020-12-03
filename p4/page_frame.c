/*
 * Page frame module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */

#define _GNU_SOURCE
#include <search.h>
#include <stdio.h>
#include <stdlib.h>

#include "page_frame.h"

/*
 * Create inverse page table entry
 * 
 * ppn: the ppn
 * pfn: the page frame node
 * 
 * return: the inverse page table entry created
 */
IPTE *create_ipte(long ppn, PFN* pfn) {
	IPTE* ipte = malloc(sizeof(IPTE));
	if (ipte == NULL) {
		exit(1);
	}
	ipte->ppn = ppn;
	ipte->pfn = pfn;
	return ipte;
}

/*
 * Compare two inverse page table entries
 * 
 * a: the inverse page table entry
 * b: the inverse page table entry
 * 
 * return: positive if the first ipte is larger, negative if the first ipte is
 * smaller, 0 if equal
 */
int compare_ipte(const void *a, const void *b) {
	IPTE *ipte_1 = (IPTE *)a;
	IPTE *ipte_2 = (IPTE *)b;
	return ipte_1->ppn - ipte_2->ppn;
}

/*
 * Print inverse page table entry
 * 
 * nodep: the pointer to the node in binary tree
 * which: the order which the node is visited
 * depth: the depth of the node
 */
void print_ipte(const void *nodep, const VISIT which, const int depth) {
    IPTE *datap = *(IPTE **) nodep;
    switch (which) {
		case preorder:
			break;
		case postorder:
			fprintf(stderr, "\t%ld -> (%lu, %lu)\n", datap->ppn, datap->pfn->pid,
				datap->pfn->vpn);
			break;
		case endorder:
			break;
		case leaf:
			fprintf(stderr, "\t%ld -> (%lu, %lu)\n", datap->ppn, datap->pfn->pid,
				datap->pfn->vpn);
			break;
		default:
			fprintf(stderr, "%d\n", depth);
			break;
    }
}

/*
 * Create free page frame node
 * 
 * ppn: the ppn
 * 
 * return: the fpfn created
 */
FPFN *create_fpfn(long ppn) {
	FPFN *fpfn = malloc(sizeof(FPFN));
	if (fpfn == NULL) {
		exit(1);
	}
	fpfn->ppn = ppn;
	fpfn->next = NULL;
	return fpfn;
}

/*
 * Create page frame node
 * 
 * ppn: the ppd
 * pid: the pid
 * vpn: the vpn
 * 
 * return: the pfn created
 */
PFN *create_pfn(long ppn, unsigned long pid, unsigned long vpn) {
	PFN *pfn = malloc(sizeof(PFN));
	if (pfn == NULL) {
		exit(1);
	}
	pfn->ppn= ppn;
	pfn->pid = pid;
	pfn->vpn = vpn;
	pfn->prev = NULL;
	pfn->next = NULL;
	pfn->reference = 0;
	pfn->data = NULL;
	return pfn;
}

/*
 * Create page frame
 * 
 * capacity: the capacity
 * 
 * return the page frame created
 */
PF *create_pf(long capacity) {
	PF *pf = malloc(sizeof(PF));
	if (pf == NULL) {
		exit(1);
	}
	pf->head = NULL;
	pf->tail = NULL;
	pf->free_head = NULL;
	pf->free_tail = NULL;
	pf->capacity = capacity;
	pf->count = 0;
	pf->size = 0;
	pf->root = NULL;
	pf->hand = NULL;
	return pf;
}

/*
 * Add a fpfn to page frame
 * 
 * pf: the page frame
 * ppn: the ppn
 */
void add_fpfn(PF *pf, long ppn) {
	FPFN *fpfn = create_fpfn(ppn);
	if (pf->free_head) {
		pf->free_tail->next = fpfn;
	} else {
		pf->free_head = fpfn;
	}
	pf->free_tail = fpfn;
}

/*
 * Delete a fpfn from page frame
 * 
 * pf: the page frame
 * 
 * return: the ppn of the fpfn deleted
 */
long delete_fpfn(PF *pf) {
	FPFN *fpfn = pf->free_head;
	pf->free_head = pf->free_head->next;
	long ppn = fpfn->ppn;
	free(fpfn);
	return ppn;
}

/*
 * Helper method for adding a pfn
 * 
 * pf: the page frame
 * pfn: the pfn
 */
void add_pfn_helper(PF *pf, PFN *pfn) {
	// add into a doubly linked list
	// set the hand if necessary
	if (pf->head) {
		pf->tail->next = pfn;
		pfn->prev = pf->tail;
	} else {
		pf->head = pfn;
		pf->hand = pfn;
	}
	pf->tail = pfn;
	pf->size++;
}

/*
 * Add a pfn to page frame
 * 
 * pf: the page frame
 * pid: the pid
 * vpn: the vpn
 * 
 * return: the ppn used by the (pid, vpn), -1 if the page frame is full
 */
long add_pfn(PF *pf, unsigned long pid, unsigned long vpn) {
	int add = 0;
	long ppn = -1;
	if (pf->free_head) {
		// free ppn
		add = 1;
		ppn = delete_fpfn(pf);
	} else if (pf->count < pf->capacity) {
		// empty ppn at first
		add = 1;
		ppn = pf->count++;
	}
	if (add) {
		// pf
		PFN *pfn = create_pfn(ppn, pid, vpn);
		add_pfn_helper(pf, pfn);
		// ipt
		IPTE *ipte = create_ipte(ppn, pfn);
		tsearch((void *)ipte, &(pf->root), &compare_ipte);
		return ppn;
	} else{
		return -1;
	}
}

/*
 * Helper method for deleting a pfn
 * 
 * pf: the page frame
 * pfn: the page frame node
 */
void delete_pfn_helper(PF *pf, PFN *pfn) {
	// delete from a doubly linked list
	// move the hand if necessary
	if (pfn->prev == NULL && pfn->next == NULL) {
		pf->head = NULL;
		pf->tail = NULL;
		pf->hand = NULL;
	} else if (pfn->prev == NULL) {
		if (pf->hand == pfn) {
			pf->hand = pf->head->next;
		}
		pf->head = pfn->next;
		pf->head->prev = NULL;
	} else if (pfn->next == NULL) {
		if (pf->hand == pfn) {
			pf->hand = pf->head;
		}
		pf->tail = pfn->prev;
		pf->tail->next = NULL;
	} else {
		if (pf->hand == pfn) {
			pf->hand = pfn->next;
		}
		pfn->prev->next = pfn->next;
		pfn->next->prev = pfn->prev;
	}
	pfn->prev = NULL;
	pfn->next = NULL;
	pf->size--;
}

/*
 * Delete a pfn from page frame
 * 
 * pf: the page frame
 * ppn: the ppn
 */
void delete_pfn(PF *pf, long ppn) {
	IPTE *key = create_ipte(ppn, NULL);
	void *result = tfind((void *)key, &(pf->root), &compare_ipte);
	if (result) {
		IPTE *ipte = *(IPTE **)result;
		// pf
		PFN *pfn = ipte->pfn;
		delete_pfn_helper(pf, pfn);
		add_fpfn(pf, ppn);
		//ipt
		tdelete((void *)key, &(pf->root), &compare_ipte);
	}
	free(key);
}

/*
 * Print page frame and inverse page table
 * 
 * pf: page frame
 */
void print_pf(PF *pf) {
	//pf
	fprintf(stderr, "\t==========page frame==========\n");
	PFN *pfn = pf->head;
	while (pfn) {
		fprintf(stderr, "\t%ld -> (%lu, %lu) ref: %d\n", pfn->ppn, pfn->pid, pfn->vpn, pfn->reference);
		pfn = pfn->next;
	}
	fprintf(stderr, "\tsize: %ld\n", pf->size);
	if (pf->hand) {
		fprintf(stderr, "\thand: %ld\n", pf->hand->ppn);
	}
	fprintf(stderr, "\tfree ppn: ");
	FPFN *fpfn = pf->free_head;
	while (fpfn) {
		fprintf(stderr, "%ld", fpfn->ppn);
		fpfn = fpfn->next;
		if (fpfn) {
			fprintf(stderr, "->");
		}
	}
	fprintf(stderr, "\n");
	fprintf(stderr, "\t==========page frame end==========\n");
	// ipt
	fprintf(stderr, "\t==========invert page table==========\n");
	if (pf->root) {
		twalk(pf->root, &print_ipte);
	}
	fprintf(stderr, "\t==========invert page table end=======\n");
}
