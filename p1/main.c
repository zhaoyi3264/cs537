#include <stdio.h>
#include <stdlib.h>
#include "stat_file_parser.h"
#include "process_list.h"

int main () {
	struct Stat *stat = malloc(sizeof(struct Stat));
	parse_stat("/proc/1/stat", stat);
	int vm = parse_statm("/proc/1/statm");
	printf("%d %c %d %d\n", stat->pid, stat->state, stat->utime, stat->stime);
	printf("%d\n", vm);
	printf("%s\n", get_proc_uid("/proc/1/status"));
	printf("%s\n", get_user_uid());
	free(stat);
	
	struct PNode *head = read_user_proc();
	struct PNode *current = head;
	while(current) {
		printf("%s\n", current->pid);
		current = current->next;
	}
}
