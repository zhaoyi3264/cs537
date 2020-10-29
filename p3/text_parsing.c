#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "spec_repr.h"
#include "spec_graph.h"

void parse_makefile (char *fname) {
	FILE *fp = fopen(fname, "r");
	char * line = NULL;
    size_t len = 0;
    int size;
    int buf_limit = 4096;
	SpecNode *spec_node = NULL;
	SpecGraph *spec_graph = malloc(sizeof(SpecGraph));
    while ((size = getline(&line, &len, fp)) != -1) {
		if (size > buf_limit) {
			fprintf(stderr, "error: the line is too long\n");
			continue;
		}
		
		line[size - 1] = '\0';
		char *dep_string = NULL;
		char *token = NULL;
		// a target line
		if (index(line, ':')) {
			if (spec_node) {
				add_spec_node(spec_graph, spec_node);
				spec_node = NULL;
			}
			// TODO: split on multiple white spaces
			token = strtok(line, ":");
			//~ printf("target: %s\n", token);
			spec_node = create_spec_node(token);
			dep_string = strtok(NULL, ":");
			//~ printf("dep string: %s\n", dep_string);
			token = strtok(dep_string, " ");
			//~ printf("target: %s dep: %s\n", spec_node->target, token);
			add_dependency(spec_node, token);
			while ((token = strtok(NULL, " "))) {
				//~ printf("target: %s dep: %s\n", spec_node->target, token);
				add_dependency(spec_node, token);
			}
			token = NULL;
		// a command line
		} else if (line[0] == '\t') {
			//~ printf("target: %s command: %s\n", spec_node->target, line + 1);
			add_command(spec_node, line + 1);
		} else {
			//~ printf("skip something else: %s\n", line);
		}
    }
	if (spec_node) {
		add_spec_node(spec_graph, spec_node);
	}
    print_spec_graph(spec_graph);
	fclose(fp);
}
