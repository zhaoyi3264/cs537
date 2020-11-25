#ifndef SCHEDULE_ALGO_H
#define SCHEDULE_ALGO_H

#include "page_frame.h"

void find_pfn(PF *pf, long ppn);

PFN *replace_pfn(PF *pf, long pid, long vpn);

#endif
