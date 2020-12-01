#ifndef STATISTICS_H
#define STATISTICS_H

typedef struct Stat {
	long real;
	long long mem_util;
	long run_proc;
	long tot_mem_ref;
	long tot_page_in;
} Stat;

Stat *create_stat();

void tick(Stat *stat, long real, long long mem_util, long run_proc);

void mem_ref(Stat *stat);

void page_in(Stat *stat);

void print_stat(Stat *stat, long page_frame_num);

#endif
