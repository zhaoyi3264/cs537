#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "process_control.h"
#include "text_parsing.h"

void run_rule(SpecGraph *spec_graph, char *target) {
	// find target node
	SpecNode *spec_node = find_spec_node(spec_graph, target);
	if (spec_node == NULL) {
		fprintf(stderr, "error: cannot the target %s\n", target);
		exit(1);
	}
	Node *current = spec_node->dependencies;
	// iterate through depdencies
	while (current) {
		run_rule(spec_graph, current->data);
		current = current->next;
	}
	// if not found, the dep is a file, do nothing
	// if dep is outdated
	create_process(spec_node);
}

// extra credit

int main(int argc, char **argv) {
	int opt;
	char *fname = NULL;
	int num_targets = 0;
	char *targets[argc];
	while ((opt = getopt(argc, argv, "-f:")) != -1) {
		switch (opt) {
			case 'f':
				fname = malloc(sizeof(char) * strlen(optarg));
				sprintf(fname, "%s", optarg);
				break;
			case 1:
				printf("%s\n", optarg);
				targets[num_targets] = malloc(sizeof(char) * strlen(optarg));
				sprintf(targets[num_targets], "%s", optarg);
				num_targets++;
				break;
			default:
				// TODO: report error
				break;
		}
	}
	SpecGraph *spec_graph = parse_makefile(fname);
	print_spec_graph(spec_graph);
	//~ create_process(spec_graph->head->next);
	// run the first rule by default
	if (num_targets == 0) {
		run_rule(spec_graph, spec_graph->head->target);
	}
	for (int i = 0; i < num_targets; i++) {
		printf("executing %s\n", targets[i]);
		run_rule(spec_graph, targets[i]);
		free(targets[i]);
	}
	
	if (fname) {
		free(fname);
	}
}
