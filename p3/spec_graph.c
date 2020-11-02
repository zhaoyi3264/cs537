#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "spec_graph.h"

int exist(SpecNode *v[], int size, char *dependency) {
	for (int i = 0; i < size; i++) {
		if (strcmp(v[i]->target, dependency) == 0) {
			return i;
		}
	}
	return -1;
}

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

int check_cycle(SpecGraph *spec_graph) {
	int size = spec_graph->size;
	SpecNode *current = spec_graph->head;
	SpecNode *v[size];
	int *visited = malloc(sizeof(int) * size);
	int *rec_stack = malloc(sizeof(int) * size);
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
	free(visited);
	free(rec_stack);
	return 0;
}

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
