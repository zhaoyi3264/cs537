/*
 * Authors: 
 * - Zhaoyi Zhang, netid: zzhang825
 * - Richard Li, netid: tli354
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "common.h"
#include "options_processing.h"

/*
 * Toggles the option flag depending on the optarg
 */
int toggle_flag() {
	return optarg == NULL || strcmp(optarg, "-");
}

/*
 * Checks if the process already exists in the process list
 * 
 * head: the head of the process list
 * pid: the pid to check
 * 
 * return: 1 if the process exists in the process list
 */
int exist(struct PNode *head, char *pid) {
	struct PNode *current = head;
	while(current) {
		if (!strcmp(current->pid, pid)) {
			return 1;
		}
		current = current->next;
	}
	return 0;
}

/*
 * Parses command line options
 * 
 * argc: the command line argument count
 * argv: the command line argument values
 * 
 * return: the process list
 */
struct PNode *parse_cmdline_options(int argc, char *argv[],
	int *p, int *state, int *utime, int *stime, int *vm, int *cmd) {
	int opt;
	int is_pid = 1;
	struct PNode *head = NULL;
	struct PNode *current = NULL;
	// Parses required and optional arguments
	while ((opt = getopt(argc, argv, "p:s::U::S::v::c::")) != -1) {
		switch(opt) {
			case 'p':
				*p = 1;
				is_pid = 1;
				// check if is a valid pid
				for (int i = 0; optarg[i] != '\0'; i++) {
					if (!isdigit(optarg[i])) {
						is_pid = 0;
						break;
					}
				}
				if (is_pid && !exist(head, optarg)) {
					if (!head) {
						head = malloc(sizeof(struct PNode));
						current = head;
					} else {
						current->next = malloc(sizeof(struct PNode));
						current = current->next;
					}
					current->pid = optarg;
				}
				break;
			case 's':
				*state = toggle_flag();
				break;
			case 'U':
				*utime = toggle_flag();
				break;
			case 'S':
				*stime = toggle_flag();
				break;
			case 'v':
				*vm = toggle_flag();
				break;
			case 'c':
				*cmd = toggle_flag();
				break;
			default:
				break;
		}
	}
	return head;
}
