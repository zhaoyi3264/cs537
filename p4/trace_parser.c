#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "trace_parser.h"

int is_number(char * str) {
	char c = 0;
	for (int i = 0; (c = str[i]); i++) {
		if (isdigit(c) == 0) {
			return 0;
		}
	}
	return 1;
}

void validate_pid_addr(char *str, char *pid_addr, char *line, int line_num) {
	if (str == NULL) {
		fprintf(stderr, "error: %d: invalid trace %s\n", line_num, line);
		exit(1);
	} else if (is_number(str) == 0) {
		fprintf(stderr, "error: %d: invalid %s %s\n", line_num, pid_addr, str);
		exit(1);
	}
}

PTable *parse_trace(char *fname) {
	if (fname == NULL) {
		fprintf(stderr, "error: no trace file specified\n");
		exit(1);
	}
	
	FILE *fp = fopen(fname, "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t size;
    int line_num = 0;
    
    PTable *pt = create_ptable();
    
    char *buf = NULL;
    char *pid = NULL;
    char *addr = NULL;

    while ((size = getline(&line, &len, fp)) != -1) {
		line_num++;
        line[size - 1] = '\0';
        buf = malloc(strlen(line) + 1);
        sprintf(buf, "%s", line);
        pid = strtok(buf, " ");
        validate_pid_addr(pid, "pid", line, line_num);
        addr = strtok(NULL, " ");
        validate_pid_addr(addr, "address", line, line_num);
        //~ printf("%.3d: pid: %s\taddr: %s\n", line_num, pid, addr);
        update_pte_trace(pt, pid, line_num);
        addr = strtok(NULL, " ");
        if (addr) {
			fprintf(stderr, "error: %d: invalid trace %s\n", line_num, line);
			exit(1);
		}
    }
	fclose(fp);
	return pt;
}
