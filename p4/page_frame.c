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

IPT *create_ipt() {
	IPT *ipt = malloc(sizeof(IPT));
	ipt->root = NULL;
	return ipt;
}

int compare_ipte(const void *a, const void *b) {
	IPTE *ipte_1 = (IPTE *)a;
	IPTE *ipte_2 = (IPTE *)b;
	return ipte_1->ppn - ipte_2->ppn;
}

void free_ipte(void *nodep) {
	IPTE *datap = (IPTE *) nodep;
	free(datap);
}

void clear_ipt(IPT *ipt) {
	tdestroy(ipt->root, &free_ipte);
	ipt->root = NULL;
}

void print_ipte(const void *nodep, const VISIT which, const int depth) {
    IPTE *datap = *(IPTE **) nodep;
    switch (which) {
		case preorder:
			break;
		case postorder:
			printf("%ld -> (%ld, %ld)\n", datap->ppn, datap->pfn->pid, datap->pfn->pid);
			break;
		case endorder:
			break;
		case leaf:
			printf("%ld -> (%ld, %ld)\n", datap->ppn, datap->pfn->pid, datap->pfn->pid);
			break;
		default:
			printf("%d\n", depth);
			break;
    }
}

void print_ipt(IPT *ipt) {
	printf("==========invert page table==========\n");
	if (ipt->root) {
		twalk(ipt->root, &print_ipte);
	}
	printf("==========invert page table end=======\n");
}

FPFN *create_fpfn(long ppn) {
	FPFN *fpfn = malloc(sizeof(FPFN));
	fpfn->ppn = ppn;
	fpfn->next = NULL;
	return fpfn;
}

PFN *create_pfn(long ppn, long pid, long vpn) {
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

long remove_fpfn(PF *pf) {
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
}

long add_pfn(PF *pf, IPT *ipt, long pid, long vpn) {
	int add = 0;
	long ppn = 0;
	if (pf->count < pf->capacity) {
		add = 1;
		ppn = pf->count++;
	} else if (pf->free_head) {
		add = 1;
		ppn = remove_fpfn(pf);
	}
	if (add) {
		PFN *pfn = create_pfn(ppn, pid, vpn);
		add_pfn_helper(pf, pfn);
		IPTE *ipte = create_ipte(ppn, pfn);
		tsearch((void *)ipte, &(ipt->root), &compare_ipte);
		return ppn;
	}
	return -1;
}

void delete_pfn_helper(PF *pf, PFN *pfn) {
	if (pfn->prev) {
		pfn->prev->next = pfn->next;
	} else {
		pf->head = pfn->next;
	}
	if (pfn->next) {
		pfn->next->prev = pfn->prev;
	} else {
		pf->tail = pfn->prev;
	}
}

int delete_pfn(PF *pf, IPT *ipt, long ppn) {
	IPTE *ipte = create_ipte(ppn, NULL);
	// pf
	IPTE *result = *(IPTE **)tfind((void *)ipte, &(ipt->root), &compare_ipte);
	if (result) {
		PFN *pfn = result->pfn;
		delete_pfn_helper(pf, pfn);
		add_fpfn(pf, ppn);
	} else {
		return 0;
	}
	//ipt
	tdelete((void *)ipte, &(ipt->root), &compare_ipte);
	return 1;
}

void delete_pfns(PF *pf, IPT *ipt, long pid) {
	PFN *pfn = pf->head;
	PFN *next = NULL;
	while (pfn) {
		if (pfn->pid == pid) {
			next = pfn->next;
			delete_pfn_helper(pf, pfn);
			IPTE *key = create_ipte(pfn->ppn, create_pfn(0, 0, 0));
			tdelete(key, &(ipt->root), &compare_ipte);
			free(key);
			add_fpfn(pf, pfn->ppn);
			pfn = next;
		} else {
			pfn = pfn->next;
		}
	}
}

void find_pfn(IPT *ipt, long ppn) {
	IPTE *key = create_ipte(ppn, NULL);
	void * result = tfind((void *)key, &(ipt->root), &compare_ipte);
	if(result) {
		(*(IPTE **)result)->pfn->reference = 1;
	}
	free(key);
}

void print_pf(PF *pf) {
	printf("==========page frame==========\n");
	PFN *pfn = pf->head;
	while (pfn) {
		printf("%ld -> (%ld, %ld) ref: %d\n", pfn->ppn, pfn->pid, pfn->vpn, pfn->reference);
		pfn = pfn->next;
	}
	printf("free ppn: ");
	FPFN *fpfn = pf->free_head;
	while (fpfn) {
		printf("%ld", fpfn->ppn);
		fpfn = fpfn->next;
		if (fpfn) {
			printf("->");
		}
	}
	printf("\n");
	printf("==========page frame end==========\n");
}
