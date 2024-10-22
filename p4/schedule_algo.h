/*
 * Page replacement scheduling algorithm module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */
 
 
#ifndef SCHEDULE_ALGO_H
#define SCHEDULE_ALGO_H

#include "page_frame.h"

void find_pfn(PF *pf, long ppn);

PFN *replace_pfn(PF *pf, unsigned long pid, unsigned long vpn);

#endif
