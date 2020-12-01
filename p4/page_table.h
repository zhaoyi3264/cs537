/*
 * Page table module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */
 
#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

typedef struct PTE {
	unsigned long pid;
	unsigned long vpn;
	long ppn;
} PTE;

typedef struct PT {
	void *root;
} PT;

PTE *create_pte(unsigned long pid, unsigned long vpn, long ppn);

PT *create_pt();

void add_pte(PT *pt, unsigned long pid, unsigned long vpn, long ppn);

void delete_pte(PT *pt, unsigned long pid, unsigned long vpn);

void delete_ptes(PT *pt, unsigned long pid);

long find_pte(PT *pt, unsigned long pid, unsigned long vpn);

void clear_pt(PT *pt);

void print_pt(PT *pt);

#endif
