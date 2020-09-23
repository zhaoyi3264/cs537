/*
 * Main driver module
 * 
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "stat_file_parser.h"
#include "process_list.h"
#include "options_processing.h"

/* 
 * Concatenates pid and file into a string of /proc/[pid]/[file]
 * 
 * dest: the buffer to store the result string
 * pid: the pid of the process
 * file: the name of the file under "/proc/[pid]"
 * 
 * return: the string of the path to the specified file
 */
void get_proc_file(char *dest, char *pid, char *file) {
	if (snprintf(dest, 64, "/proc/%s/%s", pid, file) < 0) {
		exit(1);
	}
}

/* 
 * Main driver
 * 
 * argc: the command line argument count
 * argv: the command line argument values
 * 
 * return: exit status
 */
int main (int argc, char *argv[]) {
	// option flags
	int p, state, utime, stime, vm, cmd, m, size;
	// set default values for option flags
	p = state = stime = vm = m = size = 0;
	utime = cmd = 1;
	long int addr = 0;
	// parse the command line options and get the list of processes to print
	struct PNode *head = parse_cmdline_options(argc, argv, &p, &state, &utime,
		&stime, &vm, &cmd, &m, &addr, &size);
	if (!head) {
		head = get_proc(1);
	}
	struct PNode *current = head;
	struct PNode *previous = head;
	struct Stat *stat = malloc(sizeof(struct Stat));
	if (stat == NULL) {
		exit(1);
	}
	// iterate through the PNode list
	while(current) {
		char *file = malloc(16 * sizeof(char));
		if (file == NULL) {
			exit(1);
		}
		get_proc_file(file, current->pid, "stat");
		parse_stat(file, stat);
		
		char *buf = malloc(size);
		if (buf == NULL) {
			exit(1);
		}
		// check if multiple -p options are provided
		if (m) {
			if (head->next) {
				printf("error: cannot provide multiple pids when -m option is present\n");
				exit(1);
			}
			get_mem(head->pid, buf, addr, size);
		}
		printf("%s:", current->pid);
		if (state) {
			printf(" %c", stat->state);
		}
		if (utime) {
			printf("\t utime=%lu", stat->utime);
		}
		if (stime) {
			printf("\t stime=%lu", stat->utime);
		}
		if (vm) {
			char *statm_file = malloc(16 * sizeof(char));
			if (statm_file == NULL) {
				exit(1);
			}
			get_proc_file(statm_file, current->pid, "statm");
			printf("\t vm=%d", parse_statm(statm_file));
			free(statm_file);
		}
		if (cmd) {
			char *cmd = malloc(512 * sizeof(char));
			if (cmd == NULL) {
				exit(1);
			}
			char *cmd_file = malloc(32 * sizeof(char));
			if (cmd_file == NULL) {
				exit(1);
			}
			get_proc_file(cmd_file, current->pid, "cmdline");
			get_cmd(cmd_file, cmd);
			printf("\t [%s]", cmd);
			free(cmd);
			free(cmd_file);
		}
		if (m) {
			printf(" \tmemoery %ld=", addr);
			for (int i = 0; i < size; i++) {
				printf("%x ", buf[i]);
			}
		}
		printf("\n");
		
		current = current->next;
		free(previous);
		previous = current;
		free(file);
		free(buf);
	}
	free(stat);
}
