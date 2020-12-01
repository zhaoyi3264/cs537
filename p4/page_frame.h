/*
 * Page frame module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */

#ifndef PAGE_FRAME_H
#define PAGE_FRAME_H

typedef struct FPFN {
	long ppn;
	struct FPFN *next;
} FPFN;

typedef struct PFN {
	long ppn;
	unsigned long pid;
	unsigned long vpn;
	struct PFN *prev;
	struct PFN *next;
	int reference;
} PFN;

typedef struct PF {
	PFN *head;
	PFN *tail;
	FPFN *free_head;
	FPFN *free_tail;
	long capacity;
	long count;
	long size;
	void *root;
	PFN *hand;
} PF;

typedef struct IPTE {
	long ppn;
	PFN *pfn;
} IPTE;

IPTE *create_ipte(long ppn, PFN* pfn);

int compare_ipte(const void *a, const void *b);

PFN *create_pfn(long ppn, unsigned long pid, unsigned long vpn);

PF *create_pf(long capacity);

void add_fpfn(PF *pf, long ppn);

void delete_pfn_helper(PF *pf, PFN *pfn);

long delete_fpfn(PF *pf);

void add_pfn_helper(PF *pf, PFN *pfn);

long add_pfn(PF *pf, unsigned long pid, unsigned long vpn);

void delete_pfn(PF *pf, long ppn);

void print_pf(PF *pf);

#endif
