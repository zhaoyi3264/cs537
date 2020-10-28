#include <stdlib.h>
#include <stdio.h>

#include "spec_repr.h"
#include "spec_graph.h"

void add_spec_node(SpecGraph *spec_graph, SpecNode *spec_node) {
	printf("add target: %s\n", spec_node->target);
	if (spec_graph->head) {
		spec_graph->tail->next = spec_node;
	} else {
		spec_graph->head = spec_node;
	}
	spec_graph->tail = spec_node;
}

void print_spec_graph(SpecGraph *spec_graph) {
	printf("***graph***\n");
	SpecNode *cur_spec_node = spec_graph->head;
	while (cur_spec_node) {
		printf("target: %s\n", cur_spec_node->target);
		print_dependency(cur_spec_node);
		//~ print_command(cur_spec_node);
		printf("\n");
		cur_spec_node = cur_spec_node->next;
	}
	printf("***\n");
}
