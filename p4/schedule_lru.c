#include <search.h>
#include <stdio.h>
#include <stdlib.h>

#include "schedule_algo.h"

void find_pfn(PF *pf, long ppn) {
	IPTE *key = create_ipte(ppn, NULL);
	void * result = tfind((void *)key, &(pf->root), &compare_ipte);
	if (result) {
		PFN *pfn = (*(IPTE **)result)->pfn;
		if (pfn != pf->tail) {
			delete_pfn_helper(pf, pfn);
			add_pfn_helper(pf, pfn);
		}
	}
	free(key);
}

PFN *replace_pfn(PF *pf, long pid, long vpn) {
	if (pf->head == NULL) {
		return NULL;
	}
	PFN *replaced = pf->head;
	pf->head = pf->head->next;
	pf->head->prev = NULL;
	IPTE *key = create_ipte(replaced->ppn, NULL);
	// replace pfn pointed by ipte
	IPTE *ipte = *(IPTE **)tfind(key, &(pf->root), &compare_ipte);
	PFN *new = create_pfn(replaced->ppn, pid, vpn);
	ipte->pfn = new;
	add_pfn_helper(pf, new);
	return replaced;
}
