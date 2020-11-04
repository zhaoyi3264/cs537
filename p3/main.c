/*
 * Main driver module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */
 
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "process_control.h"
#include "text_parsing.h"

/* 
 * Get the modification time of a file
 * 
 * name: name of the file
 * statbuf: the stat structure to store the time
 */
void get_modify_time(char *name, struct stat *statbuf) {
	int fd = open(name, O_RDONLY);
	// if not found, set time to 0
	if (fstat(fd, statbuf) == -1) {
		struct timespec *ts = &(statbuf->st_mtim);
		ts->tv_sec = 0;
		ts->tv_nsec = 0;
	}
}

/*
 * Check if the dependency has a newer modification time than the target
 * 
 * target: name of the target file
 * dependency: name of the dependency file
 * 
 * return: 1 if the depndency is newer, 0 otherwise
 */
int dependency_is_newer(char *target, char *dependency) {
	struct stat *target_stat = malloc(sizeof(struct stat));
	struct stat *dependency_stat = malloc(sizeof(struct stat));
	if (target_stat == NULL || dependency_stat == NULL) {
		exit(1);
	}
	get_modify_time(target, target_stat);
	get_modify_time(dependency, dependency_stat);
	struct timespec *target_time = &(target_stat->st_mtim);
	struct timespec *dependency_time = &(dependency_stat->st_mtim);
	
	long tar = 0;
	long dep = 0;
	if ((target_time->tv_sec || dependency_time->tv_sec) &&
		target_time->tv_sec == dependency_time->tv_sec) {
		tar = target_time->tv_nsec;
		dep = dependency_time->tv_nsec;
	} else {
		tar = target_time->tv_sec;
		dep = dependency_time->tv_sec;
	}
	free(target_stat);
	free(dependency_stat);
	return dep > tar;
}

/*
 * Check if a target or file in the makefile is outdated
 * 
 * spec_graph: specification graph
 * name: name of the target of file
 * 
 * return: 1 if the target or file is outdated
 */
int is_outdated(SpecGraph *spec_graph, char *name) {
	SpecNode *spec_node = find_spec_node(spec_graph, name);
	if (spec_node == NULL) {
		return 0;
	}
	Node *current = spec_node->dependencies;
	// iterate through depdencies to check if they are outdated
	while (current) {
		if (dependency_is_newer(name, current->data) ||
			is_outdated(spec_graph, current->data)) {
			return 1;
		}
		current = current->next;
	}
	return spec_node->dependencies == NULL;
}

/*
 * Run the rule in the makefile, and update dependencies if necessary
 * 
 * spec_graph: specification graph
 * name: name of the target or file
 * 
 * return: 1 if the rule is executed, 0 otherwise
 */
int run_rule(SpecGraph *spec_graph, char *name) {
	// check if the name is a target
	SpecNode *spec_node = find_spec_node(spec_graph, name);
	// name is a file
	if (spec_node == NULL) {
		return 0;
	}
	Node *current = spec_node->dependencies;
	int result = 0;
	// iterate through depdencies and run their rules if necessary
	while (current) {
		if (run_rule(spec_graph, current->data)) {
			result = 1;
		}
		current = current->next;
	}
	int outdated = is_outdated(spec_graph, name);
	if (outdated) {
		current = spec_node->commands;
		while (current) {
			create_process(current->data);
			current = current->next;
		}
		result = 1;
	}
	return result;
}

/*
 * Main method
 * 
 * argc: command line argument count
 * argv: command line argument values
 */
int main(int argc, char **argv) {
	int opt = 0;
	char *fname = NULL;
	int num_targets = 0;
	char *target = NULL;
	while ((opt = getopt(argc, argv, "-f:")) != -1) {
		switch (opt) {
			case 'f':
				fname = malloc(sizeof(char) * strlen(optarg));
				if (fname == NULL || (sprintf(fname, "%s", optarg) < 0)) {
					exit(1);
				}
				break;
			case 1:
				if (num_targets) {
					fprintf(stderr, "error: multiple targets specified\n");
					exit(1);
				}
				target = malloc(sizeof(char) * strlen(optarg));
				if (target == NULL || (sprintf(target, "%s", optarg) < 0)) {
					exit(1);
				}
				num_targets++;
				break;
			default:
				exit(1);
		}
	}
	SpecGraph *spec_graph = parse_makefile(fname);
	//~ print_spec_graph(spec_graph);
	if (fname) {
		free(fname);
	}
	if (num_targets) {
		if (find_spec_node(spec_graph, target)) {
			if (run_rule(spec_graph, target) == 0) {
				printf("%s is up to date\n", target);
			}
		} else {
			fprintf(stderr, "error: cannot find target %s\n", target);
			exit(1);
		}
		free(target);
	} else {
		if (run_rule(spec_graph, spec_graph->head->target) == 0) {
			printf("%s is up to date\n", spec_graph->head->target);
		}
	}
}
