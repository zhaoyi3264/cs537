#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

typedef struct PageTE {
	char *pid;
	char *vpn;
	int ppn;
} PageTE;

typedef struct PageT {
	void *root;
} PageT;

PageTE *create_page_te(char *pid, char *vpn, int ppn);

PageT *create_page_t();

int compare_page_te(const void *page_te_1, const void *page_te_2);

PageTE *add_page_te(PageT *page_t, char *pid, char *vpn, int ppn);

PageTE *find_page_te(PageT *page_t, char *pid, char *vpn);

PageTE *delete_page_te(PageT *page_t, char *pid, char *vpn);

void clear_page_t(PageT *page_t);

void print_page_t(PageT *page_t);

#endif
