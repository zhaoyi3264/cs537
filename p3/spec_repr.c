#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "spec_repr.h"

Node *create_node(char *data) {
	Node *node = malloc(sizeof(Node));
	node->data = malloc(sizeof(char) * strlen(data));
	sprintf(node->data, "%s", data);
	return node;
}

SpecNode *create_spec_node(char *target) {
	SpecNode *spec_node = malloc(sizeof(SpecNode));
 	spec_node->target = malloc(sizeof(char) * strlen(target));
	sprintf(spec_node->target, "%s", target);
	return spec_node;
}

void add_dependency(SpecNode *spec_node, char *dependency) {
	// TODO: check if dependency is a target or a file that exists
	Node *node = create_node(dependency);
	if (spec_node->dependencies) {
		spec_node->dependencies_tail->next = node;
	} else {
		spec_node->dependencies = node;
	}
	spec_node->dependencies_tail = node;
}

void add_command(SpecNode *spec_node, char *command) {
	Node *node = create_node(command);
	if (spec_node->commands) {
		spec_node->commands_tail->next = node;
	} else {
		spec_node->commands = node;
	}
	spec_node->commands_tail = node;
}

void print_dependency(SpecNode *spec_node) {
	printf("dep: ");
	Node *current = spec_node->dependencies;
	while (current) {
		printf("%s, ", current->data);
		current = current->next;
	}
	printf("\n");
}

void print_command(SpecNode *spec_node) {
	printf("cmd:\n");
	Node *current = spec_node->commands;
	while (current) {
		printf("  %s\n", current->data);
		current = current->next;
	}
}
