#include <search.h>
#include <stdio.h>
#include <stdlib.h>

#include "schedule_algo.h"

PFN *replace_pfn(PF *pf, IPT *ipt, long pid, long vpn) {
	if (pf->head == NULL) {
		return NULL;
	}
	PFN *replaced = pf->head;
	pf->head = pf->head->next;
	IPTE *key = create_ipte(replaced->ppn, NULL);
	tdelete(key, &(ipt->root), &compare_ipte);
	add_fpfn(pf, replaced->ppn);
	add_pfn(pf, ipt, pid, vpn);
	return replaced;
}
