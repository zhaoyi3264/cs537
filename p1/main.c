/*
 * Authors: 
 * Zhaoyi Zhang, netid: zzhang825
 * Richard Li, netid: tli354
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "stat_file_parser.h"
#include "process_list.h"
#include "options_processing.h"

<<<<<<< HEAD
// TODO: use snprintf
=======
/* 
 * Gets the spefified file under the pid repository
 */
>>>>>>> origin/Richard
void get_proc_file(char *dest, char *pid, char *file) {
	strcat(dest, "/proc/");
	strcat(dest, pid);
	strcat(dest, "/");
	strcat(dest, file);
}

/* 
 * Main method to mobilize the program
 * Prints corresponding process information as the command prompts
 */
int main (int argc, char *argv[]) {
	int p, state, utime, stime, vm, cmd;
	p = state = stime = vm = 0;
	utime = cmd = 1;
	struct PNode *head = parse_cmdline_options(argc, argv, &p, &state, &utime,
		&stime, &vm, &cmd);
	if (!head) {
		head = get_proc(1);
	}
	struct PNode *current = head;
	struct PNode *previous = head;
	struct Stat *stat = malloc(sizeof(struct Stat));
	while(current) {
		char *file = calloc(16, sizeof(char));
		get_proc_file(file, current->pid, "stat");
		parse_stat(file, stat);
		
		printf("%s:", current->pid);
		if (state) {
			printf("\t %c", stat->state);
		}
		if (utime) {
			printf("\t utime=%d", stat->utime);
		}
		if (stime) {
			printf("\t stime=%d", stat->utime);
		}
		if (vm) {
			char *statm_file = calloc(16, sizeof(char));
			get_proc_file(statm_file, current->pid, "statm");
			printf("\t vm=%d", parse_statm(statm_file));
			free(statm_file);
		}
		if (cmd) {
			char *cmd = calloc(256, sizeof(char));
			char *cmd_file = calloc(16, sizeof(char));
			get_proc_file(cmd_file, current->pid, "cmdline");
			get_cmd(cmd_file, cmd);
			printf("\t [ %s ]", cmd);
			free(cmd);
			free(cmd_file);
		}
		printf("\n");
		
		current = current->next;
		free(previous);
		previous = current;
		free(file);
	}
	free(stat);
}
