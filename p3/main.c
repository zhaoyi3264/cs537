#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "process_control.h"
#include "text_parsing.h"

void get_modify_time(char *name, struct stat *statbuf) {
	int fd = open(name, O_RDONLY);
	//~ printf("fd of %s: %d\n", name, fd);
	fstat(fd, statbuf);
	if (fd == -1) {
		struct timespec *ts = &(statbuf->st_mtim);
		ts->tv_sec = 0;
		ts->tv_nsec = 0;
	}
}

int dependency_is_newer(char *target, char *dependency) {
	struct stat *target_stat = malloc(sizeof(struct stat));
	struct stat *dependency_stat = malloc(sizeof(struct stat));
	get_modify_time(target, target_stat);
	get_modify_time(dependency, dependency_stat);
	struct timespec *target_time = &(target_stat->st_mtim);
	struct timespec *dependency_time = &(dependency_stat->st_mtim);
	
	long tar;
	long dep;
	if (target_time->tv_sec && target_time->tv_sec == dependency_time->tv_sec) {
		tar = target_time->tv_nsec;
		dep = dependency_time->tv_nsec;
		//~ printf("ns %s: %ld, %s: %ld\n", target, tar, dependency, dep);
	} else {
		tar = target_time->tv_sec;
		dep = dependency_time->tv_sec;
		//~ printf("s  %s: %ld, %s: %ld\n", target, tar, dependency, dep);
	}
	free(target_stat);
	free(dependency_stat);
	return dep >= tar;
}

int is_outdated(SpecGraph *spec_graph, char *name) {
	//~ printf("checking %s\n", name);
	SpecNode *spec_node = find_spec_node(spec_graph, name);
	if (spec_node == NULL) {
		//~ printf("%s not found\n", name);
		return 0;
	}
	Node *current = spec_node->dependencies;
	// iterate through depdencies
	while (current) {
		// if file, check modify time
		if (dependency_is_newer(name, current->data)) {
			return 1;
		}
		if (is_outdated(spec_graph, current->data)) {
			return 1;
		}
		current = current->next;
	}
	return spec_node->dependencies == NULL;
}

void run_rule(SpecGraph *spec_graph, char *name) {
	// check if the name is a target
	SpecNode *spec_node = find_spec_node(spec_graph, name);
	// name is a file
	// TODO: check if file exist
	if (spec_node == NULL) {
		return;
	}
	Node *current = spec_node->dependencies;
	// iterate through depdencies
	while (current) {
		run_rule(spec_graph, current->data);
		current = current->next;
	}
	int outdated = is_outdated(spec_graph, name);
	//~ printf("%s is outdated: %d\n", name, outdated);
	if (outdated) {
		create_process(spec_node);
	}
}

// extra credit

int main(int argc, char **argv) {
	int opt;
	char *fname = NULL;
	int num_targets = 0;
	char *target;
	while ((opt = getopt(argc, argv, "-f:")) != -1) {
		switch (opt) {
			case 'f':
				fname = malloc(sizeof(char) * strlen(optarg));
				sprintf(fname, "%s", optarg);
				break;
			case 1:
				if (num_targets) {
					fprintf(stderr, "error: multiple targets specified\n");
					exit(1);
				}
				target = malloc(sizeof(char) * strlen(optarg));
				sprintf(target, "%s", optarg);
				num_targets++;
				break;
			default:
				// TODO: report error
				break;
		}
	}
	SpecGraph *spec_graph = parse_makefile(fname);
	print_spec_graph(spec_graph);
	if (num_targets) {
		printf("target: %s\n", target);
		run_rule(spec_graph, target);
		free(target);
	} else {
		run_rule(spec_graph, spec_graph->head->target);
	}
	if (fname) {
		free(fname);
	}
}
