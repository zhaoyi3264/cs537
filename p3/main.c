#include <stdio.h>

#include "text_parsing.h"

void function(graph, dep) {
	// find node with (target == dep)
	for each dep' of dep
		funtion(graph, dep')
	// if not found, the dep is a file, do nothing
	if dep is outdated
		executed command // process control
	return
}

// command line parsing

// extra credit

int main () {
	SpecGraph *spec_graph = parse_makefile("test");
	print_spec_graph(spec_graph);
}
