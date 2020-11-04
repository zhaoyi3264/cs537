/*
 * Specfication representation module
 * 
 * Authors: 
 * - Zhang, Zhaoyi, zhaoyi, zzhang825
 * - Li, Richard, richardl, tli354
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "spec_repr.h"

/*
 * Create a node in the linked list
 * 
 * data: data to store in the node
 * 
 * return: the created node
 */
Node *create_node(char *data) {
	Node *node = malloc(sizeof(Node));
	if (node == NULL) {
		exit(1);
	}
	node->data = malloc(sizeof(char) * strlen(data) + 1);
	if (node->data == NULL) {
		exit(1);
	}
	if (sprintf(node->data, "%s", data) < 0) {
		exit(1);
	}
	return node;
}

/*
 * Create a specification node
 * 
 * target: target name
 * 
 * return: the created specification node
 */
SpecNode *create_spec_node(char *target) {
	SpecNode *spec_node = malloc(sizeof(SpecNode));
	if (spec_node == NULL) {
		exit(1);
	}
 	spec_node->target = malloc(sizeof(char) * strlen(target) + 1);
 	if (spec_node->target == NULL) {
		exit(1);
	}
	if (sprintf(spec_node->target, "%s", target) < 0) {
		exit(1);
	}
	return spec_node;
}

/*
 * Add a dependency to the specification node
 * 
 * spec_node: specificaiton node
 * dependency: dependency
 */
void add_dependency(SpecNode *spec_node, char *dependency) {
	Node *node = create_node(dependency);
	if (spec_node->dependencies) {
		spec_node->dependencies_tail->next = node;
	} else {
		spec_node->dependencies = node;
	}
	spec_node->dependencies_tail = node;
}

/*
 * Add a command to the specification node
 * 
 * spec_node: specificaiton node
 * dependency: command
 */
void add_command(SpecNode *spec_node, char *command) {
	Node *node = create_node(command);
	if (spec_node->commands) {
		spec_node->commands_tail->next = node;
	} else {
		spec_node->commands = node;
	}
	spec_node->commands_tail = node;
}

/*
 * Print the dependencies of a specificaiton node
 * 
 * spec_node: specificaiton node
 */
void print_dependency(SpecNode *spec_node) {
	printf("dep: ");
	Node *current = spec_node->dependencies;
	while (current) {
		printf("%s, ", current->data);
		current = current->next;
	}
	printf("\n");
}

/*
 * Print the commands of a specificaiton node
 * 
 * spec_node: specificaiton node
 */
void print_command(SpecNode *spec_node) {
	printf("cmd:\n");
	Node *current = spec_node->commands;
	while (current) {
		printf("  %s\n", current->data);
		current = current->next;
	}
}
