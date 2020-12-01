#include <search.h>
#include <stdio.h>
#include <stdlib.h>

#include "schedule_algo.h"

void find_pfn(PF *pf, long ppn) {
	if (pf && ppn) {
		return;
	}
}

PFN *replace_pfn(PF *pf, unsigned long pid, unsigned long vpn) {
	PFN *replaced = pf->head;
	pf->head = pf->head->next;
	pf->head->prev = NULL;
	replaced->next = NULL;
	pf->size--;
	IPTE *key = create_ipte(replaced->ppn, NULL);
	// replace pfn pointed by ipte
	IPTE *ipte = *(IPTE **)tfind(key, &(pf->root), &compare_ipte);
	PFN *new = create_pfn(replaced->ppn, pid, vpn);
	ipte->pfn = new;
	add_pfn_helper(pf, new);
	return replaced;
}
