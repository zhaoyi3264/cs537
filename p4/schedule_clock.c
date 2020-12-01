#include <search.h>
#include <stdio.h>
#include <stdlib.h>

#include "schedule_algo.h"

void find_pfn(PF *pf, long ppn) {
	IPTE *key = create_ipte(ppn, NULL);
	void * result = tfind((void *)key, &(pf->root), &compare_ipte);
	if (result) {
		(*(IPTE **)result)->pfn->reference = 1;
	}
	free(key);
}

PFN *swipe(PF *pf) {
	pf->tail->next = pf->head;
	PFN *pfn = pf->hand;
	while (pfn->reference) {
		pfn->reference = 0;
		pfn = pfn->next;
	}
	pf->tail->next = NULL;
	if (pfn->prev == NULL && pfn->next == NULL) {
		pf->hand = NULL;
	} else if (pfn->prev == NULL) {
		pf->hand = pf->head->next;
	} else if (pfn->next == NULL) {
		pf->hand = pf->head;
	} else {
		pf->hand = pfn->next;
	}
	return pfn;
}

PFN *replace_pfn(PF *pf, unsigned long pid, unsigned long vpn) {
	PFN *position = swipe(pf);
	//~ printf("replace: %ld\n", position->ppn);
	//~ printf("pf size before: %ld\n", pf->size);
	//~ delete_pfn_helper(pf, replaced);
	//~ replaced->next = NULL;
	//~ replaced->prev = NULL;
	//~ IPTE *key = create_ipte(position->ppn, NULL);
	// replace pfn pointed by ipte
	//~ IPTE *ipte = *(IPTE **)tfind(key, &(pf->root), &compare_ipte);
	//~ PFN *new = create_pfn(position->ppn, pid, vpn);
	//~ ipte->pfn = new;
	//~ add_pfn_helper(pf, new);
	//~ printf("pf size after : %ld\n", pf->size);
	PFN *replaced = create_pfn(position->ppn, position->pid, position->vpn);
	position->pid = pid;
	position->vpn = vpn;
	return replaced;
}
