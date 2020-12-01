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

FILE *open_trace(char *fname) {
	FILE *fp = fopen(fname, "r");
	if (fp == NULL) {
		fprintf(stderr, "error: file %s not found\n", fname);
		exit(1);
	}
	return fp;	
}

void validate_pid_vpn(char *str, char *pid_vpn, char *line, unsigned long line_num) {
	if (str == NULL) {
		fprintf(stderr, "error: %lu: invalid trace %s\n", line_num, line);
		exit(1);
	} else if (is_number(str) == 0) {
		fprintf(stderr, "error: %lu: invalid %s %s\n", line_num, pid_vpn, str);
		exit(1);
	}
}

ProcT *parse_trace(char *fname) {
	if (fname == NULL) {
		fprintf(stderr, "error: no trace file specified\n");
		exit(1);
	}
	
	FILE *fp = open_trace(fname);
    char *line = NULL;
    size_t len = 0;
    ssize_t size;
    unsigned long line_num = 0;
    
    ProcT *proc_t = create_proc_t();
    
    char *buf = NULL;
    char *token = NULL;
    unsigned long pid = 0;

    while ((size = getline(&line, &len, fp)) != -1) {
		line_num++;
        line[size - 1] = '\0';
        buf = malloc(strlen(line) + 1);
        sprintf(buf, "%s", line);
        token = strtok(buf, " ");
        validate_pid_vpn(token, "pid", line, line_num);
        pid = strtoul(token, NULL, 10);
        token = strtok(NULL, " ");
        validate_pid_vpn(token, "vpn", line, line_num);
        update_proc_te_trace(proc_t, pid, ftell(fp) - size);
        if (strtok(NULL, " ")) {
			fprintf(stderr, "error: %lu: invalid trace %s\n", line_num, line);
			exit(1);
		}
		free(buf);
    }
	fclose(fp);
	ProcTE *current = proc_t->head;
	while (current) {
		current->fp = open_trace(fname);
		fseek(current->fp, current->first_byte, SEEK_SET);
		current = current->next;
	}
	return proc_t;
}
