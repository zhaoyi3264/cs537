/*
 * Specification graph module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "spec_graph.h"

/*
 * Check if a dependency exists in the specificaiton graph
 * 
 * v: specification graph
 * size: size of the graph
 * dependency: name of the dependency
 * 
 * return: the index of the dependency, -1 if not found
 */
int exist(SpecNode *v[], int size, char *dependency) {
	for (int i = 0; i < size; i++) {
		if (strcmp(v[i]->target, dependency) == 0) {
			return i;
		}
	}
	return -1;
}

/*
 * Helper method for checking cycle in the graph
 * 
 * v: specification graph
 * size: size of the graph
 * idx: index of the node to check
 * visited: array which indicates if a node has been visited
 * rec_stack: array which indicates if a node is in the recursion stack
 * 
 * return: 1 if there is a cycle, 0 otherwise
 */
int check_cycle_util(SpecNode *v[], int size, int idx, int *visited,
	int *rec_stack) {
	if (rec_stack[idx]) {
		return 1;
	}
	if (visited[idx]) {
		return 0;
	}
	visited[idx] = 1;
	
	rec_stack[idx] = 1;
	Node *current = v[idx]->dependencies;
	int idx_new;
	while (current) {
		if ((idx_new = exist(v, size, current->data)) != -1) {
			if (check_cycle_util(v, size, idx_new, visited, rec_stack)) {
				return 1;
			}
		}
		current = current->next;
	}
	rec_stack[idx] = 0;
	return 0;
}

/*
 * check if there is a cycle in the specification graph
 * 
 * spec_graph: specification graph
 * 
 * return: 1 if there is a cycle, 0 otherwise
 */
int check_cycle(SpecGraph *spec_graph) {
	int size = spec_graph->size;
	SpecNode *v[size];
	int *visited = malloc(sizeof(int) * size);
	int *rec_stack = malloc(sizeof(int) * size);
	if (visited == NULL || rec_stack == NULL) {
		exit(1);
	}
	SpecNode *current = spec_graph->head;
	for (int i = 0; current; current = current->next, i++) {
		v[i] = current;
		visited[i] = 0;
		rec_stack[i] = 0;
	}
	for (int i = 0; i < size; i++) {
		if (check_cycle_util(v, size, i, visited, rec_stack)) {
			return 1;
		}
	}
	free(rec_stack);
	free(visited);
	return 0;
}

/*
 * Add a specification node to the specification graph
 * 
 * spec_graph: specification graph
 * spec_node: specification node
 * 
 * return: 0 if no cycle, 1 if there is a cycle, 2 if there is a duplicate node
 */
int add_spec_node(SpecGraph *spec_graph, SpecNode *spec_node) {
	if (spec_graph->head) {
		// check duplicate
		SpecNode *current = spec_graph->head;
		while (current) {
			if (strcmp(spec_node->target, current->target) == 0) {
				return 2;
			}
			current = current->next;
		}
		spec_graph->tail->next = spec_node;
	} else {
		spec_graph->head = spec_node;
	}
	spec_graph->tail = spec_node;
	spec_graph->size = spec_graph->size + 1;
	return check_cycle(spec_graph);
}

/*
 * Find a specification node in the specification graph
 * 
 * spec_graph: specification graph
 * name: name of the specification node
 * 
 * return: specification node if found, NULL otherwise
 */
SpecNode *find_spec_node(SpecGraph *spec_graph, char *name) {
	SpecNode *current = spec_graph->head;
	while (current) {
		if (strcmp(name, current->target) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

/* 
 * Print the specification graph
 * 
 * spec_graph: specification graph
 */
void print_spec_graph(SpecGraph *spec_graph) {
	printf("***graph***\n");
	SpecNode *cur_spec_node = spec_graph->head;
	while (cur_spec_node) {
		printf("target: %s\n", cur_spec_node->target);
		print_dependency(cur_spec_node);
		print_command(cur_spec_node);
		printf("\n");
		cur_spec_node = cur_spec_node->next;
	}
	printf("***size: %d***\n", spec_graph->size);
}
