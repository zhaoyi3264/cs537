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
	
	// process_list
	//~ struct PNode *head = get_user_proc();
	//~ struct PNode *previous = head;
	//~ struct PNode *current = head;
	//~ while(current) {
		//~ printf("%s\n", current->pid);
		//~ current = current->next;
		//~ free(previous);
		//~ previous = current;
	//~ }
	
	// main
	//~ char *file = malloc(sizeof(char) * 16);
	//~ get_proc_file(file, "123", "stat");
	//~ printf("%s\n", file);
	//~ free(file);
	
	// 
	int opt, p, status, ut, st, vm, cmd;
	while ((opt = getopt(argc, argv, "p:sUSvc")) != -1) {
		switch(opt) {
			case 'p':
				p = 1;
				break;
			case 's':
				status = 1;
				break;
			case 'U':
				ut = 1;
				break;
			case 'S':
				st = 1;
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
}
