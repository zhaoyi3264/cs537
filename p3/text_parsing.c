#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "text_parsing.h"

int is_blank(char *line) {
	for (int i = 0; line[i]; i++) {
		if (isspace(line[i]) != 0) {
			return 0;
		}
	}
	return 1;
}

int is_target (char *line) {
	char c = line[0];
	if (c == '\0') {
		return 0;
	}
	if (c != ':' && c != ' ' && index(line, ':')) {
		return 1;
	}
	return 0;
}

int is_cmd (char *line) {
	char c = line[0];
	if (c == '\0') {
		return 0;
	}
	if (c == '\t' && !is_blank(line)) {
		return 1;
	}
	return 0;
}

void update_graph(SpecGraph* spec_graph, SpecNode *spec_node,
	int line_num_target, char *line_target) {
	if (spec_node) {
		int result = add_spec_node(spec_graph, spec_node);
		if (result == 1){
			fprintf(stderr, "%d: cycle in the dependency chain: %s\n",
				line_num_target, line_target);
			exit(1);
		} else if (result == 2) {
			fprintf(stderr, "%d: duplicate target: %s\n",
				line_num_target, line_target);
			exit(1);
		}
	}
}

SpecGraph *parse_makefile (char *fname) {
	int max_size = 4096;
	FILE *fp;
	if (fname) {
		 fp = fopen(fname, "r");
	} else {
		char *file[] = {"makefile", "Makefile"};
		for (int i = 0; i < 2; i++) {
			fp = fopen(file[i], "r");
			if (fp) {
				break;
			}
		}
		if (fp == NULL) {
			fprintf(stderr, "error: cannot find makefile\n");
			exit(1);
		}
	}
    
	int c;
	int idx;
	
	char * line = NULL;
    int line_num = 0;
    char *line_target = malloc(sizeof(char) * max_size);
    int line_num_target = 0;
    
	SpecNode *spec_node = NULL;
	SpecGraph *spec_graph = malloc(sizeof(SpecGraph));
	
	do {
		// read line
		idx = 0;
		line = malloc(sizeof(char) * max_size);
		do {
			c = fgetc(fp);
			if (c == '\0') {
				fprintf(stderr, "%d: null byte found: %s\n", line_num, line);
			}
			line[idx++] = (char) c;
		} while (c != '\n' && c != EOF && idx < max_size);
		line_num++;
		if (idx >= max_size) {
			fprintf(stderr, "%d: line length exceeds the buffer size: %s\n",
				line_num, line);
			exit(1);
		} else {
			line[idx - 1] = '\0';
		}
		
		char *dep_string = NULL;
		char *token = NULL;
		char *ws = NULL;
		// a comment line
		if (line[0] == '#') {
			//~ printf("comment: %s\n", line);
			continue;
		// a target line
		} else if (is_target(line)) {
			//~ printf("target: %s\n", line);
			update_graph(spec_graph, spec_node, line_num_target, line_target);
			sprintf(line_target, "%s", line);
			line_num_target = line_num;
			// replace tabs with spaces
			while ((ws = index(line, '\t'))) {
				*ws = ' ';
			}
			token = strtok(line, ":");
			// replace the first space with null byte
			if ((ws = index(token, ' '))) {
				*ws = '\0';
			}
			spec_node = create_spec_node(token);
			if ((dep_string = strtok(NULL, ":")) &&
				(token = strtok(dep_string, " "))) {
				add_dependency(spec_node, token);
				while ((token = strtok(NULL, " "))) {
					add_dependency(spec_node, token);
				}
			}
		// a command line
		} else if (is_cmd(line)) {
			//~ printf("command: %s\n", line);
			add_command(spec_node, line + 1);
		// blank
		} else if (is_blank(line)) {
			//~ printf("blank: %s\n", line);
			continue;
		// error
		} else {
			fprintf(stderr, "%d: invalid line: %s\n", line_num, line);
			exit(1);
		}
		free(line);
	} while (c != EOF);
	update_graph(spec_graph, spec_node, line_num_target, line_target);
	fclose(fp);
	free(line_target);
	return spec_graph;
}
