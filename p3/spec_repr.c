#include <stdlib.h>
#include <stdio.h>

#include "spec_repr.h"

Node *create_node(char *data) {
	Node *node = malloc(sizeof(Node));
	node->data = malloc(sizeof(char) * 1024);
	sprintf(node->data, "%s", data);
	return node;
}

SpecNode *create_spec_node(char *target) {
	SpecNode *spec_node = malloc(sizeof(spec_node));
	spec_node->target = malloc(sizeof(char) * 128);
	spec_node->dependencies = NULL;
	spec_node->dependencies_tail = NULL;
	spec_node->commands = NULL;
	spec_node->commands_tail = NULL;
	spec_node->next = NULL;
	printf("create node: %s\n", target);
	sprintf(spec_node->target, "%s", target);
	//~ spec_node->target = "TARGET";
	printf("%p\n", spec_node->dependencies);
	printf("%p\n", spec_node->dependencies_tail);
	printf("%p\n", spec_node->commands);
	printf("%p\n", spec_node->commands_tail);
	//~ printf("%p\n", spec_node->commands);
	return spec_node;
}

void add_dependency(SpecNode *spec_node, char *dependency) {
	//~ Node *node = create_node(dependency);
	//~ if (spec_node->dependencies) {
		//~ spec_node->dependencies_tail->next = node;
	//~ } else {
		//~ spec_node->dependencies = node;
	//~ }
	//~ spec_node->dependencies_tail = node;
	if (spec_node->dependencies) {
		Node *current = spec_node->dependencies;
		while (current->next) {
			current = current->next;
		}
		current->next = create_node(dependency);
	} else {
		spec_node->dependencies = create_node(dependency);
	}
}

void add_command(SpecNode *spec_node, char *command) {
	if (spec_node->commands) {
		Node *current = spec_node->commands;
		while (current->next) {
			current = current->next;
		}
		current->next = create_node(command);
	} else {
		spec_node->commands = create_node(command);
	}
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
