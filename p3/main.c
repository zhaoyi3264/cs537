#include <stdio.h>

#include "text_parsing.h"

int main () {
	SpecGraph *spec_graph = parse_makefile("test");
	print_spec_graph(spec_graph);
}
