#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "process_control.h"
#include "text_parsing.h"

//~ void function(graph, dep) {
	//~ // find node with (target == dep)
	//~ for each dep' of dep
		//~ funtion(graph, dep')
	//~ // if not found, the dep is a file, do nothing
	//~ if dep is outdated
		//~ executed command // process control
	//~ return
//~ }

// extra credit

int main(int argc, char **argv) {
	int opt;
	char *fname = NULL;
	while ((opt = getopt(argc, argv, "-f:")) != -1) {
		switch (opt) {
			case 'f':
				fname = malloc(sizeof(char) * strlen(optarg));
				sprintf(fname, "%s", optarg);
				break;
			case 1:
				printf("%s\n", optarg);
				break;
			default:
				// TODO: report error
				break;
		}
	}
	SpecGraph *spec_graph = parse_makefile(fname);
	print_spec_graph(spec_graph);
	create_process(spec_graph->head->next);
	if (fname) {
		free(fname);
	}
}
