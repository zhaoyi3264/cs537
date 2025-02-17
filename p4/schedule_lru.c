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
		PFN *pfn = (*(IPTE **)result)->pfn;
		// move the pfn to the tail
		if (pfn != pf->tail) {
			delete_pfn_helper(pf, pfn);
			add_pfn_helper(pf, pfn);
		}
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
PFN *replace_pfn(PF *pf, unsigned long pid, unsigned long vpn) {
	// remove the head of the page frame
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
