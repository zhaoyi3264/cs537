#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "trace_parser.h"

int exponent(long int x) {
	int exp = 0;
	while (1) {
		x = x >> 1;
		if (x > 0) {
			exp++;
		} else {
			break;
		}
	}
	return exp;
}

int main(int argc, char * argv[]) {
	int opt = 0;
	long int page_size = 4096;
	int mem_size = 1;
	char *trace_file = NULL;
	
	while ((opt = getopt(argc, argv, "-p:m:")) != -1) {
		switch (opt) {
			case 'p':
				if (is_number(optarg)) {
					page_size = strtol(optarg, NULL, 10);
				} else {
					fprintf(stderr, "error: page size is not a number\n");
					exit(1);					
				}
				break;
			case 'm':
				if (is_number(optarg)) {
					mem_size = atoi(optarg);
				} else {
					fprintf(stderr, "error: memory size is not a number\n");
					exit(1);
				}
				break;
			case 1:
				trace_file = malloc(strlen(optarg) + 1);
				sprintf(trace_file, "%s", optarg);
				break;
			default:
				exit(1);
		}
	}
	
	//~ printf("2^%d = %ld\n", exponent(page_size), page_size);
	int exp = exponent(page_size);
	if ((int)pow(2, exp) != page_size) {
		fprintf(stderr, "error: page size must be a power of two\n");
		exit(1);
	}
	int page_frame_num = pow(2, 20 - exp) * mem_size;
	if (page_frame_num <= 0) {
		fprintf(stderr, "error: page size must be greater than real memory size\n");
		exit(1);
	}
	printf("page size: %ld\n mem size: %d\n  pf size: %d\ntrace    : %s\n",
		page_size, mem_size, page_frame_num, trace_file);
		
	PTable *pt = parse_trace(trace_file);
	print_pt(pt);
}
