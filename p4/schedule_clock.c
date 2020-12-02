/*
 * Page replacement scheduling algorithm module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */

#include <search.h>
#include <stdlib.h>

#include "schedule_algo.h"

/*
 * Find a pfn
 * 
 * pf: the page frame
 * ppn: the ppn
 */
void find_pfn(PF *pf, long ppn) {
	IPTE *key = create_ipte(ppn, NULL);
	void * result = tfind((void *)key, &(pf->root), &compare_ipte);
	if (result) {
		(*(IPTE **)result)->pfn->reference = 1;
	}
	free(key);
}

/*
 * Replace a pfn
 * 
 * pf: the page frame
 * pid: the pid
 * vpn: the vpn
 * 
 * return: the replaced pfn
 */
PFN *swipe(PF *pf) {
	// make a circular linked list
	pf->tail->next = pf->head;
	PFN *pfn = pf->hand;
	// find the pfn with reference bit 0
	while (pfn->reference) {
		pfn->reference = 0;
		pfn = pfn->next;
	}
	// undo the circular linked list
	pf->tail->next = NULL;
	// set the hand
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
	PFN *replaced = create_pfn(position->ppn, position->pid, position->vpn);
	// change the pid and vpn of the replaced pfn
	position->pid = pid;
	position->vpn = vpn;
	return replaced;
}
