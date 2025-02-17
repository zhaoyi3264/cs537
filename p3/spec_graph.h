/*
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */
 
#ifndef SPEC_GRAPH_H
#define SPEC_GRAPH_H

#include "spec_repr.h"

typedef struct SpecGraph {
	SpecNode *head;
	SpecNode *tail;
	int size;
} SpecGraph;

int check_cycle(SpecGraph *spec_graph);

int add_spec_node(SpecGraph *spec_graph, SpecNode *spec_node);

SpecNode *find_spec_node(SpecGraph *spec_graph, char *name);

void print_spec_graph(SpecGraph *spec_graph);

#endif
