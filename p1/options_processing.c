#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "struct.h"
#include "options_processing.h"

int toggle_flag() {
	return optarg == NULL || strcmp(optarg, "-");
}

struct PNode *parse_cmdline_options(int argc, char *argv[],
	int *p, int *state, int *utime, int *stime, int *vm, int *cmd) {
	int opt;
	struct PNode *head = malloc(sizeof(struct PNode));
	struct PNode *current = head;
	while ((opt = getopt(argc, argv, "p:s::U::S::v::c::")) != -1) {
		switch(opt) {
			case 'p':
				*p = 1;
				current->next = malloc(sizeof(struct PNode));
				current = current->next;
				current->pid = optarg;
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
	if ((*p)) {
		// skip and free the first empty node
		struct PNode *temp = head;
		head = head->next;
		free(temp);
		return head;
	} else {
		free(head);
		return NULL;
	}
}
