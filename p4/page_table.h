#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

typedef struct PTE {
	long pid;
	long vpn;
	long ppn;
} PTE;

typedef struct PT {
	void *root;
} PT;

PTE *create_pte(long pid, long vpn, long ppn);

PT *create_pt();

void add_pte(PT *pt, long pid, long vpn, long ppn);

void delete_pte(PT *pt, long pid, long vpn);

void delete_ptes(PT *pt, long pid);

long find_pte(PT *pt, long pid, long vpn);

void clear_pt(PT *pt);

void print_pt(PT *pt);

#endif
