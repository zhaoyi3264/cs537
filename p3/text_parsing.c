/*
 * Text parsing module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "text_parsing.h"

/*
 * Check if a string is blank
 * 
 * line: string
 * 
 * return: 1 if the line is blank, 0 otherwise
 */
int is_blank(char *line) {
	for (int i = 0; line[i]; i++) {
		if (isspace(line[i]) == 0) {
			return 0;
		}
	}
	return 1;
}

/*
 * Check if a string is a target line
 * 
 * line: string
 * 
 * return: 1 if the line is a target line, 0 otherwise
 */
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

/*
 * Check if a string is a command line
 * 
 * line: string
 * 
 * return: 1 if the line is a command line, 0 otherwise
 */
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

/*
 * Update the specification graph by adding a specification node
 * 
 * spec_graph: specification graph
 * spec_node: specificaiton node
 * line_num_target: line number of the target
 * line_target: target line
 */
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

/*
 * Parse the makefile as a specification graph
 * 
 * fname: file name of the makefile
 * 
 * return: specificaiton graph representation of the makefile
 */
SpecGraph *parse_makefile (char *fname) {
	int max_size = 4096;
	FILE *fp = NULL;
	if (fname) {
		 fp = fopen(fname, "r");
		 if (fp == NULL) {
			 fprintf(stderr, "error: makefile %s not found\n", fname);
			 exit(1);
		 }
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
    
	int c = 0;
	int idx = 0;
	
	char * line = NULL;
    int line_num = 0;
    char *line_target = malloc(sizeof(char) * max_size + 1);
    if (line_target == NULL) {
		exit(1);
	}
    int line_num_target = 0;
    
	SpecNode *spec_node = NULL;
	SpecGraph *spec_graph = malloc(sizeof(SpecGraph));
	if (spec_graph == NULL) {
		exit(1);
	}
	
	do {
		// read line
		idx = 0;
		line = malloc(sizeof(char) * max_size);
		if (line == NULL) {
			exit(1);
		}
		do {
			if ((c = fgetc(fp)) == '\0') {
				fprintf(stderr, "%d: null byte found: %s\n", line_num, line);
				exit(1);
			}
			line[idx++] = (char) c;
			if (idx >= max_size) {
				fprintf(stderr, "%d: line length exceeds the buffer size: %s\n",
					line_num, line);
				exit(1);
			}
		} while (c != EOF && c != '\n');
		line_num++;
		
		line[idx - 1] = '\0';
		
		char *dep_string = NULL;
		char *token = NULL;
		char *ws = NULL;
		// a comment or blank line
		if (line[0] == '#' || is_blank(line)) {
			continue;
		// a target line
		} else if (is_target(line)) {
			update_graph(spec_graph, spec_node, line_num_target, line_target);
			if (sprintf(line_target, "%s", line) < 0) {
				exit(1);
			}
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
			add_command(spec_node, line + 1);
		// error
		} else {
			fprintf(stderr, "%d: invalid line: %s\n", line_num, line);
			exit(1);
		}
		free(line);
	} while (c != EOF);
	update_graph(spec_graph, spec_node, line_num_target, line_target);
	if (fclose(fp) != 0) {
		exit(1);
	}
	free(line_target);
	return spec_graph;
}
