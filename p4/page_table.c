#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <search.h>

#include "page_table.h"

PageTE *create_page_te(char *pid, char *vpn, int ppn) {
	PageTE *page_te = malloc(sizeof(PageTE));
	page_te->pid = malloc(strlen(pid) + 1);
	sprintf(page_te->pid, "%s", pid);
	page_te->vpn = malloc(strlen(vpn) + 1);
	sprintf(page_te->vpn, "%s", vpn);
	page_te->ppn = ppn;
	return page_te;
}

PageT *create_page_t() {
	PageT *page_t = malloc(sizeof(PageT));
	return page_t;
}

int compare_page_te(const void *page_te_1, const void *page_te_2) {
	PageTE *a = (PageTE *)page_te_1;
	PageTE *b = (PageTE *)page_te_2;
	int result = strcmp(a->pid, b->pid);
	if (result) {
		return result;
	} else {
		return strcmp(a->vpn, b->vpn);
	}
}

PageTE *add_page_te(PageT *page_t, char *pid, char *vpn, int ppn) {
	PageTE *page_te = create_page_te(pid, vpn, ppn);
	return tsearch((void *)page_te, &(page_t->root), &compare_page_te);
}

PageTE *delete_page_te(PageT *page_t, char *pid, char *vpn) {
	PageTE *page_te = create_page_te(pid, vpn, 0);
	return tdelete((void *)page_te, &(page_t->root), &compare_page_te);
}

void clear_page_t(PageT *page_t) {
	tdestroy(page_t->root, &free);
	page_t->root = NULL;
}

void print_page_te(const void *nodep, const VISIT which, const int depth) {
    PageTE *datap = *(PageTE **) nodep;
    switch (which) {
		case preorder:
			break;
		case postorder:
			printf("(%s, %s) -> %d\n", datap->pid, datap->vpn, datap->ppn);
			break;
		case endorder:
			break;
		case leaf:
			printf("(%s, %s) -> %d\n", datap->pid, datap->vpn, datap->ppn);
			break;
		default:
			printf("%d\n", depth);
			break;
    }
}

void print_page_t(PageT *page_t) {
	printf("==========page table==========\n");
	if (page_t->root) {
		twalk(page_t->root, &print_page_te);
	}
}
