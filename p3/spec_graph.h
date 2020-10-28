typedef struct SpecGraph {
	SpecNode *head;
	SpecNode *tail;
} SpecGraph;

void add_spec_node(SpecGraph *spec_graph, SpecNode *spec_node);

void print_spec_graph(SpecGraph *spec_graph);
