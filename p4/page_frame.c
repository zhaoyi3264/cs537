#define _GNU_SOURCE
#include <search.h>
#include <stdio.h>
#include <stdlib.h>

#include "page_frame.h"

IPTE *create_ipte(long ppn, PFN* pfn) {
	IPTE* ipte = malloc(sizeof(IPTE));
	ipte->ppn = ppn;
	ipte->pfn = pfn;
	return ipte;
}

int compare_ipte(const void *a, const void *b) {
	IPTE *ipte_1 = (IPTE *)a;
	IPTE *ipte_2 = (IPTE *)b;
	return ipte_1->ppn - ipte_2->ppn;
}

void print_ipte(const void *nodep, const VISIT which, const int depth) {
    IPTE *datap = *(IPTE **) nodep;
    switch (which) {
		case preorder:
			break;
		case postorder:
			fprintf(stderr, "\t%ld -> (%lu, %lu)\n", datap->ppn, datap->pfn->pid, datap->pfn->vpn);
			break;
		case endorder:
			break;
		case leaf:
			fprintf(stderr, "\t%ld -> (%lu, %lu)\n", datap->ppn, datap->pfn->pid, datap->pfn->vpn);
			break;
		default:
			fprintf(stderr, "%d\n", depth);
			break;
    }
}

FPFN *create_fpfn(long ppn) {
	FPFN *fpfn = malloc(sizeof(FPFN));
	fpfn->ppn = ppn;
	fpfn->next = NULL;
	return fpfn;
}

PFN *create_pfn(long ppn, unsigned long pid, unsigned long vpn) {
	PFN *pfn = malloc(sizeof(PFN));
	pfn->ppn= ppn;
	pfn->pid = pid;
	pfn->vpn = vpn;
	pfn->prev = NULL;
	pfn->next = NULL;
	pfn->reference = 0;
	pfn->data = NULL;
	return pfn;
}

PF *create_pf(long capacity) {
	PF *pf = malloc(sizeof(PF));
	pf->head = NULL;
	pf->tail = NULL;
	pf->free_head = NULL;
	pf->free_tail = NULL;
	pf->capacity = capacity;
	pf->count = 0;
	pf->size = 0;
	pf->root = NULL;
	return pf;
}

void add_fpfn(PF *pf, long ppn) {
	FPFN *fpfn = create_fpfn(ppn);
	if (pf->free_head) {
		pf->free_tail->next = fpfn;
	} else {
		pf->free_head = fpfn;
	}
	pf->free_tail = fpfn;
}

long delete_fpfn(PF *pf) {
	FPFN *fpfn = pf->free_head;
	pf->free_head = pf->free_head->next;
	long ppn = fpfn->ppn;
	free(fpfn);
	return ppn;
}

void add_pfn_helper(PF *pf, PFN *pfn) {
	if (pf->head) {
		pf->tail->next = pfn;
		pfn->prev = pf->tail;
	} else {
		pf->head = pfn;
	}
	pf->tail = pfn;
	pf->size++;
}

long add_pfn(PF *pf, unsigned long pid, unsigned long vpn) {
	int add = 0;
	long ppn = -1;
	if (pf->free_head) {
		add = 1;
		ppn = delete_fpfn(pf);
	} else if (pf->count < pf->capacity) {
		add = 1;
		ppn = pf->count++;
	}
	if (add) {
		PFN *pfn = create_pfn(ppn, pid, vpn);
		add_pfn_helper(pf, pfn);
		IPTE *ipte = create_ipte(ppn, pfn);
		tsearch((void *)ipte, &(pf->root), &compare_ipte);
		return ppn;
	}
	pf->size--;
	return -1;
}

void delete_pfn_helper(PF *pf, PFN *pfn) {
	if (pfn->prev == NULL && pfn->next == NULL) {
		pf->head = NULL;
		pf->tail = NULL;
	} else if (pfn->prev == NULL) {
		pf->head = pfn->next;
		pf->head->prev = NULL;
	} else if (pfn->next == NULL) {
		pf->tail = pfn->prev;
		pf->tail->next = NULL;
	} else {
		pfn->prev->next = pfn->next;
		pfn->next->prev = pfn->prev;
	}
	pfn->prev = NULL;
	pfn->next = NULL;
	pf->size--;
}

int delete_pfn(PF *pf, long ppn) {
	IPTE *ipte = create_ipte(ppn, NULL);
	IPTE *result = *(IPTE **)tfind((void *)ipte, &(pf->root), &compare_ipte);
	if (result) {
		// pf
		PFN *pfn = result->pfn;
		delete_pfn_helper(pf, pfn);
		add_fpfn(pf, ppn);
		//ipt
		tdelete((void *)ipte, &(pf->root), &compare_ipte);
		return 1;
	} else {
		return 0;
	}
}

void delete_pfns(PF *pf, unsigned long pid) {
	PFN *pfn = pf->head;
	PFN *next = NULL;
	while (pfn) {
		if (pfn->pid == pid) {
			next = pfn->next;
			delete_pfn_helper(pf, pfn);
			IPTE *key = create_ipte(pfn->ppn, NULL);
			tdelete(key, &(pf->root), &compare_ipte);
			free(key);
			add_fpfn(pf, pfn->ppn);
			pfn = next;
		} else {
			pfn = pfn->next;
		}
	}
}

void print_pf(PF *pf) {
	fprintf(stderr, "\t==========page frame==========\n");
	PFN *pfn = pf->head;
	while (pfn) {
		fprintf(stderr, "\t%ld -> (%lu, %lu) ref: %d\n", pfn->ppn, pfn->pid, pfn->vpn, pfn->reference);
		pfn = pfn->next;
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
	fprintf(stderr, "\t==========invert page table==========\n");
	if (pf->root) {
		twalk(pf->root, &print_ipte);
	}
	fprintf(stderr, "\t==========invert page table end=======\n");
}
