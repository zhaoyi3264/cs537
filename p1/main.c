#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "stat_file_parser.h"
#include "process_list.h"

void get_proc_file(char *dest, char *pid, char *file) {
	strcat(dest, "/proc/");
	strcat(dest, pid);
	strcat(dest, "/");
	strcat(dest, file);
}

// don't forget extra credit
int main (int argc, char *argv[]) {
	// stat_file_parser
	//~ struct Stat *stat = malloc(sizeof(struct Stat));
	//~ parse_stat("/proc/1/stat", stat);
	//~ int vm = parse_statm("/proc/1/statm");
	//~ printf("%d %c %d %d\n", stat->pid, stat->state, stat->utime, stat->stime);
	//~ printf("%d\n", vm);
	//~ printf("%s %s\n", get_proc_uid("/proc/1/status"), get_user_uid());
	//~ char *cmd = malloc(sizeof(char) * 128);
	//~ get_cmd("/proc/1/cmdline", cmd);
	//~ printf("%s\n", cmd);
	//~ free(stat);
	//~ free(cmd);
	
	//~ // main
	//~ char *file = malloc(sizeof(char) * 16);
	//~ get_proc_file(file, "123", "stat");
	//~ printf("%s\n", file);
	//~ free(file);
	
	// 
	int opt, p, state, utime, stime, vm, cmd;
	char pid[8];
	while ((opt = getopt(argc, argv, "p:sUSvc")) != -1) {
		printf("%c\n", opt);
		switch(opt) {
			case 'p':
				p = 1;
				strcpy(pid, optarg);
				break;
			case 's':
				state = 1;
				break;
			case 'U':
				utime = 1;
				break;
			case 'S':
				stime = 1;
				break;
			case 'v':
				vm = 1;
				break;
			case 'c':
				cmd = 1;
				break;
			default:
				break;
		}
	}
	
	state = 1, utime = 1, stime = 1, vm = 1, cmd = 1;
	//~ process_list
	struct PNode *head;
	if (p) {
		head = malloc(sizeof(struct PNode));
		head->pid = pid;
	} else {
		head = get_proc(1);
	}
	struct PNode *previous = head;
	struct PNode *current = head;
	struct Stat *stat = malloc(sizeof(struct Stat));
	while(current) {
		char *file = calloc(16, sizeof(char));
		// TODO: check if file exists
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
