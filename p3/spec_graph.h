#include "spec_repr.h"

typedef struct SpecGraph {
	SpecNode *head;
	SpecNode *tail;
	int size;
} SpecGraph;

int check_cycle(SpecGraph *spec_graph);

int add_spec_node(SpecGraph *spec_graph, SpecNode *spec_node);

void print_spec_graph(SpecGraph *spec_graph);
