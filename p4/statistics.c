#include <stdlib.h>
#include <stdio.h>

#include "statistics.h"

Stat *create_stat() {
	Stat *stat = malloc(sizeof(Stat));
	stat->real = 0;
	stat->mem_util = 0;
	stat->run_proc = 0;
	stat->tot_mem_ref = 0;
	stat->tot_page_in = 0;
	return stat;
}

void tick(Stat *stat, long real, long long mem_util, long run_proc) {
	stat->real += real;
	stat->mem_util += mem_util;
	stat->run_proc += run_proc;
}

void mem_ref(Stat *stat) {
	stat->tot_mem_ref++;
}

void page_in(Stat *stat) {
	stat->tot_page_in++;
}

void print_stat(Stat *stat, long page_frame_num) {
	fprintf(stderr, "==========stat==========\n");
	fprintf(stderr, "AMU:\t%f\n", stat->mem_util / (float)(stat->real) /
		(float)page_frame_num);
	fprintf(stderr, "ARP:\t%f\n", stat->run_proc / (float)(stat->real));
	fprintf(stderr, "TMR:\t%ld\n", stat->tot_mem_ref);
	fprintf(stderr, "TPI:\t%ld\n", stat->tot_page_in);
	fprintf(stderr, "R Time: %ld\n", stat->real);
}
