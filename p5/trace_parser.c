/*
 * Trace file parsing module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */
 
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "trace_parser.h"

/*
 * Check if a string is a number
 * 
 * str: the string
 * 
 * return: 1 is the string is a number, 0 otherwise
 */
int is_number(char * str) {
	char c = 0;
	for (int i = 0; (c = str[i]); i++) {
		if (isdigit(c) == 0) {
			return 0;
		}
	}
	return 1;
}

/*
 * Open a trace file
 * 
 * fname: the file name of the trace file
 * 
 * return: the file pointer of the trace file
 */
FILE *open_trace(char *fname) {
	FILE *fp = fopen(fname, "r");
	if (fp == NULL) {
		fprintf(stderr, "error: file %s not found\n", fname);
		exit(1);
	}
	return fp;	
}

/*
 * Check if a pid of vpn is a valid number
 * 
 * str: the string
 * pid_vpn: whether the string is a pid or vpn
 * line: the trace that contains the string
 * line_num: line number in the trace file
 */
void validate_pid_vpn(char *str, char *pid_vpn, char *line, unsigned long line_num) {
	if (str == NULL) {
		fprintf(stderr, "error: %lu: invalid trace %s\n", line_num, line);
		exit(1);
	} else if (is_number(str) == 0) {
		fprintf(stderr, "error: %lu: invalid %s %s\n", line_num, pid_vpn, str);
		exit(1);
	}
}

/*
 * Parse the trace file into a process table
 * 
 * fname: the file name of the trace file
 * 
 * return: the process table
 */
ProcT *parse_trace(char *fname) {
	if (fname == NULL) {
		fprintf(stderr, "error: no trace file specified\n");
		exit(1);
	}
	// create necessary variables
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
		if (buf == NULL || sprintf(buf, "%s", line) < 0) {
			exit(1);
		}
		// validate the pid and vpn
		token = strtok(buf, " ");
		validate_pid_vpn(token, "pid", line, line_num);
		pid = strtoul(token, NULL, 10);
		token = strtok(NULL, " ");
		validate_pid_vpn(token, "vpn", line, line_num);
		update_proc_te_trace(proc_t, pid, ftell(fp) - size);
		// check if there are only two fields in the line
		if (strtok(NULL, " ")) {
			fprintf(stderr, "error: %lu: invalid trace %s\n", line_num, line);
			exit(1);
		}
		free(buf);
    }
	if (fclose(fp) != 0) {
		exit(1);
	}
	// open a file pointer for each process and point to its early occurrence
	ProcTE *current = proc_t->head;
	while (current) {
		current->fp = open_trace(fname);
		if (fseek(current->fp, current->first_byte, SEEK_SET) < 0) {
			exit(1);
		}
		current = current->next;
	}
	return proc_t;
}
